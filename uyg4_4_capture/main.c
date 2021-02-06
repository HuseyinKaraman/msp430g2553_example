#include "io430.h"


unsigned long freq=12345678L;

int main( void )
{
  freq=0; // sifirladik.
  
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  DCOCTL=0;
  BCSCTL1 =  CALBC1_8MHZ;  // 16 IDI 
  DCOCTL  = CALDCO_8MHZ;
 
  __delay_cycles(1000000);
 

  BCSCTL2 |=  DIVS_3;      // EKLEDIM.
  BCSCTL2 &= ~SELS ;     /* SMCLK Source Select 0:DCOCLK / 1:XT2CLK/LFXTCLK  */
  
     
     
  P1SEL  =  BIT1; //P1.1 CCI0_A için Giris   //P1.1=>  Timer0_A, capture: CCI0A input, compare: Out0 output
  P1SEL |= (BIT0 + BIT4);      // P1.0 ACLK,P1.4 SMCLK   
  /* P1.0=> ACLK signal output, P1.4=>SMCLK signal output */  
  // pin function'a bakabilirsin istenen özellikleri eklemek için P1SEL ve P1DIR 'in yönü ne olacak..
  
  P1DIR |= (BIT0 + BIT4);      // P1.0 Cikis - aclk'nin frekansini cikis olarak alicaz      // BIT 4 EKLEDIM
  P1DIR &= ~BIT1;      //P1.1 Giris
  
  TA0CCR0  = 0;      // tedbir amaçli sifirladik
   
  // TA0.CCI0A için ayarlar: 
  TA0CCTL0 = CM_1 + CCIS_0 + CAP + SCS + SCCI + CCIE; /* Timer0_A3(3 tane var) Capture/Compare Control 0  */
  // Capture/Compare Control Register => /* Capture mode: 1 - pos. edge */ 
  // + CCIS_0 => /* Capture input select: 0 - CCIxA */
  // + CAPTURE MODE 
  // + /* Capture sychronize  */
  // + /* Latched capture signal (read)  */  //dis kaynak ile iç kaynagin pulse'ini senkronize olmasini sagliyormus :)
  // + /* Capture/compare interrupt enable  */
  // (Bit 10 SCCI Synchronized capture/compare input. The selected CCI input signal is latched with the EQUx signal and can be read via this bit)
  
  TA0CTL   = TASSEL_2 + MC_2+ ID_3; // ID_3 ekledim  ve tassel_2 denedim
  // up yada cont. mod fark etmiyor etkilemeyecek capture'i.
  
  /* WDT is clocked by fACLK (assumed 32KHz) */  /* 1000ms  " */ // :
  /* IAR'da hazir macro*/
  WDTCTL =WDT_ADLY_1000;          // WDT 1 SANIYE KESME PERIYODU  //WDT_ADLY_1000  IDI
  IE1 |= WDTIE;
  
  
  __bis_SR_register(LPM0_bits + GIE);
  
}


#pragma vector=WDT_VECTOR  // Timer yerine WDT'yi kullanarak her 1sn'de gecikme urettik
__interrupt void wdt_isr(void)
 {
  freq = 0; 
 }


#pragma vector=TIMER0_A0_VECTOR
__interrupt void ta0_a0_isr(void)
 {
  freq ++; // her kesmede artacak.
 }

