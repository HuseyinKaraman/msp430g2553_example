#include "io430.h"
#include "i2c_usci.h"

#define SLAVE_ADDR    0xAE   //0xA0 OR 0x50 hangisi bi bakilmali donanima 

const char*      tx_data = "fatihinanc.com"; // 14 byte
unsigned char*   rx_data;

void main(void)
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
 
  BCSCTL1 = CALBC1_1MHZ; 
  DCOCTL = CALDCO_1MHZ;
  __delay_cycles(100000);
  
  P1DIR |= BIT0;

    
  i2c_usci_init(SLAVE_ADDR);
  while(i2c_usci_notready());
  
  while(1)
   { 
     while(i2c_usci_notready());  
     i2c_usci_write(tx_data,0x00,14); // tx_data'mizi cihaza gönderdik.veri 0.adresten itibaren yazmakta.
     __delay_cycles(10000);
   
     while(i2c_usci_notready());
     rx_data = i2c_usci_receive(0x00,14); // 0. adresten itibaren okuma yapiyoruz verimizi dogru gönderdikmi diye kontrol ediyoruz.
     rx_data[14] = '\0';     // '\0' = NULL
     
     __delay_cycles(100000);
     P1OUT ^= BIT0;
   
   }
  
}
