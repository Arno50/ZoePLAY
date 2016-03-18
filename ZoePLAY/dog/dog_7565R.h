/*
 * Copyright (c) 2014 by ELECTRONIC ASSEMBLY <technik@lcd-module.de>
 * EA DOG Graphic (ST7565R) software library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef DOG_7565R_H
#define DOG_7565R_H



class dog_7565R
{
  public:

    void init           (void);
    void clear			(void);
    void contrast       (uint8_t contr);
	void string         (uint8_t column, uint8_t page, const uint8_t *font_adress, const char *str);
	void rectangle		(uint8_t start_column, uint8_t start_page, uint8_t end_column, uint8_t end_page, uint8_t pattern);
	void picture		(uint8_t column, uint8_t page, const uint8_t *pic_adress);
    void data		    (uint8_t dat);
	void position       (uint8_t column, uint8_t page);
    void command	    (uint8_t dat);

  private:
	void spi_out		(uint8_t dat);


    
};

#endif
