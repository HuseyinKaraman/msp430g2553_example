#include "io430.h"

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  BCSCTL1= CALBC1_1MHZ;
  DCOCTL= CALDCO_1MHZ;  
  __delay_cycles(100000);
  
  P1DIR = BIT0+BIT6; // Bit0 ve BIT6'nin Y�n�n� �ikis yaptik. 
  P1OUT=0x00;   // T�m �ikislari 0'ladik.
  
                       /*  Tek timer 2 tane CC Unitesini kulandik.*/ 
  
 //Timer A0 i�in yapildi asagidaki ayarlar : 
  TA0CTL = TASSEL_1 + MC_1;   // Timer A0 i�in=>  clock kaynagi : aclk se�ildi.  Mode ise =>Up mode: the timer counts up to TACCR0
 //Yandaki ayarlar ise Timer A1 i�in => TA1CTL = TASSEL_1 + MC_1;           =>  MCx Bits 5-4 Mode control. Setting MCx = 00h when Timer_A is not in use conserves power.
 
 // TAIE interrupt ini a�madik. CC ' lerin interruptlarini a�tik => Asagidakiler: 
 // TACCTLx, Capture/Compare Control Register : 
   TA0CCTL0 = CCIE;   /* Timer A Capture/Compare Control 0 */
   TA0CCTL1 = CCIE;  /* Timer A Capture/Compare Control 1 */ // sonradan ekleme TIMER0_1 
                     // Capture/compare interrupt enable. 
  // CAP=0 => Old i�in compare mode'da.
 
   TA0CCR0 =8192-1; //TACCRx,Timer_A0 Capture/Compare Register 0
   //Compare mode: TACCRx holds the data for the comparison to the timer value in the Timer_A Register, TAR.
   // 32768-1; 1 sn ye dek geliyor.
    
   TA0CCR1 =8192-1; // sonradan ekleme TIMER0_1,TACCRx,Timer_A0 Capture/Compare Register 1
   
   // TA0CTL=MC_1;   Yukarda MC_0 yapariz yani baslatmayiz . Yani MC_0 yapip(Halted Mod) degerleri gireriz sonra MC_1(Up Mode) Ile timeri baslatiriz.
   //�stte yazani yaparsak =>  Kurar kurmaz baslattirmayiz  
   // veya butona basinca  TA0CTL=MC_1 yaptiririz ve her butona Timer �alisir.
   
   
    __bis_SR_register(LPM3_bits+GIE);   
    // Islemciyi kesme kaynakli �alistiriyoruz.
    // LPM3 de ACLK �alisir lpm4 de �alismaz.
    // genel kesmeleride a�tik.A�ik olmazsa Program interrupt'a dallanmaz.
    
  
}

#pragma vector =TIMER0_A0_VECTOR // Timer A0 i kullaniyoruz. /* 0xFFF2 Timer0_A CC0 */
__interrupt void ta0_a0_isr(void)
 {
 
   P1OUT ^=BIT0;
 
 }

/* Timer A0_3 3 adet capture/compare i�eriyor.TA0CCTL0,TA0CCTL1,TA0CCTL2 */
#pragma vector =TIMER0_A1_VECTOR // Timer A0 i kullaniyoruz. // sonradan ekleme TIMER0_1
__interrupt void ta0_a1_isr(void)  
 {
// Burda Hangi CCRx dan geldigini kontrol edelim:      
   // 02h Capture/compare 1 TACCR1 CCIFG  Highest  
    //  04h Capture/compare 2 TACCR2 CCIFG
   if(TA0IV == 2)  // 4'se Diger CCR den geliyordur.  
    {
    P1OUT ^=BIT6;
    }
   
   
 }