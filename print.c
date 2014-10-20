#include "print.h"


void putcharacter(unsigned character);
void putstring(char *s);

static const unsigned long dv[] = {
//  4294967296      // 32 bit unsigned max
    1000000000,     // +0
     100000000,     // +1
      10000000,     // +2
       1000000,     // +3
        100000,     // +4
//       65535      // 16 bit unsigned max     
         10000,     // +5
          1000,     // +6
           100,     // +7
            10,     // +8
             1,     // +9
};

static const int dw[] = {      // Width of integer
    //  4294967296      // 32 bit unsigned max
    10,     // +0
    9,     // +1
    8,     // +2
    7,     // +3
    6,     // +4
    //       65535      // 16 bit unsigned max
    5,     // +5
    4,     // +6
    3,     // +7
    2,     // +8
    1,     // +9
};

static void xtoa(unsigned long x, int width, const int *wp, const unsigned long *dp)
{
    char c;
    unsigned long d;
    
    if(x) {
        while(x < *dp) {
            ++dp;
            if (width >= *wp) putcharacter('0');
            ++wp;
        }
        do {
            d = *dp++;
            c = '0';
            while(x >= d) ++c, x -= d;
            putcharacter(c);
        } while(!(d & 1));
    }
    else {
        while (width--) putcharacter('0');
    }
}

static void puth(unsigned n)
{
    static const char hex[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    putcharacter(hex[n & 15]);
}
 
void printformat(char *format, ...)
{
    char formatChar;
    int i;
    long n;
    int width = 1;
    
    va_list a;
    va_start(a, format);
    while(formatChar = *format++) {
        if(formatChar == '%') {
            formatChar = *format++;
            if ((formatChar > 48) && (formatChar < 58) ) {
                width = formatChar-48;
                formatChar = *format++;
            }
            switch(formatChar) {
                case 's':                       // String
                    putstring(va_arg(a, char*));
                    break;
                case 'c':                       // Char
                    putcharacter(va_arg(a, unsigned));
                    break;
                case 'i':                       // 16 bit Integer
                case 'u':                       // 16 bit Unsigned
                    i = va_arg(a, int);
                    if(formatChar == 'i' && i < 0) i = -i, putcharacter('-');
                    xtoa((unsigned)i, width, dw + 5, dv + 5);  // Send 16 bit integer to xtoa and point to +5 element of dv
                    break;
                case 'l':                       // 32 bit Long
                case 'n':                       // 32 bit unsigned long
                    n = va_arg(a, long);
                    if(formatChar == 'l' &&  n < 0) n = -n, putcharacter('-');
                    xtoa((unsigned long)n, width, dw, dv);
                    break;
                case 'x':                       // 16 bit hexadecimal
                    i = va_arg(a, int);
                    puth(i >> 12);
                    puth(i >> 8);
                    puth(i >> 4);
                    puth(i);
                    break;
                case 0: return;
                default: goto bad_fmt;
            }
        } else
            bad_fmt:    putcharacter(formatChar);
    }
    va_end(a);
}

void putstring(char *s)
{
  while(*s) putcharacter(*s++);
}

/**
 * Sends a single byte out through UART or to LCD
 **/
void putcharacter(unsigned character)
{
    if (printDestination)
    {
        LCDWriteChar(character);
    }
    else
    {
        while (!(IFG2 & UCA0TXIFG));		// USCI_A0 TX buffer ready?
        UCA0TXBUF = character;			// TX -> RXed character
    }
}
