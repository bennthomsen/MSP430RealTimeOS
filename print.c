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

static void xtoa(unsigned long x, const unsigned long *dp)
{
    char c;
    unsigned long d;
    if(x) {
        while(x < *dp) ++dp;
        do {
            d = *dp++;
            c = '0';
            while(x >= d) ++c, x -= d;
            putcharacter(c);
        } while(!(d & 1));
    } else
        putcharacter('0');
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
    
    va_list a;
    va_start(a, format);
    while(formatChar = *format++) {
        if(formatChar == '%') {
            switch(formatChar = *format++) {
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
                    xtoa((unsigned)i, dv + 5);
                    break;
                case 'l':                       // 32 bit Long
                case 'n':                       // 32 bit uNsigned loNg
                    n = va_arg(a, long);
                    if(formatChar == 'l' &&  n < 0) n = -n, putcharacter('-');
                    xtoa((unsigned long)n, dv);
                    break;
                case 'x':                       // 16 bit heXadecimal
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

void printline(char* string)
{
  putstring(string);
  putstring("\r\n");
}

void putstring(char *s)
{
  while(*s) putcharacter(*s++);
}

/**
 * Sends a single byte out through UART
 **/
void putcharacter(unsigned character)
{
  while (!(IFG2&UCA0TXIFG));		// USCI_A0 TX buffer ready?
  UCA0TXBUF = character;			// TX -> RXed character
}
