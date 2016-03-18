/*
 * Copyright (c) 2016 by Arno Hafner (Austria)
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "CAN_ZOE_Matrix.h"
#include "CAN_Twizplay.h"

 
 /*----------------------------
Func: init_CAN
Desc: initializes the CAN Controller 
Vars: ---
------------------------------*/

void CAN_Twizplay::init(void)
{

	CAN_tx_DDR |= 1<<CAN_tx_pin;
	CAN_rx_DDR &= ~(1<<CAN_rx_pin);
	
	CANGIE  = (1<<ENIT) + (1<<ENTX) + (1<<ENRX);       // Transmit/Receive  Interrupt for Transfer Protocol (TP)
	CANIE1  = (1<<IEMOB14) + (1<<IEMOB13);                // CANIE1 = MOB Interrupt for Transfer Protocol (TP) 13/14
	CANIE2  = 0x00;                        // CANIE2 = 0xH00 (0=kein Interrupt) disable interrupt
	CANBT1  = 0x02;                        // Geschwindigkeit CAN-BUS = 500 KBit/s
	CANBT2  = 0x0C;                        // Bitrate-Einstellung für Quarz 16 MHz
	CANBT3  = 0x37;                        // Bitrate-Einstellung für Quarz 16 MHz
	CANGCON = 0x02;                        // CAN-Kanal einschalten
    
	uint8_t Mob_count;
		
	for(Mob_count = 0; Mob_count<CAN_max_MOB; Mob_count++)
	{
	   CANPAGE  = Mob_count<<4;                                  // select MOb
	   CANSTMOB = 0x0;                                           // clear  status register
	   CANCDMOB = 0xC0;
	   CANIDT4  = 0x00;               // set ID register
	   CANIDT3  = 0x00;
	   CANIDT2  = (uint8_t)pgm_read_word(&(CAN_id[Mob_count]))<<5;
	   CANIDT1	= (uint8_t)((uint16_t)pgm_read_word(&(CAN_id[Mob_count]))>>3);              // 11 Nutzbit auf oberste 32Bit geschoben, weil LSB...
	   CANIDM4  = 0x0;                                           // set mask register
	   CANIDM3  = 0x0;
	   CANIDM2  = 0xFF;
	   CANIDM1  = 0xFF;
	}
}

 /*----------------------------
 Func: init_CAN transfert Protokoll
 Desc: initializes the CAN Controller
       Request on Mob 14 (0xE), CAN id 0x79b Lithium Battery Controller
	   Response on Mob 13 (0xD), CAN id 0x7bb Lithium Battery Controller
	   mirror Request on Mob 12 (0xC), CAN id 0x79b
 Vars: ---
 ------------------------------*/

void CAN_Twizplay::init_TP(uint8_t CAN_num_TP)
{
   // Mob 13 reserved TP response (RX)
   // Mob 14 reserved TP request (TX) 
			CANPAGE  = 13<<4;              // select MOb
			CANSTMOB = 0x0;                // clear  status register
			CANCDMOB = 0xC0;          
			CANIDT4  = 0x00;               // set ID register
			CANIDT3  = 0x00;
			CANIDT2  = (uint8_t)pgm_read_word(&(responseID[CAN_num_TP]))<<5;
			CANIDT1  = (uint8_t)((uint16_t)pgm_read_word(&(responseID[CAN_num_TP]))>>3);// 11 Nutzbit auf oberste 32Bit geschoben, weil LSB...
			CANIDM4  = 0x0;                // set mask register
			CANIDM3  = 0x0;
			CANIDM2  = 0xFF;
			CANIDM1  = 0xFF;

			CANPAGE  = 14<<4;              // select MOb
			CANSTMOB = 0x0;                // clear  status register
			CANCDMOB = 0x00;               // tx bit will be set later, not to start to early
			CANIDT4  = 0x00;               // set ID register
			CANIDT3  = 0x00;
			CANIDT2  = (uint8_t)pgm_read_word(&(requestID[CAN_num_TP]))<<5;
			CANIDT1	 = (uint8_t)((uint16_t)pgm_read_word(&(requestID[CAN_num_TP]))>>3);// 11 Nutzbit auf oberste 32Bit geschoben, weil LSB...
			CANIDM4  = 0x0;                // set mask register
			CANIDM3  = 0x0;
			CANIDM2  = 0xFF;
			CANIDM1  = 0xFF;
}

void CAN_Twizplay::Can_start_TP(uint8_t CAN_num_TP)
{
	CAN_Twizplay::send_count=0;
	CAN_Twizplay::Int_count=0;
	CAN_Twizplay::TP_count=0;
	CAN_Twizplay::stream_count=0;

		       CANPAGE  = 14<<4;  // Mob 14 reserved TP request (TX)
		       CANCDMOB = 8;   // dlc =8, less will not work, don't know why...
		       CANMSG = 0x02;  // only two bytes are good..
		       CANMSG = pgm_read_word(&(requestMODE[CAN_num_TP]));    // MODE $
		       CANMSG = pgm_read_word(&(requestPID [CAN_num_TP]));    // PID
		       CANMSG = 0x55;  // stuff..
		       CANMSG = 0x55;  // stuff..
		       CANMSG = 0x55;  // stuff..
		       CANMSG = 0x55;  // stuff..
		       CANMSG = 0x55;  // stuff..
		       
		       while (CANGSTA & (1<<TXBSY));  //  wait for txbsy
		       
		       CANCDMOB |= (1<<CONMOB0);  // tx now!
      
		       send_count++;
}

void CAN_Twizplay::Can_FlowControl_TP(uint8_t CAN_num_TP)
{
			   CANPAGE  = 14<<4;                                  // select MOb request
		       CANCDMOB = 8;   // dlc =8, less will not work, don't know why...
		       CANMSG = 0x30;  // FC (30) + ClearToSend (CTS) (0)
		       CANMSG = 0x00;  // BS Block Size: Zulässige Anzahl der folgenden CF, 0=all
		       CANMSG = 0x01;  // ST Min. Separation Time: Mindest- abstand der CF Frames in ms
		       CANMSG = 0x55;  // stuff..
		       CANMSG = 0x55;  // stuff..
		       CANMSG = 0x55;  // stuff..
		       CANMSG = 0x55;  // stuff..
		       CANMSG = 0x55;  // stuff..

		       while (CANGSTA & (1<<TXBSY));  //  wait for txbsy
		       
		       CANCDMOB |= (1<<CONMOB0);  // tx now!
		       
		       send_count++;
}


void CAN_Twizplay::store( uint8_t* TP_stream, uint8_t StreamSIZE)
{
    uint8_t Data_count;
	uint8_t First_char;
	uint8_t ResponseType;
	uint8_t SequenceNumber;
	uint8_t ResponseMode;
	uint8_t ResponsePID;
	uint8_t ResponseCOUNT;
	
	
	First_char = CANMSG;  // Msg #00
	ResponseType = First_char >> 4;
	SequenceNumber = First_char & 0x0F;
    CAN_Twizplay::TP_status = ResponseType;		
	
	switch (ResponseType)
	{
		case 0x00:  // Single Frame	
		case 0x01:	// First Frame

			CAN_Twizplay::TP_HEX1[0] = First_char;
			ResponseCOUNT = CANMSG;  // Msg #01
			CAN_Twizplay::TP_HEX1[1] = ResponseCOUNT;
			ResponseMode = CANMSG;   // Msg #02
			CAN_Twizplay::TP_HEX1[2] = ResponseMode;
			ResponsePID = CANMSG;    // Msg #03
			CAN_Twizplay::TP_HEX1[3] = ResponsePID;
		
			for (Data_count = 4; Data_count < 8; Data_count++)  // get data Msg #04 - #07
			{
				CAN_Twizplay::TP_HEX1[Data_count] = CANMSG;             
				if (CAN_Twizplay::stream_count < StreamSIZE)                 // stop storing, if buffer is to small... 
				{
					* (TP_stream+CAN_Twizplay::stream_count++) = CAN_Twizplay::TP_HEX1[Data_count];
				}
			}
			CANSTMOB = 0x00;                                       // reset status register
			CANCDMOB = 0xC0;                                       // restart MOB
			TP_count++;
	   break;
	   case 0x02:	// Consecutive Frame
	   
			CAN_Twizplay::TP_HEX2[0] = First_char;
			for (Data_count = 1; Data_count < 8; Data_count++)  // get data Msg #01 - #07
			{
				CAN_Twizplay::TP_HEX2[Data_count] = CANMSG;
				if (CAN_Twizplay::stream_count < StreamSIZE)                 // stop storing, if buffer is to small...
				{
					 * (TP_stream + CAN_Twizplay::stream_count++) = CAN_Twizplay::TP_HEX2[Data_count];
				}
			}
			CANSTMOB = 0x00;                                       // reset status register
			CANCDMOB = 0xC0;                                       // restart MOB
			TP_count++;
  
	   break;
	   default:
			CAN_Twizplay::TP_HEX3[0] = First_char;                      
			for (Data_count = 1; Data_count < 8; Data_count++)
			{
				CAN_Twizplay::TP_HEX3[Data_count] = CANMSG;                       // get data
			}
			CANSTMOB = 0x00;                                       // reset status register
			CANCDMOB = 0xC0;                                       // restart MOB

	   break;
	}
	   if (CAN_Twizplay::stream_count == StreamSIZE) CAN_Twizplay::TP_status=0xFF;  // Stream is full, TP ready ?
}

void CAN_Twizplay::Can_rx(void)
{
    uint8_t Data_count;
    uint8_t Mob_count;
    uint8_t Can_rx_dlc;
	
	CAN_OFF_Count++;	
	for(Mob_count = 0; Mob_count<8; Mob_count++)
	{
	   CANPAGE  = Mob_count<<4;                                  // select MOb
	   if (CANSTMOB & (1<<5))                                        // bit 5 = RXOK
       {
		    CAN_OFF_Count = 0;
    	    Can_rx_dlc = CANCDMOB & 0x0F;                               // get length of data field, DLC 0-3
	        for (Data_count = 0; Data_count < Can_rx_dlc; Data_count++)
	        {
	           Can_rx_data[Mob_count*8+Data_count] = CANMSG;                       // get data
        	} 
	        CANSTMOB = 0x00;                                       // reset status register
	        CANCDMOB = 0xC0;                                       // restart MOB
	   }
	}

/**********Test Daten
      Can_rx_data[0] = 0x9A;
      Can_rx_data[1] = 0x27;
      Can_rx_data[2] = 0xD0;
      Can_rx_data[3] = 0xE2;
      Can_rx_data[4] = 0x44;
      Can_rx_data[5] = 0x05;
      Can_rx_data[6] = 0x60;
      Can_rx_data[7] = 0x03;
**********Test Daten */

    uint8_t Msg_count;
    uint32_t CAN_chanal_u32;

	for(Msg_count=0; Msg_count < 8; Msg_count++)
	{
	    CAN_chanal_u32 = 0;
        for(Data_count = pgm_read_byte(&(Msg_MOB[Msg_count]))*8+pgm_read_byte(&(Msg_startbit[Msg_count]))/8;
		   (Data_count < (pgm_read_byte(&(Msg_MOB[Msg_count]))*8+pgm_read_byte(&(Msg_stopbit[Msg_count]))/8)+1); 
		    Data_count++)
		{
	       CAN_chanal_u32 = CAN_chanal_u32<<8;
	       CAN_chanal_u32 = CAN_chanal_u32 + Can_rx_data[Data_count];
		}
	    CAN_chanal_u32 = CAN_chanal_u32 >> (7 - pgm_read_byte(&(Msg_stopbit[Msg_count])) % 8);    // Anzahl der gültigen Bits im letzten Byte
	    CAN_chanal_u32 = CAN_chanal_u32 & (((uint32_t)1<<(pgm_read_byte(&(Msg_stopbit[Msg_count])) - pgm_read_byte(&(Msg_startbit[Msg_count])) +1))-1);// Maske der gültigen Bit
	    CAN_chanal[Msg_count]=CAN_chanal_u32*pgm_read_float(&(calc_factor[Msg_count]));
	    CAN_chanal[Msg_count]=CAN_chanal[Msg_count]+pgm_read_float(&(calc_offset[Msg_count]));
	}
}
