/* P1.3 Giris 
   P1.0 (LED1) ÇIKIS
*/
#include "io430.h"

void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
 // watchdog timeri durduran kod
  
  BCSCTL1=CALBC1_1MHZ; // clck degeri
  DCOCTL=CALDCO_1MHZ;
  __delay_cycles(100000); // oturma süresi
  // yazmasakta islmeci 1 mhzde çalismaya baslamaktadir 
  
   
  
    P1DIR |= 0x40; // P1.6 Çikis oldu. Digerleri ayni kaldi
    P1OUT &= ~0x40;

    P1DIR |= 0x01;   // P1.0 ÇIKIS  | Digerleri ayni kaldi. LOOK IO430G2553.h  kisaltmalar mevcut.. 0x0001 = BIT0;
    P1OUT &= ~0x01;     

    P1DIR &= ~0x08;  // p1.3 Giris yaptik.
    P1REN |=  0x08; // pull-up ve ya pull-down direnci aktif ettik.
    P1OUT |= 0x08;
    // P1SEL &= ~0x08;
  
 
  
  while(1)
  {
  
    while(!(P1IN & BIT3)) // P1.3 bitini sorgulamis oluyoruz.
    {
      P1OUT |=  (BIT0);  // P1.0 ÇIKIS 1 P1.6 ÇIKIS 0 OLDU  // önce p1.0 'i yaktik . digerleri ayni.
      P1OUT &= (~BIT6); // digerleri ayni p1.6'nin çikisi 0 
    }
    
      P1OUT &= ~BIT0; //P1.0  Önce söndürdük.
      P1OUT |=  BIT6; // P1.6 sonra yaktik.
  }
  
  
  
 // return 0;
}

