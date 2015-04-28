/*
  pins_arduino.c - pin definitions for the Arduino board
  Part of Arduino / Wiring Lite

  Copyright (c) 2005 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: pins_arduino.c 565 2009-03-25 10:50:00Z dmellis $

  Modified 28-08-2009 for attiny84 R.Wiersma
  Modified 09-10-2009 for attiny45 A.Saporetti
  Modified 25-04-2015 for attiny841 D.F.Mac.
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

// ATMEL ATTINY841 / ARDUINO
//
//                           +-\/-+
//                     VCC  1|    |14  GND
//             (D 10)  PB0  2|    |13  AREF (D  0)
//  INT0       (D  9)  PB1  3|    |12  PA1  (D  1)
//                     PB3  4|    |11  PA2  (D  2)
//             (D  8)  PB2  5|    |10  PA3  (D  3)
//  SS         (D  7)  PA7  6|    |9   PA4  (D  4) SCL/SCK
//  MOSI/SDA   (D  6)  PA6  7|    |8   PA5  (D  5) MISO
//                           +----+

const static uint8_t A0 = 0;
const static uint8_t A1 = 1;
const static uint8_t A2 = 2;
const static uint8_t A3 = 3;
const static uint8_t A4 = 4;
const static uint8_t A5 = 5;
const static uint8_t A6 = 6;
const static uint8_t A7 = 7;

#define digitalPinToPCICR(p)    ( ((p) >= 0 && (p) <= 10) ? (&GIMSK) : ((uint8_t *)0) )
#define digitalPinToPCICRbit(p) ( ((p) <= 7) ? PCIE0 : PCIE1 )
#define digitalPinToPCMSK(p)    ( ((p) <= 7) ? (&PCMSK0) : (((p) <= 10) ? (&PCMSK1) : ((uint8_t *)0)) )
#define digitalPinToPCMSKbit(p) ( ((p) <= 7) ? (p) : (10 - (p)) )

#ifdef ARDUINO_MAIN

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] = 
{
  NOT_A_PORT,
  (uint16_t)&DDRA,
  (uint16_t)&DDRB,
};

const uint16_t PROGMEM port_to_output_PGM[] = 
{
  NOT_A_PORT,
  (uint16_t)&PORTA,
  (uint16_t)&PORTB,
};

const uint16_t PROGMEM port_to_input_PGM[] = 
{
  NOT_A_PORT,
  (uint16_t)&PINA,
  (uint16_t)&PINB,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = 
{
  PA, /* 0 */
  PA,
  PA,
  PA,
  PA,
  PA,
  PA,
  PA,
  PB, /* 8 */
  PB,
  PB,
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = 
{
  _BV(0), /* port A */
  _BV(1),
  _BV(2),
  _BV(3),
  _BV(4),
  _BV(5), 
  _BV(6),
  _BV(7),
  _BV(2), /* port B */
  _BV(1),
  _BV(0),
};


const uint8_t PROGMEM digital_pin_to_timer_PGM[] = 
{
  NOT_ON_TIMER,   // PA0 : 
  NOT_ON_TIMER,   // PA1 : TOCC0 : TXD0
  NOT_ON_TIMER,   // PA2 : TOCC1 : RXD0
  TIMER2B,        // PA3 : TOCC2 : 
  TIMER2A,        // PA4 : TOCC3 : SCK  / SCL
  TIMER1B,        // PA5 : TOCC4 : MISO
  TIMER1A,        // PA6 : TOCC5 : MOSI / SDA
  TIMER0B,        // PA7 : TOCC6 : SS
  TIMER0A,        // PB2 : TOCC7 
  NOT_ON_TIMER,   // PB1 :
  NOT_ON_TIMER,   // PB0 :
};

/*
TOCPMCOE = (1<<TOCC7OE)|(1<<TOCC6OE)|(1<<TOCC5OE)|(1<<TOCC4OE)|(1<<TOCC3OE)|(1<<TOCC2OE);
TOCPMSA1 = (1<<TOCC5S0)|(1<<TOCC4S0);
TOCPMSA0 = (1<<TOCC3S1)|(1<<TOCC2S1);
*/


#endif

#endif
