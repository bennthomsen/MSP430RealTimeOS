#ifndef MSP430G2553
#define MSP430G2553
#include <msp430g2553.h>
#endif

// define ADC channels
#define ADC_TEMP INCH_10
#define ADC_A1 INCH_5
#define ADC_A2 INCH_4

// define ADC input pins
#define ADC_PIN_A1 BIT5      //Define pin P1.5 as an ADC input
#define ADC_PIN_A2 BIT4      //Define pin P1.4 as an ADC Ref Out

// define ADC channel flags. These are used to determine which channel was read.
#define TEMPERATURE 1
#define A1 2
#define A2 3
#define DONE 8               // bit 8 in the flag is used to indicate that the acquistion is complete.


volatile int ADCvalue;
volatile char ADCflag;

void ADCConfigure(void);
void ADCAcquireTemp(void);
void ADCAcquireA1(void);
void ADCAcquireA2(void);
