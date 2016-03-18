/**
 * \file
 *
 * \brief Common IOPORT service demo configuration header file.
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#ifndef CONF_TWIZPLAY_H_INCLUDED
#define CONF_TWIZPLAY_H_INCLUDED

#define Backlgt_dir DDRB
#define Backlgt PORTB 
#define Backlgt_pin    7

#define DOGM_CS1_dir DDRC
#define DOGM_CS1 PORTC
#define DOGM_CS1_pin 4

#define DOGM_A0_dir DDRC
#define DOGM_A0 PORTC
#define DOGM_A0_pin 2

#define DOGM_SCLK_dir DDRC
#define DOGM_SCLK PORTC
#define DOGM_SCLK_pin 1

#define DOGM_SI_dir DDRC
#define DOGM_SI PORTC
#define DOGM_SI_pin 3

#define DOGM_Reset_dir DDRC
#define DOGM_Reset PORTC
#define DOGM_Reset_pin 0

#define Taster1_dir DDRC
#define Taster1 PINC
#define Taster1_pin 5

#define Taster2_dir DDRC
#define Taster2 PINC
#define Taster2_pin 6

#define Taster3_dir DDRC
#define Taster3 PINC
#define Taster3_pin 7

#define CAN_rx_DDR DDRD
#define CAN_rx_port PIND
#define CAN_rx_pin 6

#define CAN_tx_DDR DDRD
#define CAN_tx_port PORTD
#define CAN_tx_pin 5

#endif /* CONF_TWIZPLAY_H_INCLUDED */
