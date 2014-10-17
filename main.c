#include"WDT.h"
#include "UART.h"
#include "ADC.h"
#include "print.h"
#include "LCDShield.h"

/* Main function */

#define EVENTSTART0 5
#define EVENTDELAY0 5
#define EVENTSTART1 20
#define EVENTDELAY1 20

#define EVENTSTART2 30
#define EVENTDELAY2 60

static unsigned int event0 = EVENTSTART0;
static unsigned int event1 = EVENTSTART1;
static unsigned int event2 = EVENTSTART2;

int main(void)
{
    
    BCSCTL1 = CALBC1_1MHZ; // Set DCO to 1MHz
    DCOCTL = CALDCO_1MHZ; // Set DCO to 1MHz
    
    WDTConfigure();
    UARTConfigure();
    ADCConfigure();
    LCDShieldConfigure(0x3E);
    
    P1DIR |= (BIT0);              // P1.2 to output
    P1OUT |= (BIT0);              // Set the LEDs P1.0 and P1.6 to on
    
    while(1)
    {
        if (time == event0)
        {
            event0=time+EVENTDELAY0;
            P1OUT ^= BIT0;
        }
        
        if (time == event2)
        {
            event2=time+EVENTDELAY2;
            ADCAcquireTemp();
        }
        if (ADCflag == 1)
        {
            ADCflag = 0;
            printformat("Temp: %i C\r\n",((ADCvalue * 27069L - 18169625L) >> 16));
        }
        
        __bis_SR_register(CPUOFF + GIE); // LPM0 with interrupts enabled
    }
}