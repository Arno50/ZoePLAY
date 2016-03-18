/*
 * Copyright (c) 2016 by Arno Hafner (Austria)
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "key.h"

 /*----------------------------
Func: init_key
Desc: initializes the key Port
Vars: ---
------------------------------*/
    void key::init        (volatile uint8_t *in_port, uint8_t dir_port, uint8_t pin)
	{
			/* Set input direction switches  */
			dir_port &= ~(1<<pin);
			port_nr = in_port;
			pin_number = pin;
	}
	
 /*----------------------------
 Func: get_key
 Desc: gets the bounced and stored value
 Vars: ---
 ------------------------------*/	
    bool key::get         ()
	{
		return key::key_state;
	}

	uint8_t key::get_duration()
	{
		return key::duration;
	}
	
/*----------------------------
 Func: update
 Desc: Wait 100ms before starting a counter and no Impulse, start an Impulse of 150ms after key stopped pressed.
 Vars: ---
 ------------------------------*/	
    void key::update      ()
	{
		if(!(*port_nr & 1<<pin_number)) 
		{
			key::duration_press++;
			key::key_state_old=true;
			key::duration_impuls = 0;
		}
		if (key::duration_press > 100)  // Maximal 1s drücken ergibt einen Impuls später geht nur der Zähler hoch.
		{
			key::duration_press  = 100;
			key::key_state_old = 0;
			key::duration++;
			if (key::duration > 250) key::duration=250;

		}
		if(*port_nr & 1<<pin_number)
		{
			key::duration_press = 0;
			key::duration  = 0;
			if (key_state_old)
			{
				key::duration_impuls++;
				key::key_state = 1;
				if (key::duration_impuls>15) // Impuls ist 150ms
				{
					key::duration_impuls = 0;
					key::key_state = 0;
					key::key_state_old = 0;
					
				}
			}
			
		}
		
	}
