/*
  RealTimeClock.c
  MSP430RealTimeOS
 
 Uses TimerA0 to produce an interrupt every millisecond
 

  Created by Benn Thomsen on 18/10/2014.
  Copyright (c) 2014 Benn Thomsen. All rights reserved.
*/

#include "RealTimeClock.h"

#define INCR 1UL

void RTCConfigure(void)
{
    time_ms = 0;
    ms = 0;
    seconds = 0;
    minutes = 0;
    hours = 0;
    /* Configure timer A as a millisecond interval counter */
    TA0CCR0 = 1000-1;             // Set maximum count value to determine count frequency = SMCLK/TACCR0 (1MHz/1000 = 1kHz)
    TA0CCTL0 = OUTMOD_0 + CCIE;   // Set out mode 0, enable CCR0 interrupt
    TA0CTL = TASSEL_2 + MC_1;     // SMCLK as input clock source and set to count up mode to TA0CCR0
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0_ISR (void)
{
    time_ms = time_ms + INCR;
    if (++ms == 1000)
    {
        ms = 0;
       //P1OUT ^= BIT0;            // Toggle LED1 on P1.0 every second (Used for testing)
        if (++seconds == 60)
        {
            seconds = 0;
            if (++minutes == 60)
            {
                minutes = 0;
                if (++hours == 24)
                {
                    hours = 0;
                }
            }
        }
    }
    __bic_SR_register_on_exit(CPUOFF);  /* Exit Low Power Mode 0 */
}