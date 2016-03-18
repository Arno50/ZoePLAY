/*
 * Copyright (c) 2016 by Arno Hafner (Austria)
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */


#ifndef KEY_H
#define KEY_H


class key
{
	public:

    void init        (volatile uint8_t *in_port, uint8_t dir_port, uint8_t pin);
	bool get         ();
	uint8_t get_duration ();
    void update      ();
	
    private:
	
	bool key_state;
	bool key_state_old;
	uint8_t pin_number;
	uint8_t duration;
	uint8_t duration_press;
	uint8_t duration_impuls;
	volatile uint8_t *port_nr;
	 
	
	
};
#endif