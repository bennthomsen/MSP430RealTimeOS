#include "ADC.h"

void ADCConfigure(void)
{
 /* Configure ADC*/
    //ADC10CTL0=SREF_0+ REF2_5V +REFON + ADC10ON + ADC10SHT_3 + ADC10IE ; //  + REFON  1.5V ref,Ref on,64 clocks for sample
    ADC10CTL0=SREF_1 + REFON + REFOUT + ADC10ON + ADC10SHT_3 + ADC10IE ; //1.5V ref,Ref on,64 clocks for sample
    ADC10CTL1=ADC10DIV_3; //temp sensor is at 10 and clock/4
    ADC10AE0 |= ADC_PIN_A1 + ADC_PIN_A2; // Set P1.4 and P1.5 as ADC inputs
    ADCflag = 0;
}

void ADCAcquireTemp(void)
{
    ADC10CTL0 |= SREF_1 + REFON;         // Vcc ref
    ADC10CTL1 = INCH_10;            // Select Channel
    ADC10CTL0 |= ENC + ADC10SC;    // Start Acquisition
    ADCflag = TEMPERATURE;
}

void ADCAcquireA1(void)
{
    ADC10CTL1=ADC_A1;            // Select Channel
    ADC10CTL0 |= ENC + ADC10SC;  // Start Acquisition
    ADCflag = A1;
}

void ADCAcquireA2(void)
{
    ADC10CTL1=ADC_A2;            // Select Channel
    ADC10CTL0 |= ENC + ADC10SC;  // Start Acquisition
    ADCflag = A2;
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
  ADCvalue = ADC10MEM; // read ADC value (note this is a 10bit value stored in a 16 bit register)
  ADC10CTL0 &= ~ENC;
  ADCflag |= DONE;
 __bic_SR_register_on_exit(CPUOFF); // Return to active mode
}

