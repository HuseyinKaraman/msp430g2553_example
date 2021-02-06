/*
    ****** interrupt uygulamasi yapilacak ******** 
*/

#include "io430.h"


void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
 // watchdog timeri durduran kod
  
  // clck degeri : bu 2 satiri yapmasakta b�yle baslayacaktir.
  BCSCTL1=CALBC1_1MHZ;   /* Basic Clock System Control 1  => BCSCTL1 Calibration Data for 1MHz  */
 
  DCOCTL=CALDCO_1MHZ;    /* DCO Clock Frequency Control => DCOCTL  Calibration Data for 1MHz  */
  
  __delay_cycles(100000); // oturma s�resi
  // yazmasakta islmeci 1 mhzde �alismaya baslamaktadir 
  
 P1DIR = BIT6;  // p1.6 �ikis yaptik digerleri giris oldu.
 P1REN = BIT3; // ellemeden deneyelim.denedim p1.3 e pull- up bagli.
 P1OUT = BIT3;
 
 P1IE = BIT3;  // p1.3 i�in interrupt i a�tik.
 P1IES &= ~BIT3; // p1.3 i�in => Bit = 0: The PxIFGx flag is set with a low-to-high transition
 P1IFG = 0x00;  // a�ilista kesmeye gitmesin diye sifirladik.
 
 
 __bis_SR_register(LPM4_bits+ GIE); // go to definitions . a�iklamalari mevcut.d�s�k g�ce alma modu +GIE a�ma
  /* Enter Low Power Mode 4 */  // SCG1+SCG0+OSCOFF+CPUOFF => LPM4_bits
 // lpm4 moduna gidiyoruz ve genel kesmeleri a�tik..... 
  
 
}

#pragma vector=PORT1_VECTOR       // /* 0xFFE4 Port 1 */  Bu adrese dallanacak. port kesmeleri i�in sabit.
__interrupt void p1_isr (void)   // kesme olunca burdaki islemler yapilacak: p1_isr  biz koyduk adini  ve degisebilir..
 {
    
    P1OUT ^=BIT6; // XOR ' LADIK.
    P1IFG= 0x00;     // Interrupt flagini temizledik. Portlar i�in b�yle.Diger �evre birimleri b�yle degil.
   
 }