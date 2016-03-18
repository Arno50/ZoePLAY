/*
 * Copyright (c) 2016 by Arno Hafner (Austria)
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "screen02.h"

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
 void CAN_screen02::update(CAN_Twizplay& CanX, screenmem& MEM0X)
 {
				 
			 	itoa(CanX.TP_HEX1[0],string,16);
			 	MEM0X.string(0,40,font_6x8,string);
			 	itoa(CanX.TP_HEX1[1],string,16);
			 	MEM0X.string(16,40,font_6x8,string);
			 	itoa(CanX.TP_HEX1[2],string,16);
			 	MEM0X.string(32,40,font_6x8,string);
			 	itoa(CanX.TP_HEX1[3],string,16);
			 	MEM0X.string(48,40,font_6x8,string);
			 	itoa(CanX.TP_HEX1[4],string,16);
			 	MEM0X.string(64,40,font_6x8,string);
			 	itoa(CanX.TP_HEX1[5],string,16);
			 	MEM0X.string(80,40,font_6x8,string);
			 	itoa(CanX.TP_HEX1[6],string,16);
			 	MEM0X.string(96,40,font_6x8,string);
			 	itoa(CanX.TP_HEX1[7],string,16);
			 	MEM0X.string(112,40,font_6x8,string);

			 	itoa(CanX.TP_HEX2[0],string,16);
			 	MEM0X.string(0,48,font_6x8,string);
			 	itoa(CanX.TP_HEX2[1],string,16);
			 	MEM0X.string(16,48,font_6x8,string);
			 	itoa(CanX.TP_HEX2[2],string,16);
			 	MEM0X.string(32,48,font_6x8,string);
			 	itoa(CanX.TP_HEX2[3],string,16);
			 	MEM0X.string(48,48,font_6x8,string);
			 	itoa(CanX.TP_HEX2[4],string,16);
			 	MEM0X.string(64,48,font_6x8,string);
			 	itoa(CanX.TP_HEX2[5],string,16);
			 	MEM0X.string(80,48,font_6x8,string);
			 	itoa(CanX.TP_HEX2[6],string,16);
			 	MEM0X.string(96,48,font_6x8,string);
			 	itoa(CanX.TP_HEX2[7],string,16);
			 	MEM0X.string(112,48,font_6x8,string);
				 
			 	itoa(CanX.TP_HEX3[0],string,16);
			 	MEM0X.string(0,56,font_6x8,string);
			 	itoa(CanX.TP_HEX3[1],string,16);
			 	MEM0X.string(16,56,font_6x8,string);
			 	itoa(CanX.TP_HEX3[2],string,16);
			 	MEM0X.string(32,56,font_6x8,string);
			 	itoa(CanX.TP_HEX3[3],string,16);
			 	MEM0X.string(48,56,font_6x8,string);
			 	itoa(CanX.TP_HEX3[4],string,16);
			 	MEM0X.string(64,56,font_6x8,string);
			 	itoa(CanX.TP_HEX3[5],string,16);
			 	MEM0X.string(80,56,font_6x8,string);
			 	itoa(CanX.TP_HEX3[6],string,16);
			 	MEM0X.string(96,56,font_6x8,string);
			 	itoa(CanX.TP_HEX3[7],string,16);
			 	MEM0X.string(112,56,font_6x8,string);

					 	dtostrf(CanX.send_count,3,0,string);
					 	MEM0X.string(0,0,font_6x8,"SendCount:");
					 	MEM0X.string(80,0,font_6x8,string);
 
					 	dtostrf(CanX.Int_count,3,0,string);
					 	MEM0X.string(0,8,font_6x8,"IntCount:");
					 	MEM0X.string(80,8,font_6x8,string);

					 	dtostrf(CanX.TP_count,3,0,string);
					 	MEM0X.string(0,16,font_6x8,"TPCount:");
					 	MEM0X.string(80,16,font_6x8,string);
					 	
					 	dtostrf(CanX.stream_count,3,0,string);
					 	MEM0X.string(0,24,font_6x8,"StreamCount:");
					 	MEM0X.string(80,24,font_6x8,string);
						 
						dtostrf(CanX.TP_status,3,0,string);
						MEM0X.string(0,32,font_6x8,"TP_Status");
					 	MEM0X.string(80,32,font_6x8,string);						
				 
 }