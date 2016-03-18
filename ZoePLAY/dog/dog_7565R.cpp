/*
 * Copyright (c) 2014 by ELECTRONIC ASSEMBLY <technik@lcd-module.de>
 * EA DOG Graphic (ST7565R) software library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */


#include "dog_7565R.h"

/*----------------------------
Func: init_display
Desc: initializes the entire DOG-Display
Vars: ---
------------------------------*/

void dog_7565R::init(void)
{
    /* preset the Signal for Display                       */
    DOGM_A0   |=   1<<DOGM_A0_pin;     // Set A0
    DOGM_CS1  |=   1<<DOGM_CS1_pin;     // Set CS
    DOGM_SCLK |=   1<<DOGM_SCLK_pin;    // set SCLK
    DOGM_Reset|=   1<<DOGM_Reset_pin;  // Set Reset Pin
    DOGM_SI   |=   1<<DOGM_SI_pin;     // Set SI
    /* Set Signals for display as output                   */
    DDRC      |=  (1<<DOGM_A0_pin)+
                  (1<<DOGM_CS1_pin)+
                  (1<<DOGM_SI_pin)+
                  (1<<DOGM_SCLK_pin)+
                  (1<<DOGM_Reset_pin);
    _delay_ms(50);
    /* Initialisierungssequenz des Display */
    DOGM_CS1  &= ~(1<<DOGM_CS1_pin);     // clear CS
    DOGM_A0   &= ~(1<<DOGM_A0_pin);      // clear A0
    spi_out(0x40);    //Display start line 0
    spi_out(0xA1);    //ADC reverse
    spi_out(0xC0);    //Normal COM0~COM63
    spi_out(0xA6);    //Display normal
    spi_out(0xA2);    //Set bias 1/9 (Duty 1/65)
    spi_out(0x2F);    //Booster, Regulator and Follower on
    spi_out(0xF8);    //Set internal Power supply operating mode
    spi_out(0x00);    //Set internal Booster to 4x
    spi_out(0x27);    // >2. Byte mit Wert
    spi_out(0x81);    //Contrast set
    spi_out(0x16);    // >2. Byte mit Wert
    spi_out(0xAC);    //No indicator off
    spi_out(0x00);    // >2. Byte mit Wert (no flashing mode)
    spi_out(0xAF);    //Display on

    clear();
}
/*----------------------------
Func: clear_display
Desc: clears the entire DOG-Display
Vars: ---
------------------------------*/

void dog_7565R::clear(void) 
{
	uint8_t page, column;
	uint8_t page_cnt = 8, column_cnt = 128;

	for(page = 0; page < page_cnt; page++) //Display has 8 pages
	{
		position( 0, page);
		for(column = 0; column < column_cnt; column++) //clear the whole page line
			data(0x00);
	}
}

/*----------------------------
Func: contrast
Desc: sets contrast to the DOG-Display
Vars: byte contrast (0..63)
------------------------------*/
void dog_7565R::contrast(uint8_t contr) 
{
	command(0x81);   		//double byte command
	command(contr&0x3F);	//contrast has only 6 bits
}


/*----------------------------
Func: string
Desc: shows string with selected font on position
Vars: column (0..127/131), page(0..3/7),  font adress in programm memory, stringarray
------------------------------*/
void dog_7565R::string(uint8_t column, uint8_t page, const uint8_t *font_adress, const char *str)
{
	uint16_t pos_array; 										//Postion of character data in memory array
	uint8_t x, y, column_cnt, width_max;								//temporary column and page adress, couloumn_cnt tand width_max are used to stay inside display area
	uint8_t start_code, last_code, width, page_height, bytes_p_string;	//font information, needed for calculation
	const char *string;

	
	
	start_code 	   = pgm_read_byte(&font_adress[2]);  //get first defined character
	last_code	   = pgm_read_byte(&font_adress[3]);  //get last defined character
	width		   = pgm_read_byte(&font_adress[4]);  //width in pixel of one character
	page_height    = pgm_read_byte(&font_adress[6]);  //page count per charactar
	bytes_p_string = pgm_read_byte(&font_adress[7]);  //bytes per string
  
  if(page_height + page > 8)      //stay inside display area
		page_height = 8 - page;
  
	
	//The string is displayed character after character. If the font has more then one page, 
	//the top page is printed first, then the next page and so on
	for(y = 0; y < page_height; y++)
	{
		position(column, page + y ); //set postpositions and page
		column_cnt = column; //store column for display last column check
		string = str;             //temporary pointer to the beginning of the string to print
		while(*string != 0)
		{	
			if((uint8_t)*string < start_code || (uint8_t)*string > last_code) //make sure data is valid
				string++;
			else
			{							
				//calculate position of ascii character in font array
				//bytes for header + (ascii - startcode) * bytes per uint8_t)
				pos_array = 8 + (uint16_t)(*string++ - start_code) * bytes_p_string;
				pos_array += y*width; //get the dot pattern for the part of the uint8_t to print
        
                if(column_cnt + width > 128) //stay inside display area
		     		width_max = 128-column_cnt;
	        	else
		    		width_max = width;
          
				for(x=0; x < width_max; x++) //print the whole character 
				{
					if ((pgm_read_byte(&font_adress[pos_array+x]) == 0 ) &&        // TT Endet mit 0
					    (pgm_read_byte(&font_adress[0]) == 84 ) &&                 // TT steht im Header
						(x > 4))                                                   // Mindest Länge für TT
						
						x=width_max;  
						   
					else data(pgm_read_byte(&font_adress[pos_array+x]));
				}
			}
		}
	}
}

/*----------------------------
Func: rectangle
Desc: shows a pattern filled rectangle on the display
Vars: start and end column (0..127/131) and page(0..3/7), bit pattern
------------------------------*/

void dog_7565R::rectangle(uint8_t start_column, uint8_t start_page, uint8_t end_column, uint8_t end_page, uint8_t pattern)  
{
	uint8_t x, y;
  
  if(end_column > 128) //stay inside display area
		end_column = 128;
 
  if(end_page > 7)
		end_page = 7;
  
  for(y=start_page; y<=end_page; y++)
	{
		position(start_column, y );
	
		for(x=start_column; x<=end_column; x++)
			data(pattern);
		
	}
}

/*----------------------------
Func: picture
Desc: shows a BLH-picture on the display (see BitMapEdit EA LCD-Tools (http://www.lcd-module.de/support.html))
Vars: column (0..127/131) and page(0..3/7), program memory adress of data
------------------------------*/

void dog_7565R::picture( uint8_t column, uint8_t page, const uint8_t *pic_adress)  
{
	uint8_t c,p;
	unsigned int byte_cnt = 2; // Überspringen der Größeninformationen.
	uint8_t width, page_cnt;
		
	width = pgm_read_byte(&pic_adress[0]);
	page_cnt = (pgm_read_byte(&pic_adress[1]) + 7) / 8; //height in pages, add 7 and divide by 8 for getting the used pages (byte boundaries)
	     
    if(width + column > 128) //stay inside display area
		width = 128 - column;
    if(page_cnt + page > 8)
		page_cnt = 8 - page;
	
	for(p=0; p<page_cnt; p++)
	{
		position( column, page + p );
	
		for(c=0; c<width; c++)
			data(pgm_read_byte(&pic_adress[byte_cnt++]));
		
	}
}


/*----------------------------
Func: position
Desc: sets write pointer in DOG-Display
Vars: column (0..127/131), page(0..3/7)
------------------------------*/
void dog_7565R::position(uint8_t column, uint8_t page )  
{
		
	command(0x10 + (column>>4)); 	//MSB adress column
	command(0x00 + (column&0x0F));	//LSB adress column
	command(0xB0 + (page&0x0F)); 	//adress page	
}

/*----------------------------
Func: command
Desc: Sends a command to the DOG-Display
Vars: data
------------------------------*/
void dog_7565R::command(uint8_t dat) 
{
  	DOGM_A0 &= ~(1<<DOGM_A0_pin);    // clear A0
	spi_out(dat);
}

/*----------------------------
Func: data
Desc: Sends data to the DOG-Display
Vars: data
------------------------------*/
void dog_7565R::data(uint8_t dat) 
{
  	 DOGM_A0 |= 1<<DOGM_A0_pin;    // set A0
	 spi_out(dat);
}

/*----------------------------
Func: spi_out
Desc: Sends one Byte, no CS
Vars: data
------------------------------*/
void dog_7565R::spi_out(uint8_t dat) 
{
	uint8_t i = 8;
		do 
		{
			if(dat & 0x80)
			    {
				DOGM_SI |= 1<<DOGM_SI_pin;      // Setze Datenbit="1" auf SI
				}
			else
			    {
                DOGM_SI &= ~(1<<DOGM_SI_pin);   // Setze Datenbit="0" auf SI
				}
			DOGM_SCLK &= ~(1<<DOGM_SCLK_pin);  // Clock vorbereiten
			DOGM_SCLK |= 1<<DOGM_SCLK_pin;     // Positive Flanke, Bit übernehmen
			dat <<= 1;	   
    	}while(--i);
}

