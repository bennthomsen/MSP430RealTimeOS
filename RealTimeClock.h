/*
  RealTimeClock.h
  MSP430RealTimeOS
 
 The elapsed time in milliseconds since the microcontroller reset is stored in the variable time_ms
 as a 32bit unsigned integer. time_ms will overflow in approximately 49 days.

  Created by Benn Thomsen on 18/10/2014.
  Copyright (c) 2014 Benn Thomsen. All rights reserved.
*/

#ifndef MSP430G2553
#define MSP430G2553
#include <msp430g2553.h>
#endif

volatile unsigned long time_ms;
volatile unsigned int ms;
volatile unsigned int seconds;
volatile unsigned int minutes;
volatile unsigned int hours;

#define MAXTIME_MS 0xFFFFFFFF

void RTCConfigure(void);
