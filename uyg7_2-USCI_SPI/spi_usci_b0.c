#include "io430.h"
#include "spi_usci_b0.h"


unsigned char spi_receive = 0;

void spi_init(void)
 {
    /* USCI_Bx Control Register 1 */
    UCB0CTL1  = UCSWRST;          // Enabled. USCI logic held in reset state
    /* USCI_Bx Control Register 0 */
    UCB0CTL0  |= UCMST  /*+ UCCKPL */ + UCMSB + UCSYNC +UCMODE_0;   
   // Master mode, Clock polarity select, MSB first, Synchronous mode, Sync. Mode: USCI Mode: 0 => 3-pin SPI
                                                // synhronous mod spi,i2c olur , asynchronous da ise uart
    
    UCB0CTL1  |= UCSSEL_2;       // USCI clock source select. SMCLK
   /* USCI B0 Baud Rate 0  */
    UCB0BR0   = 0x10; // Bit clock prescaler setting.  SMCLK/2
  /* USCI B0 Baud Rate 1  */
    UCB0BR1   = 0;   // The 16-bit value of (UCxxBR0 + UCxxBR1 × 256) forms the prescaler value.
    
    
    P1SEL  =  BIT5 + BIT6 + BIT7;      // P1.5 = UCB0CLK, P1.6 = UCB0SOMI, P1.7=UCB0SIMO
    P1SEL2 |= BIT5 + BIT6 + BIT7;      // P1.5 = UCB0CLK, P1.6 = UCB0SOMI, P1.7=UCB0SIMO
                                       // Pin seçimi için Datasheet'e bak.    
    
    // IE2 |= UCB0RXIE;         // USCI_B0 receive interrupt enable
    UCB0CTL1 &= ~UCSWRST;       // Disabled. USCI reset released for operation.
 }


void spi_putch(unsigned char veri)     // Veri gönderme olayi burda olmakta.
 {  // IFG2, Interrupt Flag Register 2
   while(!(IFG2&UCB0TXIFG)); // UCB0TXIFG is set when UCB0TXBUF is empty.
     UCB0TXBUF = veri;
 }

void spi_puts(const char* veri)      // Çoklu veri gönderme burda olur.
 {   
   while(*veri)
     spi_putch(*veri++);
 }


// Spi Receive modunda LPM4'ü kullanabiliriz.  
#pragma vector=USCIAB0RX_VECTOR       /* 0xFFEE USCI A0/B0 Receive */
__interrupt void USCI_B0_SPI_RX_ISR(void)   // 
 {  
    if(IFG2 & UCB0RXIFG) // USCI_B0 receive interrupt flag
     {
       spi_receive = UCB0RXBUF;
    
     }
 }