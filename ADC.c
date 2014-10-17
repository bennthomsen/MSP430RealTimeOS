#include "ADC.h"

void ADCConfigure(void)
{
 /* Configure ADC*/
    ADC10CTL0=SREF_1 + REFON + ADC10ON + ADC10SHT_3 + ADC10IE ; //1.5V ref,Ref on,64 clocks for sample
    ADC10CTL1=ADC10DIV_3; //temp sensor is at 10 and clock/4
    ADCflag = 0;
}

void ADCAcquireTemp(void)
{
 /* Configure ADC Channel */
 ADC10CTL1=INCH_10;
 ADC10CTL0 |= ENC + ADC10SC;

}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
  ADCvalue = ADC10MEM; // read ADC value (note this is a 10bit value stored in a 16 bit register)
  ADC10CTL0 &= ~ENC;
  ADCflag = 1;
 __bic_SR_register_on_exit(CPUOFF); // Return to active mode
}

