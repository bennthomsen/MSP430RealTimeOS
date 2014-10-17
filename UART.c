#include "UART.h"

void UARTConfigure(void){
  /* Configure hardware UART */
  P1SEL = BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD
  P1SEL2 = BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD
  UCA0CTL1 |= UCSSEL_2; // Use SMCLK
  UCA0BR0 = 104; // Set baud rate to 9600 with 1MHz clock (Data Sheet 15.3.13)
  UCA0BR1 = 0; // Set baud rate to 9600 with 1MHz clock
  UCA0MCTL = UCBRS0; // Modulation UCBRSx = 1
  UCA0CTL1 &= ~UCSWRST; // Initialize USCI state machine
  IE2 |= UCA0RXIE; // Enable USCI_A0 RX interrupt
}

void UARTSendArray(unsigned char *TxArray, unsigned char ArrayLength){
  // Send number of bytes Specified in ArrayLength in the array at using the hardware UART 0
  // Example usage: UARTSendArray("Hello", 5);
  // int data[2]={1023, 235};
  // UARTSendArray(data, 4); // Note because the UART transmits bytes it is necessary to send two bytes for each integer hence the data length is twice the array length

    while(ArrayLength--){ // Loop until StringLength == 0 and post decrement
    while(!(IFG2 & UCA0TXIFG)); // Wait for TX buffer to be ready for new data
    UCA0TXBUF = *TxArray++; //Write the character at the location specified by the pointer and increment pointer
  }
}
