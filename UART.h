#ifndef MSP430G2553
#define MSP430G2553
#include <msp430g2553.h>
#endif

char UARTRxData[20];
char UARTRxFlag;

void UARTConfigure(void);
void UARTSendChar(unsigned char character);
void UARTSendArray(unsigned char *TxArray, unsigned char ArrayLength);
void UARTSendString(char *TxArray);
void UARTPrintln(char *TxArray);
