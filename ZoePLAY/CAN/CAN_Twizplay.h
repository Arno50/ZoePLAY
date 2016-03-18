/*
 * Copyright (c) 2016 by Arno Hafner (Austria)
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */


#ifndef CAN_TWIZPLAY_H
#define CAN_TWIZPLAY_H


class CAN_Twizplay
{
	public:
    void init               (void);
    void init_TP            (uint8_t CAN_num_TP);
    void Can_rx             (void);
    void Can_start_TP       (uint8_t CAN_num_TP);
    void Can_FlowControl_TP (uint8_t CAN_num_TP);
	void store              (uint8_t* TP_stream, uint8_t StreamSIZE);
	
	float CAN_chanal[CAN_max_Chan];
    uint8_t TP_stream1[SzCell1];
    uint8_t TP_stream2[SzCell2];
    uint8_t TP_HEX1[8];
    uint8_t TP_HEX2[8];
    uint8_t TP_HEX3[8];
	uint8_t TP_count;
	uint8_t send_count;
	uint8_t Int_count;
	uint8_t stream_count;
	uint8_t TP_status;
	uint8_t Can_rx_data[CAN_max_MOB * 8]; 
	uint8_t CAN_OFF_Count;
	
	
    private:
    void init_Matrix  (void);


};
#endif