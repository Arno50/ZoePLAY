/*
 * Copyright (c) 2016 by Arno Hafner (Austria)
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "screen01.h"

 /*----------------------------
Func: init_screen1
Desc: initializes the CAN Controller 
Vars: ---
------------------------------*/
void CAN_screen01::update10ms	(CAN_Twizplay& CanX)
{
	if (CountSOC<12000) CountSOC++;
	else { dSOC = 0; old_CountSOC =0;}
	if ((old_SOC != CanX.CAN_chanal[caSOC])&&(old_SOC!=0))  // the dSOC is calculated out of changing SOC, at 43kWh every 0,4 Sec.
		{
			dSOC=120/(float)CountSOC;
			old_CountSOC = CountSOC;
			CountSOC = 0;
		}
	if (old_CountSOC<CountSOC) dSOC=120/(float)CountSOC;  // calculate lower dSOC when time is longer than last. (will decrease)  
    if (2*old_CountSOC<CountSOC) dSOC = 0; // after the double of time (half of the dSOC) output is set to 0.
	if (old_SOC > CanX.CAN_chanal[caSOC]) dSOC*=-1;
	old_SOC= CanX.CAN_chanal[caSOC];
}
 /*----------------------------
 Func: screen1
 Desc: initializes the CAN Controller
 Vars: ---
 ------------------------------*/
 void CAN_screen01::update(CAN_Twizplay& CanX, screenmem& MEM0X)
 {
		MEM0X.clear();
        char string_loc[10];
	 	// screen 1:
	 	dtostrf(CanX.CAN_chanal[caCPC],6,2,string_loc);
	 	MEM0X.string(0,0,font_6x8,"Pilot Cur.");
	 	MEM0X.string(74,0,font_6x8,string_loc);
	 	MEM0X.string(114,0,font_6x8,"A");
	 	dtostrf(CanX.CAN_chanal[caChP],6,2,string_loc);
	 	MEM0X.string(0,8,font_6x8,"MaxCharPwr");
	 	MEM0X.string(74,8,font_6x8,string_loc);
	 	MEM0X.string(114,8,font_6x8,"kW");
	 	dtostrf(CanX.CAN_chanal[caACP],6,2,string_loc);
	 	MEM0X.string(0,16,font_6x8,"AvailCharPwr");
	 	MEM0X.string(74,16,font_6x8,string_loc);
	 	MEM0X.string(114,16,font_6x8,"kW");
	 	dtostrf(CanX.CAN_chanal[caSOC],6,2,string_loc);
	 	MEM0X.string(0,24,font_6x8,"SOC->Batt");
	 	MEM0X.string(74,24,font_6x8,string_loc);
	 	MEM0X.string(114,24,font_6x8,"%");
		dtostrf(dSOC,6,2,string_loc); 
		MEM0X.string(74,32,font_6x8,string_loc);
		MEM0X.string(0,32,font_6x8,"Ladung/min");
		MEM0X.string(111,32,font_6x8,"%/m");
		dtostrf(CanX.CAN_chanal[caEng],6,2,string_loc);
	 	MEM0X.string(0,40,font_6x8,"Energy->Batt");
	 	MEM0X.string(74,40,font_6x8,string_loc);
	 	MEM0X.string(112,40,font_6x8,"k"); MEM0X.string(117,40,font_6x8,"Wh");
	 	dtostrf(CanX.CAN_chanal[caSOH],6,2,string_loc);
	 	MEM0X.string(0,48,font_6x8,"SOH->Batt:");
	 	MEM0X.string(74,48,font_6x8,string_loc);
	 	MEM0X.string(114,48,font_6x8,"%");
	 	dtostrf(CanX.CAN_chanal[caBaT],6,2,string_loc);
	 	MEM0X.string(0,56,font_6x8,"Batt.Temp.");
	 	MEM0X.string(74,56,font_6x8,string_loc);
		MEM0X.draw_pattern(115,56,8,6); MEM0X.draw_pattern(116,56,8,9); MEM0X.draw_pattern(117,56,8,9); MEM0X.draw_pattern(118,56,8,6); //° 
	 	MEM0X.string(120,56,font_6x8,"C");

	 	// screen 1 end
 }