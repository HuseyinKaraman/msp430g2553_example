/*
    ****** interrupt uygulamasi yapilacak ******** 
*/

#include "io430.h"


void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
 // watchdog timeri durduran kod
  
  // clck degeri : bu 2 satiri yapmasakta böyle baslayacaktir.
  BCSCTL1=CALBC1_1MHZ;   /* Basic Clock System Control 1  => BCSCTL1 Calibration Data for 1MHz  */
 
  DCOCTL=CALDCO_1MHZ;    /* DCO Clock Frequency Control => DCOCTL  Calibration Data for 1MHz  */
  
  __delay_cycles(100000); // oturma süresi
  // yazmasakta islmeci 1 mhzde çalismaya baslamaktadir 
  
 P1DIR = BIT6;  // p1.6 çikis yaptik digerleri giris oldu.
 P1REN = BIT3; // ellemeden deneyelim.denedim p1.3 e pull- up bagli.
 P1OUT = BIT3;
 
 P1IE = BIT3;  // p1.3 için interrupt i açtik.
 P1IES &= ~BIT3; // p1.3 için => Bit = 0: The PxIFGx flag is set with a low-to-high transition
 P1IFG = 0x00;  // açilista kesmeye gitmesin diye sifirladik.
 
 
 __bis_SR_register(LPM4_bits+ GIE); // go to definitions . açiklamalari mevcut.düsük güce alma modu +GIE açma
  /* Enter Low Power Mode 4 */  // SCG1+SCG0+OSCOFF+CPUOFF => LPM4_bits
 // lpm4 moduna gidiyoruz ve genel kesmeleri açtik..... 
  
 
}

#pragma vector=PORT1_VECTOR       // /* 0xFFE4 Port 1 */  Bu adrese dallanacak. port kesmeleri için sabit.
__interrupt void p1_isr (void)   // kesme olunca burdaki islemler yapilacak: p1_isr  biz koyduk adini  ve degisebilir..
 {
    
    P1OUT ^=BIT6; // XOR ' LADIK.
    P1IFG= 0x00;     // Interrupt flagini temizledik. Portlar için böyle.Diger çevre birimleri böyle degil.
   
 }