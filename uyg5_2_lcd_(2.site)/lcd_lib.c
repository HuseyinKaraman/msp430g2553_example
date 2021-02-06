

#include "lcd_lib.h"



void LCD_init(void)
{
  LCD_ir(0x28);         // dual line, 4 bits
  LCD_ir(0x06);         // increment mode
  LCD_ir(0x0C);         // cursor turned off
  LCD_ir(0x10);         // cursor right
  LCD_ir(0x01);         // clear display
}

void LCD_clear(void)
{
  LCD_ir(0x01);
}

void LCD_home(void)
{
  LCD_ir(0x02);
}

void LCD_Line1(void)
{
  unsigned char i;
  LCD_ir(0x80);
  for (i = 0; i < 16; i++)
  {
    LCD_dr(0x20);
  }
  LCD_ir(0x80); 
}

void LCD_Line2(void)
{
  unsigned char i;
  LCD_ir(0xC0);
  for (i = 0; i < 16; i++)
  {
    LCD_dr(0x20);
  }
  LCD_ir(0xC0); 
}

void LCD_Line3(void)
{
  LCD_ir(0x94);
}

void LCD_Line4(void)
{
  LCD_ir(0xD4);
}

void LCD_posn(char n)
// direct positioning where n = 0-79
{
  char ch;
  ch = n;
  if (ch  >= 40) ch += 24;
  LCD_ir(ch | 0x80);
}

void LCD_ready(void)
{
  char busy;
  LCD_DDR = 0xF0;
  LCD_RW = 1;
  LCD_RS = 0;
  do
  {
    LCD_E  = 1;
    // high 4 bits read first
    // busy flag is D7 of LCD = bit 3 of P2IN
    busy = (LCD_IN & 0x08);
    LCD_E  = 0;
    LCD_E  = 1;
    LCD_E  = 0;
  } while (busy);
  LCD_OUT = 0;
  LCD_DDR = 0xFF;
}

void LCD_ir(char ch)
{
  LCD_ready();
  //LCD_RS = 0;     // not needed      // select instruction register
  //LCD_RW = 0;     // not needed
  LCD_OUT |= ((ch >> 4) & 0x0F);
  LCD_E = 1;
  LCD_E = 0;
  LCD_OUT = 0;
  LCD_OUT |= (ch & 0x0F);
  LCD_E = 1;
  LCD_E = 0;
}

void LCD_dr(char ch)
{
  LCD_ready();
  LCD_RS = 1;        // select data register
  //LCD_RW = 0;      // not needed
  LCD_OUT |= ((ch >> 4) & 0x0F);
  LCD_E = 1;
  LCD_E = 0;
  LCD_OUT = 0;
  LCD_RS = 1;
  LCD_OUT |= (ch & 0x0F);
  LCD_E = 1;
  LCD_E = 0;
}

void LCD_txt(char *s)
{
  while(*s) LCD_dr(*s++);
}

//---------------------------------------------------
// Hardware Initialization
//---------------------------------------------------

void init(void)
{
   // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD; 
 
  P1DIR = 0xFF;  // configure output ports
  
  P2DIR = 0xFF;
  P2SEL = 0x00;  //select for I/O function
  P2OUT = 0x00;
}

//---------------------------------------------------
// software delay
//---------------------------------------------------

void delay(unsigned long d)
{
  unsigned long i;
  for (i = 0; i < d; i++);
}
