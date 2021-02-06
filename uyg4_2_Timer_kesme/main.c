/*    
    SMCLK kaynagi,TACTL 'de frekans bolucu ve Up/Down sayici Kullandik:
*/

#include "io430.h"

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  BCSCTL1= CALBC1_1MHZ;
  DCOCTL= CALDCO_1MHZ;  
  __delay_cycles(100000);
  
  P1DIR = BIT0; // Bit0'in Yönünü çikis yaptik. 
  P1OUT=0x00;   // Tüm çikislari 0'ladik.
  
                                  
  
  //Timer A0 için yapildi asagidaki ayarlar : 
  TACTL = TASSEL_2 + MC_0 + ID_3 ;     
  /* Timer A0 için=>  clock kaynagi : Bu sefer SMCLK seçildi TASSEL_2 ile.
     Setting MCx = 00h when Timer_A is not in use conserves power.
    TACTL => IDx Bits 7-6 Input divider. These bits select the divider for the input clock.
    Timer A input divider: 3 ->  /8      
  */ 
    
    
      // TAIE interrupt ini açmadik. CC ' lerin interruptlarini açtik => Asagidakiler: 
     // TACCTLx, Capture/Compare Control Register : 
   TACCTL0 = CCIE;   /* Timer A Capture/Compare 0 */                       
                     // Capture/compare interrupt enable. 
                     // CAP=0 => compare mode'da.
 
   TACCR0 =61500-1; // TACCRx,Timer_A Capture/Compare Register x
  //cap=0'da => Compare mode: TACCRx holds the data for the comparison to the timer value in the Timer_A Register, TAR.
       
   
    
    
              // TA0CTL=MC_1;   Yukarda MC_0 yapariz yani baslatmayiz . Yani MC_0 yapip(Halted Mod) degerleri gireriz sonra MC_1(Up Mode) Ile timeri baslatiriz.
             //üstte yazani yaparsak =>  Kurar kurmaz baslattirmayiz  
            // TA0CTL=MC_1 ve ya baska _2 _3 yaptiririz ve Timer çalisir.
   
   
   
     /************************************ONEMLI**************************************************************/ 
        TA0CTL |=MC_3;   /*  MCx Bits 5-4 Mode ise => /* Timer A mode control: 3 - Up/Down */ // Timer i baslattik
       
      // Clk = > 1 Mhz de sayici 65K da yani Led 65 Ms'er saniyede yanip sönücek
     /* Up/Down mode yaptik ve Up mode' de 65 ms olan süreyi Up/Down mode ile 130 MS'ye ye uzattik.*/
        // BU SUREYI YUKARDA SMCLK'U BÖLERREK UZATABILIRZ......
         // YANI  TACTL => IDx Bits 7-6 Input divider. These bits select the divider for the input clock.
         // CLK/8 = 125 Khz yaptik sayicida  61500-1 'e kadar sayacak se sonra geri 61500-1 den 0 a gidecek .
        
        
      __bis_SR_register(LPM0_bits+GIE);   // Islemciyi kesme kaynakli çalistiriyoruz.
       // LPM3 de ACLK çalisir lpm4 de çalismaz.
      //LPM3' de smclk çalismaz LPM0'a aldik.
     // genel kesmeleride açtik.Açik olmazsa Program interrupt'a dallanmaz.
    
  
}

#pragma vector =TIMER0_A0_VECTOR // Timer A0 i kullaniyoruz. /* 0xFFF2 Timer0_A CC0 */
__interrupt void ta0_a0_isr(void)
 {
 
   P1OUT ^=BIT0;
 
 }


