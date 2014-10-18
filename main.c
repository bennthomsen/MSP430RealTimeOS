#include "RealTimeClock.h"
#include "UART.h"
#include "ADC.h"
#include "print.h"
#include "LCDDisplay.h"
#include "i2c.h"


/* Main function */

#define EVENTSTART0 1000UL
#define EVENTDELAY0 1000UL
#define EVENTSTART1 2000UL
#define EVENTDELAY1 1000UL

#define EVENTSTART2 2000UL
#define EVENTDELAY2 1000UL

static unsigned long event0 = EVENTSTART0;
static unsigned long event1 = EVENTSTART1;
static unsigned long event2 = EVENTSTART2;

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop WDT
    BCSCTL1 = CALBC1_1MHZ; // Set DCO to 1MHz
    DCOCTL = CALDCO_1MHZ; // Set DCO to 1MHz
    
    P1DIR |= (BIT0);              // P1.2 to output
    P1OUT &= ~(BIT0);              // Set the LEDs P1.0 and P1.6 to on
    
    RTCConfigure();
    UARTConfigure();
    ADCConfigure();
    LCDConfigure(0x3E);
    

    
    
    
    while(1)
    {
        // Service Switch Presses
        if (pressRelease1)
        {
            switch (pressRelease1) {
                case S1:
                    LCDHome();
                    LCDWriteString("S1", 2);
                    break;
                case SW2:
                    LCDHome();
                    LCDWriteString("S0", 2);
                    break;
            }
            pressRelease1 = 0;
        }
        
        if (pressRelease2)
        {
            switch (pressRelease2) {
                case S2:
                    LCDHome();
                    LCDWriteString("S2", 2);
                    break;
                case S3:
                    LCDHome();
                    LCDWriteString("S3", 2);
                    break;
                case S4:
                    LCDHome();
                    LCDWriteString("S4", 2);
                    break;
                case S5:
                    LCDHome();
                    LCDWriteString("S5", 2);
                    break;
            }
            pressRelease2 = 0;
        }
        
        //Service timed events
        if (time_ms == event0)
        {
            LCDHome();
            LCDWriteString("Hello", 5);
        }
        
        if (time_ms == event1)
        {
            event1=time_ms+EVENTDELAY1;      // Schedule repeat event
            LCDHome();
            LCDWriteString("Update", 6);
        }
        
        if (time_ms == event2)
        {
            event2=time_ms+EVENTDELAY2;      // Schedule repeat event
            UARTPrintln("test");
            printline("Reading ADC");
            ADCAcquireTemp();
        }
        
        // Service ADC flag
        if (ADCflag == 1)
        {
            ADCflag = 0;
            printformat("Temp: %i C\r\n",((ADCvalue * 27069L - 18169625L) >> 16));
        }
        
        __bis_SR_register(CPUOFF + GIE); // LPM0 with interrupts enabled
    }
}