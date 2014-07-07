#ifndef MSP430G2553
#define MSP430G2553
#include <msp430g2553.h>
#endif

volatile int ADCvalue;
volatile char ADCflag;

void ADCConfigure(void);
void ADCAcquireTemp(void);
