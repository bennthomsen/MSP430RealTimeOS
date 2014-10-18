//
//  RealTimeClock.h
//  MSP430RealTimeOS
//
//  Created by Benn Thomsen on 18/10/2014.
//  Copyright (c) 2014 Benn Thomsen. All rights reserved.
//

#ifndef MSP430G2553
#define MSP430G2553
#include <msp430g2553.h>
#endif

volatile unsigned int time_ms;
volatile unsigned int ms;
volatile unsigned int seconds;
volatile unsigned int minutes;
volatile unsigned int hours;

void RTCConfigure(void);
