#include "io430.h"
#include "lcd_4bit.h"
#include "adc10.h"

void main(void)

{

  WDTCTL = WDTPW + WDTHOLD;// Stop Watchdog

  BCSCTL1=CALBC1_1MHZ;
  DCOCTL=CALDCO_1MHZ;
  __delay_cycles(100000);

  /*   Timer Ayar:    */
  /* Timer0_A3 Control : */      
  TA0CTL = TASSEL_2 + TACLR;   // /* Timer A clock source select: 2 - SMCLK */ , /* Timer A counter clear  */      
  /* Timer0_A3 Capture/Compare 0 : */                                   
  TA0CCR0 = 50000-1;    
  /* Timer0_A3 Capture/Compare Control 0 : */
  TA0CCTL0 = CCIE;   /* Capture/compare interrupt enable  */
 

  Vcc_Oku();    // Bir kez �alisacak ve vcc degerini alicaz.
  Adc_Init();  // Initialize ADC
  lcdInit();    // Initialize LCD
  
  

  
  lcdSetText(" MSP430 Egitimi", 0, 0);
  lcdSetText(" ADC10 Uygulamasi ", 0, 1);
  delay_ms(2000);
  lcdClear();
 
  TA0CTL |= MC_3;  // Mod => Up-Down Baslatiliyor. 

   //**** LPM0'da Timer kapanmiyor arka planda �alisiyor. ****
 __bis_SR_register(LPM0_bits + GIE);   // Enter Low Power Mode 0 + interrupts

}


#pragma vector=TIMER0_A0_VECTOR 
__interrupt void ta0_ist(void)
 {
  volatile  char tick;
  
  tick++;
  if(tick==5)
   {                               // �nceki konfig�rasyonu koruyarak :  
    ADC10CTL0 |= ENC + ADC10SC;    // Bu satir ile donanima start verdik cevrime baslar.
    __bis_SR_register(LPM0_bits + GIE);   // Enter Low Power Mode 0 + interrupts
    // D�s�k g�� moduna alininca assagiki kodu islemez. 
    // ADC10 Kesme Vekt�r�nde  __bic_SR_register_on_exit(LPM0_bits); lpm bitlerini temizleyince => d�s�k g��ten �ikar
    // ve asagidaki kod �alisir.
    // ******** Yani cpu aktif olunca asagidaki koda ge�tik..***************
    // LPM0_bits => (CPUOFF) 
    deger_goster(ADC10MEM); // ADC10 Kesme Vekt�r� �evrim tamamlaninca �alisir ve adc sonucunu alabiliriz.
    tick=0; 
   
   }
 }