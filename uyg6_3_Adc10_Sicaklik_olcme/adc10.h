void integer_goster(int ham,int x , int y);
void sicaklik_oku(void);



void sicaklik_init(void)
 {
   /* ADC10 Control 0 : */ 
   ADC10CTL0 &= ~ENC;  /* ADC10 Enable Conversion  */
   ADC10CTL0 = ADC10SHT_3  + SREF_1 + REFON  + ADC10ON +  ADC10IE + MSC; 
  /* 64 x ADC10CLKs */ /* VR+ = VREF+ and VR- = AVSS */ /* ADC10 Reference on  */ 
  /* ADC10 On/Enable  */ /* ADC10 Interrupt Enalbe  */ /* ADC10 Multiple SampleConversion  */
  // Donanimi Açtik. 
 
   
    /* ADC10 Control 1 : */                                                                                                                             
   ADC10CTL1 =  INCH_10 +  ADC10SSEL_0 + ADC10DIV_4 + CONSEQ_2;   // *ADC10OSC seçildi.
   /* Selects Channel 10 */ /* ADC10OSC */  /* ADC10 Clock Divider Select 4 */ /* Repeat single channel */
   //  *Kanal_10 okunacak en son okunacak olan kanali  Input Channel'a bildiririz.
   
   /* ADC10 Data Transfer Control 1  */  // *Bu projeye eklendi.  
   ADC10DTC1 = 32;                     // *32 çevrim yapilacak.(Tek kanal 32 kez okunacak)
  
   __delay_cycles(256);
    // Ref = Vcc-Gnd
   // INCH_10(Sicaklik sensoru) Analog Giris olarak seçildi
   // CONSEQ_2 = Tek kanal ,çoklu çevrim modu.
   // SREF_1 dahili ref gerilimi kullaniyoruz.
   
 }


void sicaklik_oku(void)
 {
  float yeni = 0;
  char i;                      
  static int dma_buf[32]; // static yapalimki debugta görtersin dedi.Debug için static yap dedi.
  // 64 byte alan kullaniyor  // static yapmasa idik stack in içine atacakmis . suan heapte mi acaba ? 
  unsigned int toplam=0;          // static yaparsan debugda görürsün.
 
   /* ADC10 Data Transfer Start Address  */
  ADC10SA = (int)dma_buf;                 // *eklendi , Bu sekilde yazinca dizinin baslangiç adresini verir.
  ADC10CTL0 |= ENC + ADC10SC;    // Bu satir ile donanima start verdik cevrime baslar.
  __bis_SR_register(LPM0_bits + GIE);    // Enter Low Power Mode 0 + interrupts
   
  for(i=0;i<32;i++)
    toplam += dma_buf[i];
   
  toplam >>= 5;      // *5 bit saga kaydirdik yani 32 ye böldük :) .Örneklemeyi çift yapmamizda önemlidir. bölnme kolaylasir
  // veya toplam /= 32 ;
  
  
  yeni = (((toplam -673) * 423) / 1023.0f) * 10;
  // *10 virgülden sonra tek hane demek, 100 olursa çift hane
  
         
  integer_goster((int)toplam,12, 0);         
  integer_goster((int)yeni,6,1);
  
  __no_operation();      // debugta break point yapiyoruz sanirm
 }



void integer_goster(int ham,int x , int y)
 {
   if(y==0){
   lcdSetInt(ham/1000,x,y);      
   lcdSetInt((ham%1000)/100,(x+1),y);
   lcdSetInt((ham%100)/10,(x+2),y);
   lcdSetInt(ham%10,(x+3),y);
    }
   else{
   lcdSetInt(ham/1000,x,y);      
   lcdSetInt((ham%1000)/100,(x+1),y);
   lcdSetInt((ham%100)/10,(x+2),y);
   lcdSetText(".",(x+3),y);
   lcdSetInt(ham%10,(x+4),y);
   
   }
 }



// ADC10 Kesme Vektörü
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)        
 {                                          // Çevrim tamamlaninca burdayiz.....
   __bic_SR_register_on_exit(LPM0_bits);  // Islemci düsük güç modundan çikar.
                                         // bit clear
 }