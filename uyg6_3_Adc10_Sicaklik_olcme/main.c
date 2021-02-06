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
 

  sicaklik_init(); // init adc10 
  lcdInit();    // Initialize LCD
 
  

  
  lcdSetText(" MSP430 Egitimi ", 0, 0);
  lcdSetText(" Sicaklik Olcme ", 0, 1);
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
  
  if(tick==10) // tick i 5 den 10 yaptik
   {                               
    
     
    //  while(ADC10CTL0 & ADC10BUSY);          // *eklendi , Adc müsait olana kadar bekle..
     
    lcdSetText("Toplam_adc =", 0, 0);  
    lcdSetText("*C=", 0, 1);
    sicaklik_oku(); 
     
    tick=0; 
   
   }
  
 }