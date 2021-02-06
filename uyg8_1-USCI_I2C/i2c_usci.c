#include "io430.h"
#include "i2c_usci.h"

unsigned char rx[];


void setup_pins(void){

    P1SEL  |= BIT6 + BIT7;   // P1.6 SCL - P1.7 SDA USCI_B0
    P1SEL2 |= BIT6 + BIT7;   // P1.6 SCL - P1.7 SDA USCI_B0
}

void i2c_usci_start_tx(void){ // I2C transmit 
    
    while (UCB0CTL1 & UCTXSTP); // Eger stop gelmisse start gönder.  UCTXSTP  => 0 No STOP generated /  1 Generate STOP 
                                // UCTXSTP is automatically cleared after STOP is generated.    
    UCB0CTL1 |= UCTR + UCTXSTT; // I2C Tx modunda , START Gönderiliyor... 
}                               
 
void i2c_usci_start_rx(void){  // I2C receive
        
   UCB0CTL1 &= ~UCTR;       // I2C Rx Modunda ,
   UCB0CTL1 |= UCTXSTT;     // START Gönderiliyor...
 }

void i2c_usci_stop(void){

   UCB0CTL1 |= UCTXSTP;             // Stop Gönderiliyor...
   while (UCB0CTL1 & UCTXSTP);      // Stop  Gönderilene kadar bekle 
}                                   // (UCTXSTP is automatically cleared after STOP is generated.)  


void i2c_usci_write(const char* data,const char adress,const int length){

    char i=0;
    
    

    i2c_usci_start_tx();
    UCB0TXBUF = adress;          // eeprom'da data yazilacak adres. The transmit data buffer
    while(!(IFG2 & UCB0TXIFG));  // UCB0TXIFG is set when UCB0TXBUF is empty

    for(i=0;i<length;i++)
     {
        UCB0TXBUF = data[i];
        while(!(IFG2 & UCB0TXIFG));  // UCB0TXIFG is set when UCB0TXBUF is empty
    }
    
    i2c_usci_stop();
}

unsigned char* i2c_usci_receive(const char address,const int length){

    unsigned int i=0;
   
    i2c_usci_start_tx();         // eepromda okunmaya baslanacak adresi yolluyacagiz.
    
    UCB0TXBUF = address>>1;          // eeprom'da data yazilacak adres.
    while(!(IFG2 & UCB0TXIFG));  // UCB0TXIFG is set when UCB0TXBUF is empty

    i2c_usci_start_rx();       // Cihazi okumak istedik Rx start. 
    
    while(i<length-1){
         while(!(IFG2 & UCB0RXIFG)); // UCB0RXIFG is set when UCB0RXBUF has received a complete character
         rx[i++] = UCB0RXBUF; // The receive-data buffer
    } 

     i2c_usci_stop();  // Stop gönderdik , 
    // Stop gönderdikten sonra 1 byte'lik veri alacak kadar süremiz kaliyor.
   // (Stop condition gelene kadar eeprom 1 veri daha gönderiyor.)
    
  // Eger son byte almazsak sonraki receive'i baslatinca ilk olarak son byte gönderilir.
    while(!(IFG2 & UCB0RXIFG));
    rx[i]=UCB0RXBUF;  // Son byte burda okunacak:
    
    return (rx);
}


unsigned char i2c_usci_notready(void)
 {
   return (UCB0STAT & UCBBUSY);   /* 0 Bus inactive
                                   * 1 Bus busy */
 }



void i2c_usci_init(const char slave_adress)
{
  setup_pins();
 // __disable_interrupt();                    // Kesmeleri kapattik.

  UCB0CTL1 = UCSWRST;                        // SW Reset Aktif  
  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;      // I2C Master , I2C Seçildi , Senkron Mod   (look : __MSP430_HAS_USCI__)        
  UCB0CTL1 |= UCSSEL_2;                      // Isaret kaynagi SMCLK
  UCB0BR0   = 10;                            // fSCL = SMCLK/12 = ~100kHz   
  UCB0BR1   = 0;
  UCB0I2CSA = slave_adress>>1;               // Slave adresi bir bit saga kaydiriyoruz.sondaki bit R/W'dir.??????
  /*The I2CSAx bits contain the slave address of the external device to be addressed by the USCI_Bx module. */ 
  // Cihaz tx modunda haberlesecegi cihazin adresini kendi(donanim ile) yapiyor. Biz yazilimda adresini göndermiyoruz.
  
  UCB0CTL1  &= ~UCSWRST;                     // SW Reset Pasif , Disabled. USCI reset released for operation.
   
   IE2 |= UCB0TXIE;
   IE2 |= UCB0RXIE;
   UCB0I2CIE |= UCNACKIE;                  // NACK Kesmesi Aktif.Karsidan NACK gelirse kullaniliyor.
 }

 /*  // Eger Nack geldiyse STOP Gönder.
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIABRX_ISR(void){
  
   if(UCB0STAT & UCNACKIFG){   // Eger Nack geldiyse STOP Gönder.
       i2c_usci_stop();
       UCB0STAT &= ~UCNACKIFG;    // Nack bitini sifirladik.
    }
 }
 */ 