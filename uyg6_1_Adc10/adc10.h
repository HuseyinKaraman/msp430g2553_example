int Vcc_Oku(void);
// void integer_yaz(int*);
void Adc_Init(void);
void deger_goster(int);


int VCC;


void Adc_Init()
{ 
   /* ADC10 Control 0 : */ 
   ADC10CTL0 &= ~ENC;  /* ADC10 Enable Conversion  */
   ADC10CTL0 = ADC10SHT_3  + SREF_0 + ADC10ON +  ADC10IE;  /* 64 x ADC10CLKs */ /* VR+ = AVCC and VR- = AVSS */ /* ADC10 On/Enable  */ /* ADC10 Interrupt Enalbe  */
  /* ADC10 Control 1 : */                                                                                       // Donanimi Açtik.                                        
   ADC10CTL1 =  INCH_2 +  ADC10SSEL_2  + ADC10DIV_7; /* Selects Channel 2 */ /* MCLK */ /* ADC10 Clock Divider Select 7 */
   /* ADC10 Analog Enable 0 : */
   ADC10AE0 = BIT2; // Analog Giris Aktif Edildi
 
   // 64 x ADC10CLKs , MCLK , ADC10 Clock Divider Select 7
   // A2 (P1.2) Kanali Seçild,. REF = VCC - GND
 }

int Vcc_Oku(void)
{                       //Bir kez çalisacaktir.
   ADC10CTL0 &= ~ENC; 
   ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + REF2_5V + ADC10ON +  ADC10IE;  /* VR+ = VREF+ and VR- = AVSS */  /* ADC10 Reference on  */
   ADC10CTL1 =  INCH_11 +  ADC10SSEL_0; /* Input channel select=> Selects Channel 11 */  /* ADC10OSC */ 
 
  // 64 x ADC10CLKs , Dahili referans gerilimi => 2.5V
  // Giris Knali => INCH_11(VCC/2)
  // ADC10CLK => ADC10SC Seçiliyor.
  // Kesmeler Açik ,  ADC10 Açik , Dahili Ref Üreteci Açik.
   
   __delay_cycles(128);  // REFON => Referans gerilim açilsin  diye ufak  geçikme verdik.
   ADC10CTL0 |= ENC + ADC10SC;   /* ADC10 Enable Conversion + ADC10 Start Conversion  */ 
                // Bu satir ile donanima start verdik cevrime baslar.
   __bis_SR_register(LPM0_bits + GIE);    // Enter Low Power Mode 0 + interrupts 
   //Burada CPUoff olucak.
 
  // ***********************************************************************************************************
 // Çevrim bitince (tamamlaninca ) ADC10_VECTOR une gidicek ve LPM0'dan çikicak ve burdan devam edecek....
   
 VCC = (int)(ADC10MEM * 4.8875f); // (5/1023)*1000 = 4.8875 -> REF = 2.5v  // 2x yapip 5'i kullandik ki dogru sonucu bulalim
 // ADC10MEM  => INCH_11(VCC/2)'den gelen degeri okudu.                   // Çünkü INCH_11 den Vcc/2 gelio.
 return VCC; 
} 


// void integer_yaz(int* deger)
// {}

void deger_goster(int ham)
 {
   int volt =0;
   
   volt=(int)((ham)*((float)VCC/1023));
  
   lcdSetText("10BIT =", 0, 0);
   lcdSetInt(ADC10MEM,9,0);
   
   /* 
   lcdSetText("10BIT =", 0, 0);
   lcdSetInt(ADC10MEM/1000,6,0);      
   lcdSetInt((ADC10MEM%1000)/100,7,0);
   lcdSetInt((ADC10MEM%100)/10,8,0);
   lcdSetInt(ADC10MEM%10,9,0);;
   */
    
   lcdSetText("GERILIM = ", 0, 1);
   lcdSetInt((int)volt/1000,9,1);
   lcdSetText(".", 10, 1);
   lcdSetInt((int)volt%1000,11,1);
 }

// ADC10 Kesme Vektörü
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)        
 {                                          // Çevrim tamamlaninca burdayiz.....
   __bic_SR_register_on_exit(LPM0_bits);  // Islemci düsük güç modundan çikar.
                                         // bit clear
 }
