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
 

  Vcc_Oku();    // Bir kez çalisacak ve vcc degerini alicaz.
  Adc_Init();  // Initialize ADC
  lcdInit();    // Initialize LCD
  
  

  
  lcdSetText(" MSP430 Egitimi", 0, 0);
  lcdSetText(" Cift Kanal Okuma ", 0, 1);
  delay_ms(2000);
  lcdClear();
 
  TA0CTL |= MC_3;  // Mod => Up-Down Baslatiliyor. 

   //**** LPM0'da Timer kapanmiyor arka planda çalisiyor. ****
 __bis_SR_register(LPM0_bits + GIE);   // Enter Low Power Mode 0 + interrupts

}


#pragma vector=TIMER0_A0_VECTOR 
__interrupt void ta0_ist(void)
 {
  volatile  char tick;
  
  tick++;
  if(tick==5)
   {                               
     static int sonuc[2];                  // *eklendi
     
    while(ADC10CTL0 & ADC10BUSY);          // *eklendi , Adc müsait olana kadar bekle..
    ADC10SA = (int)sonuc;                 // *eklendi , Bu sekilde yazinca dizinin baslangiç adresini verir.
    ADC10CTL0 |= ENC + ADC10SC;    // Bu satir ile donanima start verdik cevrime baslar.
    __bis_SR_register(LPM0_bits + GIE);   // Enter Low Power Mode 0 + interrupts
    // Düsük güç moduna alininca assagiki kodu islemez. 
    // ADC10 Kesme Vektöründe  __bic_SR_register_on_exit(LPM0_bits); lpm bitlerini temizleyince => düsük güçten çikar
    // ve asagidaki kod çalisir.
    // ******** Yani cpu aktif olunca asagidaki koda geçtik..***************
    // LPM0_bits => (CPUOFF) 
     
    lcdSetText("A2 =", 0, 0);        // A2 Kanali  
    integer_goster(sonuc[1],6 , 0);
       // lcdSetInt(sonuc[1]/1000,6,0);     
      //  lcdSetInt((sonuc[1]%1000)/100,7,0);
      //  lcdSetInt((sonuc[1]%100)/10,8,0);
     // lcdSetInt(sonuc[1]%10,9,0);
    
     lcdSetText("A3_10 =", 0, 1);   // A3 Kanali
     integer_goster(sonuc[0],6 , 1);
  //  lcdSetInt(sonuc[0]/1000,6,1);       
  //  lcdSetInt((sonuc[0]%1000)/100,7,1);
  //  lcdSetInt((sonuc[0]%100)/10,8,1);
  //  lcdSetInt(sonuc[0]%10,9,1);
    
    
    tick=0; 
   
   }
 }