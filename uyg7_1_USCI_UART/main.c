#include "io430.h"
#include "lcd_4bit.h"
#include "uart_usci.h"



int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;
  __delay_cycles(100000);
  
  lcdInit();
  uart_init();
  
  lcd_goto(1,1);
  lcd_puts("HUSEYIN KARAMAN");
  lcd_goto(2,1);
  lcd_puts(" HW UART 9600 Bps ");
  
  delay_ms(2000);
  
  lcdClear();
  lcd_goto(1,1);
  lcd_puts("Girilen Karakter: ");
  lcd_goto(2,1);   
  
  uart_puts("MSP430 Egitimi\r\n");
  uart_puts("USCI A0 9600 Baund UART\r\n");
  uart_puts("Girilen Karakter: ");
        
            
  __bis_SR_register(LPM4_bits + GIE);  // Receive modunda LPM4'ü kullanabiliriz.
}
