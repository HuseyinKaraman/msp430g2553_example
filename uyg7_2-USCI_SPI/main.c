#include "io430.h"
#include "spi_usci_b0.h"


#define LATCH_595(x) ( (x) ? (P1OUT |= BIT0):(P1OUT &= ~BIT0)  ) // x=1 ise çikis 1 x=0 ise çikis 0


const char* dizi = "MSP430G2553 USCI_B0 8 BIT SPI Testi";
extern unsigned char spi_receive;

void hc595_gonder(unsigned char veri);

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;
  __delay_cycles(100000);
  
  P1DIR |= BIT0;
  P1OUT = 0x00;
  
  TA0CTL   = MC_0 + TASSEL_2; /* Timer A mode control: 0 - Stop */ /* Timer A clock source select: 2 - SMCLK */ 
  TA0CCR0  = 50000 - 1;
  TA0CCTL0 = CCIE; /* Capture/compare interrupt enable  */

  spi_init();
         
  TA0CTL |= MC_3;  /* Timer A mode control: 3 - Up/Down */
  __bis_SR_register(LPM0_bits + GIE); 
}


void hc595_gonder(unsigned char veri)
 {
 while(!(IFG2&UCB0TXIFG));
    UCB0TXBUF = veri; 

 LATCH_595(1); // chip slecet sanirim....
 LATCH_595(0);  // 74hc595 e P1.0 bagli onun kilidini ac kapa ile ona veri yolluoz.

 }


#pragma vector=TIMER0_A0_VECTOR
__interrupt void ta0_isr(void)
 {
  static char i=0;
  // exp 2: 
  hc595_gonder(i++);        // Timerda frekans bölücü ile timer i yavaslat ve sayi artimini daha iyi gör.
                           // /*+ UCCKPL*/ hc kullanirken bunu kullanmiyoruz.
// exp 1:
//  if(i==2) // Veri Göndermede kullandik...      // /*+ UCCKPL */'i aktif yap . spi_init()'de. 
//   {
//     spi_puts(dizi);
//     i=0;
//   }
   
 }