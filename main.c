#include "RealTimeClock.h"
#include "UART.h"
#include "ADC.h"
#include "LCDDisplay.h"
#include "print.h"
#include "i2c.h"


/* Main function */

#define EVENTSTART0 1000UL
#define EVENTDELAY0 1000UL

#define EVENTSTART1 5000UL
#define EVENTDELAY1 1000UL

#define EVENTSTART2 5000UL
#define EVENTDELAY2 2000UL

static unsigned long event0 = EVENTSTART0;
static unsigned long event1 = EVENTSTART1;
static unsigned long event2 = EVENTSTART2;
static unsigned long event3 = 4500;

char cursorState = 0;

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop WDT
    BCSCTL1 = CALBC1_1MHZ; // Set DCO to 1MHz
    DCOCTL = CALDCO_1MHZ; // Set DCO to 1MHz
    
    P1DIR |= (BIT0);              // P1.2 to output
    P1OUT &= ~(BIT0);              // Set the LEDs P1.0 and P1.6 to on
    
    UARTConfigure();
    ADCConfigure();
    LCDConfigure();
    LCDBackLight(50);
    LCDBlinkOff();
    LCDCursorOff();
    RTCConfigure();
    
    while(1)
    {
        // Service Switch Presses
        if (pressRelease1)
        {
            switch (pressRelease1) {
                case S1:
                    LCDBack();
                    break;
                case SW2:
                    LCDHome();
                    LCDWriteString("S0");
                    break;
            }
            pressRelease1 = 0;
        }
        
        if (pressRelease2) {
            switch (pressRelease2) {
                case S2:
                    LCDDown();
                    break;
                case S3:
                    LCDForward();
                    break;
                case S4:
                    LCDUp();
                    break;
                case S5:
                    if (++cursorState == 4) cursorState = 0;   // Increment state and wrap if exceeds laat state
                    switch (cursorState) {
                        case 0:
                            LCDBlinkOff();
                            break;
                        case 1:
                            LCDCursorOn();
                            break;
                        case 2:
                            LCDBlinkOn();
                            break;
                        case 3:
                            LCDCursorOff();
                            break;
                    }
                    break;
            }
            pressRelease2 = 0;
        }
        
        //Service timed events
        if (time_ms >= event0)
        {
            event0 = MAXTIME_MS;             // Do not repeat event
            LCDHome();
            LCDWriteString("Real Time OS ");
            
            LCDSetLocation(1,0);
            LCDWriteString("Time & ADC Demo");
        }
        
        if (time_ms >= event3)
        {
            event3=MAXTIME_MS;               // Do not repeat event
            LCDClear();
        }
        
        if (time_ms >= event1)
        {
            event1=time_ms+EVENTDELAY1;      // Schedule repeat event
            printDestination = LCD;
            LCDSetLocation(0,0);
            printformat("Time: %2i:%2i:%2i", hours, minutes, seconds);
        }
        
        if (time_ms >= event2)
        {
            event2=time_ms+EVENTDELAY2;      // Schedule repeat event
            UARTPrintln("Reading ADC");      // Send string to UART
            ADCAcquireTemp();
        }
        
        // Service ADC flag
        if (ADCflag == 1)
        {
            ADCflag = 0;                     // Reset ADC flag to indicate that the event has been serviced
            printDestination = UART;
            printformat("Temp: %i C\r\n",((ADCvalue * 27069L - 18169625L) >> 16));  // Send formatted ADC reading to UART
            printDestination = LCD;
            LCDSetLocation(1,0);
            printformat("Temp: %i C",((ADCvalue * 27069L - 18169625L) >> 16)); // Send formatted ADC reading to LCD Display
            
        }
        
        __bis_SR_register(CPUOFF + GIE); // LPM0 with interrupts enabled
    }
}