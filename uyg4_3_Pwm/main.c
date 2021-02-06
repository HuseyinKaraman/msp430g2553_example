
#include "io430.h"

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;
  __delay_cycles(100000);
  
  P1SEL |=BIT6;   // TA0.1'den PWM Cikisi // Timer0_A, compare: Out1 output // burdan baktim p1sel'e =>datasheet =>Table 16. Port P1 (P1.0 to P1.2) Pin Functions
  P1DIR |=BIT6;   // 6. bite led bagli. 
  
  
//   P1SEL |=BIT1;   /* Kendi eklemem.... */
//   P1DIR |=BIT1;  // p1.1 'i çikis yapalim./* Kendi eklemem.... */
   
   
  TA0CTL = TASSEL_2 + MC_0; 
  /* Timer A clock source select: 2 - SMCLK */
  /* Timer A mode control: 0 - Stop */
  
  TA0CCR0 = 100-1;    // Periyodu belirledik.
  TA0CCR1 = 90;   // Duty Cycle = %20  -1'i output modda yok 
  
  /* Timer A Capture/Compare Control 1 */
   TA0CCTL1 = OUTMOD_7; // P1.6 Reset/Set Mode // Cikis için TA0.1 lazim. /* PWM output mode: 3 - PWM set/reset */
   // sadece TA0CCTL1'den Cikis alinacaktir.
   /* mode 3 ve mode 7 birbirinin tersi . Birinde % 60 lik doluluk orani üretirsek , digerinde % 40 lik doluluk orani elde ederiz. */
   
 //  TA0CCTL0 =OUTMOD_3; /* Kendi eklemem.... */ // bu led2'nin tam  tersi çikis verecek 
   
  
   TA0CTL |= MC_1;  // timeri Çalistiriyoruz.
  /* Timer A mode control: 1 - Up to CCR0 */
  
  __bis_SR_register(LPM0_bits + GIE); // LPM1 de ' de calisir.
  
 
}
