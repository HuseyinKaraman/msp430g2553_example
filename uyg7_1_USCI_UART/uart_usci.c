#include "io430.h"
#include "uart_usci.h"
#include "lcd_4bit.h"





unsigned char rx_buffer;



void uart_init(void)
 {
 
    P1SEL  |= BIT1 + BIT2;    // P1.1 = RXD , P1.2 = TXD
    P1SEL2 |= BIT1 + BIT2;    // P1.1 = RXD , P1.2 = TXD
 
    UCA0CTL1 |= UCSWRST;          // Enabled. USCI logic held in reset state
    UCA0CTL1 |= UCSSEL_2;        // SMCLK
    UCA0BR0  = 104;           // 1MHz 9600     sayi 104 oldugu için 8 bitlik ilk yere yazildi
    UCA0BR1  = 0;            // 1MHz 9600      eger sayi 255 den büyük ise devami sonraki 8 bitlik yere yazilir.
    UCA0MCTL = UCBRS_1;         // Buda tabloya göre yazildi 1MHz'de 9600 Baund rate için.
    UCA0CTL1 &= ~UCSWRST;       // Disabled. USCI reset released for operation.
    
    /* Interrupt Enable 2  */
    IE2 |= UCA0RXIE;                // USCI_A0 receive interrupt enable
 
 
 }


void uart_putch(unsigned char temp)     // UART üzerinden veri gönderme olayi burda olmakta.
 {
 
   while(!(IFG2 & UCA0TXIFG)); /* IFG2 => Interrupt Flag 2  */  // USCI_A0 transmit interrupt flag. UCA0TXIFG is set when UCA0TXBUF is empty.
  // Buffer müsaitse ona veri aktarabilecegiz, Bos olup olmadigini kontrol ettik. 
   UCA0TXBUF = temp; // Buffer'a yeni veri girdik.
 
 }

void uart_puts(char* buffer)
 {
   while(*buffer)   // Sonlandirma karakterini görene kadar çalisir. 
    {
         while(!(IFG2&UCA0TXIFG));      // USCI_A0 TX buffer ready?
         UCA0TXBUF = (*buffer++);
     // uart_putch i burda cagirabilirdik.
    }
 
 
 }


    
#pragma vector=USCIAB0RX_VECTOR       /* 0xFFEE USCI A0/B0 Receive */
__interrupt void USCI0RX_ISR(void)   // UART'dan veri gelme olayi burda olmakta.Veri gelince interrupt aktif ve buraya dallaniyor.
 {  
   
   rx_buffer = UCA0RXBUF;            // Buffer daki veri rx_buffer'a aktarildi.
   lcd_putch(rx_buffer); 
   

  if(rx_buffer== '0')               // Eger sifir girersem Lcd'yi temizle.
   {
    lcdClear();
    lcd_goto(1,1);
    lcd_puts("Girilen karakter:");
    lcd_goto(2,1);
   } 
 }