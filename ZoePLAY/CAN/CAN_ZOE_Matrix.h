/*
 * Copyright (c) 2016 by Arno Hafner (Austria)
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */


#ifndef CAN_ZOE_MATRIX_H
#define CAN_ZOE_MATRIX_H

enum { CAN_max_MOB = 13 };
const uint16_t CAN_id[CAN_max_MOB] PROGMEM =
{
	0x42E,    // Mob 0:   Charging Pilot Current,0x42e,38,43,Zeile 125
	          // Mob 0:   State of Charge, 0x42e, 0,12 Zeile 121
	          // Mob 0:   HVBatteryTemp, 0x42e,44,50, Zeile 126
	          // Mob 0:   ChargingPower
	0x5D7,    // Mob 1:   speed
	0x1FD,    // Mob 2:   Im Display angezeigte Leistung in kW
	0x427,    // Mob 3:   AvailableEnergy, 0x427, 49,57 Zeile 111
	          // Mob 3:   AvailableChargingPower
	0x658,    // Mob 4:   Battery Health, 0x658,33,39,Zeile 193
	0x100,    // Mob 5:   empty
	0x100,    // Mob 6:   empty
	0x100,    // Mob 7:   empty
	0x100,    // Mob 8:   empty
	0x100,    // Mob 9:   empty
	0x100,    // Mob 10:  empty
	0x100,    // Mob 11:  empty
	0x100 };  // Mob 12:  empty
              // Mob 13:  Reserved for Transport-protocol 2.0 request (TX)
              // Mob 14:  Reserved for Transport-protocol 2.0 response (RX)

enum { CAN_max_Chan = 8 };
enum                                               {caCPC,caChP,caSOC,caPWR,caEng,caACP,caSOH,caBaT};
const uint8_t Msg_MOB[CAN_max_Chan]      PROGMEM = {    0,    0,    0,    2,    3,    3,    4,    0};
const uint8_t Msg_startbit[CAN_max_Chan] PROGMEM = {   38,   56,    0,   48,   49,   40,   33,   44};
const uint8_t Msg_stopbit[CAN_max_Chan]  PROGMEM = {   43,   63,   12,   55,   57,   47,   39,   50};
const float calc_factor[CAN_max_Chan]    PROGMEM = {  1.0,  0.3, 0.02,  1.0,  0.1,  0.3,  1.0,  1.0};
const float calc_offset[CAN_max_Chan]    PROGMEM = {  0.0,  0.0,  0.0,-80.0,  0.0,  0.0,  0.0,-40.0};

enum { CAN_max_TP = 2 };
enum                                                   {    tpCell1,     tpCell2}; // fixed pointer to array 
const uint16_t requestID   [CAN_max_TP]      PROGMEM = {      0x79b,       0x79b}; // Lithium Battery Controller,LBC
const uint16_t responseID  [CAN_max_TP]      PROGMEM = {      0x7bb,       0x7bb}; // Lithium Battery Controller,LBC
const uint8_t  requestMODE [CAN_max_TP]      PROGMEM = {       0x21,        0x21};
const uint8_t  requestPID  [CAN_max_TP]      PROGMEM = {       0x41,        0x42};
enum  responseSIZE                                     { SzCell1=124, SzCell2=68}; // Size of the response in used Byte
	
#endif