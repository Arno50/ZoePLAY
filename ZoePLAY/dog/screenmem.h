/*
 * Copyright (c) 2016 by Arno Hafner (Austria)
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */


#ifndef CAN_SCREENMEM_H
#define CAN_SCREENMEM_H


class screenmem
{
	uint8_t ScreenMem_1k[1024];
	
	public:

	void send        (dog_7565R& DogX);
	void picture	 ( uint8_t column, uint8_t page, const uint8_t *pic_adress);
	void draw_pixel  ( uint8_t column, uint8_t line);
	void draw_pattern( uint8_t column, uint8_t line, uint8_t height, uint8_t pattern);
	void draw_line   ( int16_t column1, int16_t line1, int16_t column2, int16_t line2);
	void string      ( uint8_t column, uint8_t line, const uint8_t *font_adress, const char *str); 
	void clear       ();
};

#endif