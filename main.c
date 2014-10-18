#include "RealTimeClock.h"
#include "UART.h"
#include "ADC.h"
#include "print.h"
#include "LCDDisplay.h"
#include "i2c.h"


/* Main function */

#define EVENTSTART0 40
#define EVENTDELAY0 40
#define EVENTSTART1 2000
#define EVENTDELAY1 1000

#define EVENTSTART2 2000
#define EVENTDELAY2 1000

static unsigned int event0 = EVENTSTART0;
static unsigned int event1 = EVENTSTART1;
static unsigned int event2 = EVENTSTART2;

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
        
        
        if (time_ms == event0)
        {
            event0=time_ms+EVENTDELAY0;
            
        }
        
        if (time_ms == event1)
        {
            LCDHome();
            LCDWriteString("Hello", 5);
            
        }
        
        if (time_ms == event2)
        {
            event2=time_ms+EVENTDELAY2;
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