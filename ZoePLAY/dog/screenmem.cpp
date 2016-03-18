/*
 * Copyright (c) 2016 by Arno Hafner (Austria)
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "screenmem.h"

 /*----------------------------
Func: init_screenmem
Desc: initializes the CAN Controller 
Vars: ---
------------------------------*/

 /*----------------------------
 Func: send
 Desc: Send the Screen memory to the given display
 Vars: display
 ------------------------------*/
 void screenmem::send(dog_7565R& DogX)
{
	uint8_t page_cnt, column_cnt;
	
	for (page_cnt=0; page_cnt<8; page_cnt++ )
	{
		DogX.position( 0, page_cnt);
    	for (column_cnt=0; column_cnt<128;column_cnt++)
	        DogX.data(screenmem::ScreenMem_1k[page_cnt*128+column_cnt]);
	}
}
	 
 /*----------------------------
 Func: draws pixel
 Desc: draws a pixel in the screen memory
 Vars: line (0..63) and column(0..127)
 ------------------------------*/
void screenmem::draw_pixel   ( uint8_t column, uint8_t line)
{
	uint16_t Mem_Pos; 
	uint8_t Pattern_pos;
	
	line=(line<64?line:63);
	column=(column<128?column:127);

	/* find screen memory position  */
	Mem_Pos = ((line/8) * 128) + column;
	
	/* find pattern position        */
	Pattern_pos = line % 8;
	
	/* draw the pixel into the Memory */
	ScreenMem_1k[Mem_Pos] |= 1<<Pattern_pos;

	
}

/*----------------------------
 Func: draw pattern
 Desc: draws a pattern of pixel in the screen memory
 Vars: line (0..63), column(0..127), height (0..8) and a pattern 
 ------------------------------*/
void screenmem::draw_pattern(uint8_t column, uint8_t line, uint8_t height, uint8_t pattern)
{
	uint8_t  Pattern_start;
	uint16_t Mem_Pos;
	uint8_t  Pattern_part;
	uint8_t  Mask;

	height=(height<9?height:8);
	line=(line+height<64?line:64-height);
	column=(column<128?column:127);

	/* find first screen memory position  */
	Mem_Pos = ((line/8) * 128) + column;
	/* find start position of pattern     */
	Pattern_start = line % 8;
	/* Mask of first page */
	Mask = (1<<height)-1;
	/* mask the pattern */
	Pattern_part = pattern & Mask;
	/* shift the Mask and the pattern  */
	/* clear the part */
	ScreenMem_1k[Mem_Pos] &= ~(Mask<<Pattern_start);
	/* set the pattern  */
	ScreenMem_1k[Mem_Pos] |= (Pattern_part<<Pattern_start);
	
	/* second page */
	if (Pattern_start+height>8)
	{
		Mem_Pos += 128;
		/* shift the Mask and the pattern  */
		/* clear the part */
		ScreenMem_1k[Mem_Pos] &= ~(Mask>>(8-Pattern_start));
		/* set the pattern  */
		ScreenMem_1k[Mem_Pos] |= (Pattern_part>>(8-Pattern_start));
	}
}

/*----------------------------
 Func: string
 Desc: draws a string in the screen memory
 Vars: line (0..63), column(0..127), font, Character
 ------------------------------*/
void screenmem::string      (uint8_t column, uint8_t line, const uint8_t *font_adress, const char *str)
{
	uint16_t pos_array; 										//position of character data in memory array
	uint8_t x, y, column_cnt, width_max;								//temporary column and page address, couloumn_cnt stand width_max are used to stay inside display area
	uint8_t start_code, last_code, width, height, bytes_p_string;	//font information, needed for calculation
	const char *string;

	
	
	start_code 	   = pgm_read_byte(&font_adress[2]);  //get first defined character
	last_code	   = pgm_read_byte(&font_adress[3]);  //get last defined character
	width		   = pgm_read_byte(&font_adress[4]);  //width in pixel of one character
	height         = pgm_read_byte(&font_adress[5]);  //height in pixel of one character
	bytes_p_string = pgm_read_byte(&font_adress[7]);  //bytes per string
	
	if(height + line > 64)      //stay inside display area
	height = 64 - line;
	
	
	//The string is displayed character after character. If the font has more then 8 dot height,
	//the top pixel are printed first, then the next page and so on.
	for(y = 0; y < (height+7)/8; y++)
	{
		column_cnt = column; //store column to start next line if biger than 8 pixel height. 
		string = str;             //temporary pointer to the beginning of the string to print
		while(*string != 0)
		{
			if((uint8_t)*string < start_code || (uint8_t)*string > last_code) //make sure data is valid, invalid ignored.
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
					if ((pgm_read_byte(&font_adress[pos_array+x]) == 0 ) &&        // TrueType Endet mit 0
					(pgm_read_byte(&font_adress[0]) == 84 ) &&                 // T steht im Header
					(x > 1))                                                   // Mindest Länge für TrueType
					
					x=width_max;
					
					else screenmem::draw_pattern(column_cnt++, line+8*y, height, pgm_read_byte(&font_adress[pos_array+x]));
				}
			}
		}
	}
}

/*----------------------------
 Func: draw line
 Desc: draws a line from point 1 to point 2 in the screen memory
 Vars: line 1 (0..63), column 1 (0..127), line 2 (0..63), column 2 (0..127)
 ------------------------------*/
void screenmem::draw_line   (int16_t column1, int16_t line1, int16_t column2, int16_t line2)
{
	int16_t dots;
	
	if (abs((int8_t)(line2-line1))>abs((int8_t)(column2-column1))) 	//the direction with more dots defines the necessary dots of the line.
	{
		if (line2>line1)
		{
			for (dots=line1;dots<=line2;dots++)
			{
				screenmem::draw_pixel(((column2-column1)*(dots-line1)/(line2-line1))+column1,dots );
			}
		} 
		else
		{
			for (dots=line2;dots<=line1;dots++)
			{
				screenmem::draw_pixel(((column1-column2)*(dots-line2)/(line1-line2))+column2,dots );
			}
		}
	}
	else
	{
		if ((line2-line1)==0) screenmem::draw_pixel(column1,line1 );
		else
		{
			if (column2>column1)
		    {
				for (dots=column1;dots<=column2;dots++)
				{
					screenmem::draw_pixel(dots, ((line2-line1)*(dots-column1)/(column2-column1))+line1);
				}
			} 
			else
			{
				for (dots=column2;dots<=column1;dots++)
				{
					screenmem::draw_pixel(dots, ((line1-line2)*(dots-column2)/(column1-column2))+line2);
				}
			}

		}
	}
}

/*----------------------------
 Func: picture
 Desc: shows a BLH-picture on the display (see BitMapEdit EA LCD-Tools (http://www.lcd-module.de/support.html))
 Vars: column (0..127) and page(0..7), program memory address of data
 ------------------------------*/

 void screenmem::picture( uint8_t column, uint8_t page, const uint8_t *pic_adress)
 {
	 uint8_t c,p;
	 unsigned int byte_cnt = 2; // Überspringen der Größeninformationen.
	 uint8_t width, page_cnt, column_cnt;
	 
	 width = pgm_read_byte(&pic_adress[0]);
	 page_cnt = (pgm_read_byte(&pic_adress[1]) + 7) / 8; //height in pages, add 7 and divide by 8 for getting the used pages (byte boundaries)
	 
	 if(width + column > 128) //stay inside display area
	 width = 128 - column;
	 if(page_cnt + page > 8)
	 page_cnt = 8 - page;
	 
	 for(p=0; p<page_cnt; p++)
	 {
		 column_cnt = column;
		 
		 for(c=0; c<width; c++)
		 screenmem::ScreenMem_1k[(page+p)*128+column_cnt++]=(pgm_read_byte(&pic_adress[byte_cnt++]));
		 
	 }

 }
 void screenmem::clear()
{
	uint16_t i;
    for (i=0; i<1024; i++) screenmem::ScreenMem_1k[i]=0;
}