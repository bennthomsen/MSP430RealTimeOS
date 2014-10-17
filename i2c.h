#ifndef MSP430G2553
#define MSP430G2553
#include <msp430g2553.h>
#endif

void i2cConfigure(void);
void i2cTx(unsigned char address, unsigned char *tx_data, unsigned char tx_count);
void i2cRx(unsigned char address, unsigned char rx_count);
