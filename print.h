#ifndef MSP430G2553
#define MSP430G2553
#include <msp430g2553.h>
#endif
#include "stdarg.h"

#define UART 0
#define LCD 1

volatile char printDestination;

void printformat(char *format, ...);
