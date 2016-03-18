/*
 * IO_Test_V01.cpp
 *
 * Created: 26.01.2016 22:21:18
 * Author : AbRiNgOi
 */ 

#define F_CPU 16000000UL  // 16 MHz
#include <avr/io.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h> 
// #include <string.h>

#include "mega/iocanxx.h"
#include "config/conf_io_twizplay.h"
#include "key/key.cpp"
#include "dog/dog_7565R.cpp"
#include "dog/font_16x32nums.h"
#include "dog/font_6x8.h"
#include "dog/font_8x16.h"
#include "dog/font_8x8.h"
#include "dog/font_8x8TT.h"
#include "dog/font_4x6nums.h"
#include "zoe_logo_inv.h"
#include "CAN/CAN_Twizplay.cpp"
#include "dog/screenmem.cpp"
#include "screens/screen01.cpp"
#include "screens/screen02.cpp"
#include "screens/screen03.cpp"
#include "eeprom/eeprom.cpp"

CAN_Twizplay CAN1;  
dog_7565R DOG;
screenmem    MEM01;
CAN_screen01 SCR01;
CAN_screen02 SCR02;
CAN_screen03 SCR03;

key KEY01;
key KEY02;
key KEY03;

EEPROM EEPROM01;

char string[20];
uint8_t count_100ms = 0;
uint8_t old_count100ms = 0;
uint8_t count_10ms = 0;
uint8_t brightness;
uint8_t screen_nr;
uint8_t old_screen_nr;
uint8_t * Ptr_Stream;
uint8_t Act_StreamSIZE;

int main(void)
{
   
	DDRB |= (1<<PB0);  // Relays on Pin PB0 as Output
	PORTB |= (1<<PB0); // switch Relays on	

	// initialize the Timer for fast scheduler 10ms
	TCCR1A = 0;
	TCCR1B |= _BV (1);       // CC11 setzt prescaler 8
	TCCR1B |= _BV (3);       // WGM12
	OCR1A = 20000;           // Compare Wert 20000-Prescale 8 = 10ms
	TIMSK1 |= _BV (OCIE1A);  // Interrupt für Compare Timer 1 freigeben


    // initialize timer0 in PWM mode for back light
    TCCR0A |= (1<<WGM00)|(1<<COM0A1)|(1<<WGM01)|(1<<CS00);
    // make sure to make OC0 pin (pin PB3 for AT90CAN128) as output pin
    DDRB |= (1<<PB7); 
	OCR0A =  50; // brightness for back light.

	DOG.init();
    CAN1.init();
    KEY01.init(&Taster1, Taster1_dir, Taster1_pin);
    KEY02.init(&Taster2, Taster2_dir, Taster2_pin);
    KEY03.init(&Taster3, Taster3_dir, Taster3_pin);
	EEPROM01.init();
	
    MEM01.picture(0,0,zoe_logo_inv);
	MEM01.send(DOG);
	
	_delay_ms(2000);
    MEM01.clear();
	screen_nr = 1;
	sei ();                  // Interrupt release global
	
   while (1)
   {
		if (old_count100ms != count_100ms)
		{
			old_count100ms = count_100ms;
			switch (screen_nr)
			{
				case 1: SCR01.update(CAN1,MEM01);
				break;
				case 2: SCR02.update(CAN1,MEM01);
  						if (!(count_100ms%50) && (CAN1.TP_status == 0 || CAN1.TP_status == 0xFF))  // every 5 Sec.
						{
							CAN1.init_TP(tpCell1);
							Ptr_Stream = CAN1.TP_stream1; // Ptr = Array or Ptr = &Array[0] ... it is the same! :)
							Act_StreamSIZE = SzCell1;
							CAN1.Can_start_TP(tpCell1);
						}
						if (CAN1.TP_status == 1)
						{
							CAN1.Can_FlowControl_TP(tpCell1);
						}
				break;
				case 3: SCR03.update(CAN1,MEM01);
				break;
				default: screen_nr = 1;
				break;
			}
			
			if 	((screen_nr!=old_screen_nr) &&  !(KEY01.get() || KEY03.get() ))		
			{
				old_screen_nr = screen_nr;
			}
			if 	(screen_nr == old_screen_nr)
			{
				if (KEY01.get())
				{
					MEM01.clear();
					screen_nr++;
				}
				if (KEY03.get())
				{
					MEM01.clear();
					screen_nr--;
				}
			}
			if (KEY03.get_duration()>0) OCR0A-=5;
			if (KEY01.get_duration()>0) OCR0A+=5;
			
			MEM01.send(DOG);
		}
		
    _delay_us(1);
   }
}

ISR(TIMER1_COMPA_vect)
{
              CAN1.Can_rx();
			  KEY01.update(); 
			  KEY02.update();
			  KEY03.update();
			  SCR01.update10ms(CAN1);
		  
	  		  count_10ms++;
			  if (count_10ms == 10)
			  {
				  count_10ms = 0;
				  count_100ms++;
			  }
			  if (CAN1.CAN_OFF_Count > 100) PORTB &= ~(1<<PB0);  // Switch of after 1s with no new CAN message.
}

ISR(CANIT_vect)
{
   uint8_t save_canpage = CANPAGE;		//  actual CANPAGE (my be interrupted by sending...)

	  if (CANSIT1 & (1<<SIT13))         //  only Interrupt on MOB 13 is for transport protocol RX
	  {
			CANPAGE = (13<<4);          //  set CAN Page 13 (reserved for receive transport protocol).
		    if (CANSTMOB & (1<<RXOK))   //  receive OK
		    {
				CAN1.store(Ptr_Stream, Act_StreamSIZE);	//  store the received messages into buffer
			    CANSTMOB &= ~(1<<RXOK);	//  reset INT reason
			    CAN1.Int_count++;       //  count all receive Interrupts  
		    }
		    CANPAGE = save_canpage;     //  restore CANPAGE
	  }
	  
      if (CANSIT1 & (1<<SIT14))         //  only Interrupt on MOB 14 is for transport protocol TX
	  { 
		  CANPAGE = (14<<4);            //  set CAN Page 14 (reserved for transmit transport protocol). 
		  if (CANSTMOB & (1<<TXOK))     //  transmit OK
	      {
			  CANSTMOB &= ~(1<<TXOK);	//  reset INT reason
			  CAN1.Int_count++;         //  count all transmit Interrupts
		  }
		  CANPAGE = save_canpage;       //  restore CANPAGE
	  }
}