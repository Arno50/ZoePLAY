/*
 * Copyright (c) 2016 by Arno Hafner (Austria)
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */


#ifndef CAN_SCREEN02_H
#define CAN_SCREEN02_H


class CAN_screen02
{
	public:

    void update        (CAN_Twizplay& CanX, screenmem& MEM0X);

	
    private:
	
    char string[10];
};
#endif