#include "lcd_4bit.h"



void lcdWriteData(unsigned char data) {
        delay_us(40);
     //    P2OUT |= RS; // Set RS to Data
	LOWNIB(data >> 4); // Upper nibble
	LCD_STROBE();
	LOWNIB(data); // Lower nibble
        LCD_STROBE(); 
	delay_us(50); // Delay > 47 us
}

void lcdWriteCmd(unsigned char cmd) {
	RS = 0; // Reset RS to CMD
	LOWNIB(cmd >> 4); // Upper nibble
	LCD_STROBE(); 
      	LOWNIB(cmd); // Lower nibble
	LCD_STROBE(); 
	delay_ms(5); // Delay > 1.5ms
}




void lcd_putch(char c)      // Sonradan ekleme
{ 
 RS = 1;
 lcdWriteData(c);
}
 
void lcd_puts(const char * s)   // Sonradan ekleme
{
   RS = 1;
   while(*s)
   lcdWriteData((*s++));
}


void lcd_goto (unsigned char row, unsigned char column)     // Sonradan ekleme
{
   RS = 0; // RESET RS to cmd
   if(row==1)
      lcdWriteCmd(0x80+((column-1)%16));
   else if (row==2)
      lcdWriteCmd(0xC0+((column-1)%16));
}

void lcdClear() {
	lcdWriteCmd(CLEAR);
}


void lcdInit() {
	delay_ms(100);
	// Wait for 100ms after power is applied.

	P2DIR = 0x3F;  // Make pins outputs
	P2OUT = 0x03;  // Start LCD (send 0x03)

	LCD_STROBE();  // Send 0x03 3 times at 5ms then 100 us      
	delay_ms(5);
        LCD_STROBE(); 
	delay_ms(5);
        LCD_STROBE(); 
	delay_ms(5);

	P2OUT = 0x02; // Switch to 4-bit mode
        LCD_STROBE(); 
	delay_ms(5);

	lcdWriteCmd(0x28); // 4-bit, 2 line, 5x8
	lcdWriteCmd(0x08); // Instruction Flow
	lcdWriteCmd(0x01); // Clear LCD
	lcdWriteCmd(0x06); // Auto-Increment
	lcdWriteCmd(0x0C); // Display On, No blink
}