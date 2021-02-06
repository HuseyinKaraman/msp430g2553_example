
#include "io430.h"
#include "lcd_lib.h"

void main(void)
{
  init();
  LCD_init();
  
  LCD_txt("Merhaba");
  LCD_Line2();
  LCD_txt("Dunya");
  
  while(1)
  {
    P1OUT_bit.P0 = 0;
    delay(DELAY);
    P1OUT_bit.P0 = 1;
    delay(DELAY);
  }
  
}