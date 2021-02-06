
#include "io430.h"

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  BCSCTL1=CALBC1_16MHZ;
  
  DCOCTL=CALDCO_16MHZ;
  
  __delay_cycles(100000);
  
   BCSCTL1 |=DIVA_0;  // ACLK /2  =>  DIVA_1 (0x10)   /* ACLK Divider 1: /2 */
   BCSCTL2 |=DIVS_1;  // SMCLK /8 =>  /* SMCLK Divider 3: /8 */  ve SMCLK /1 =>  /* SMCLK Divider 0: /1 */
   
   BCSCTL2 &= ~SELS;   //  SMCLK=>LFXT1CLK      /* SMCLK Source Select 0:DCOCLK / 1:XT2CLK/LFXTCLK  */ => bu 0 olursa smclkkaynagi dco olur
                      // BCSCTL2 |= SELS;  => 1 olursa xt2 veya lfxt yi seçebiliriz
   
   BCSCTL3 |=LFXT1S_0;   // LFXT1CLK= 32.768 kHz harici Kristal => Mode 0 for LFXT1 : Normal operation  => BUNU SEÇERSEK  ACLK IÇIN  HARICI OCS YI KULLANIRIZ
  //  BCSCTL3 |= LFXT1S_2;  /* Mode 2 for LFXT1 : VLO */ // LFXT1CLK= ~12 kHz VLO Osilatör  => bunun ile ACLK IÇIN VLO YU SEÇTIK
                
   
   
   P1DIR = BIT0 + BIT4; // go datasheet for pins => P1.0 VE P1.4 Çikis
   P1SEL = BIT0 + BIT4; // p1.0 => aclk  ve  p1.4=> smclk
   
   __bis_SR_register(CPUOFF+ GIE);
   
  
  
}
