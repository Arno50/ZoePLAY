/*
 * Copyright (c) 2016 by Arno Hafner (Austria)
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "screen03.h"

 /*----------------------------
Func: init_screen2
Desc: initializes the CAN Controller 
Vars: ---
------------------------------*/

 /*----------------------------
 Func: screen2
 Desc: initializes the CAN Controller
 Vars: ---
 ------------------------------*/
 void CAN_screen03::update(CAN_Twizplay& CanX, screenmem& MEM0X)
 {
        char string_loc2[10];
		uint8_t Line;
		uint8_t position;
		float voltage;
		
		
		for (Line=0;Line<11;Line++)
		{
			for (position=0;(position<6)&&((Line*6+position)<62);position++)
			{	
				voltage = (float)((((int16_t)CanX.TP_stream1[(position+Line*6)*2]<<8) + CanX.TP_stream1[((position+Line*6)*2)+1]))/1000;
				dtostrf(voltage,1,3,string_loc2);
				MEM0X.string((position*21),(Line*6),font_4x6,string_loc2);
			}
		}
 }