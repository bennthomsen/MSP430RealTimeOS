#include "LCDShield.h"
#include "i2c.h"



void LCDShieldConfigure(unsigned char address)
{
    
    const unsigned char TxData[] =              // Table of data to transmit
    { 0x00,
        0x38,
        0x00,
        0x39,
        0x14,
        0x70,
        0x54,
        0x6F,
        0x0F, //Display On, Cursor On, Cursor Blink On
        0x01
    };
    
    i2cConfigure();
    
    i2cTx(address,TxData,10);
    
    /* Configure timer A as a pulse width modulator */
    TA1CCR0 = 1000-1;             // Set maximum count value to determine PWM Frequency = SMCLK/TACCR0 (1MHz/1000 = 1kHz)
    TA1CCR2 = 999;                // Initialise counter compare value 1 to control Duty Cycle = TACCR1/TACCR0 (500/1000 = 50%)
    TA1CCTL2 = OUTMOD_7;          // Set output to on when counter resets and off when counter equals TACCR1. Normal PWM.
    TA1CTL = TASSEL_2 + MC_1;     // Use the SMCLK to clock the counter and set to count up mode
    
    P2DIR |= BIT4;               // Set P1.6 to output
    P2SEL |= BIT4;               // Select output P2.4 to be TA1.2
    P2SEL2 &= ~BIT4;             // Select output P2.4 to be TA1.2
}

