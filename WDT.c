#include "WDT.h"

void WDTConfigure(void)
{
   WDTCTL = WDT_MDLY_32;             // Start and set watchdog timer (WDT) to trigger every 32ms
   IFG1 &= ~WDTIFG;                  // Clear the interrupt flag for the WDT
   IE1 |= WDTIE;                     // enable WDT interrupt
   time = 0;
}

// WDT Interrupt Service Routine increment tick count
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
   IFG1 &= ~WDTIFG;                 // clear WDT interrupt flag
   time++;
   P1IE |= button;                 // Enable Button interrupt
   __bic_SR_register_on_exit(CPUOFF); /* Exit Low Power Mode 0 */
}
