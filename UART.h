#ifndef MSP430G2553
#define MSP430G2553
#include <msp430g2553.h>
#endif

void UARTConfigure(void);
void UARTSendArray(unsigned char *TxArray, unsigned char ArrayLength);
