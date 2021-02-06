/* https://forum.allaboutcircuits.com/ubs/msp430-lcd-example.568/  */
#include "io430.h"

#define DELAY 20000

// define LCD interface on PORT2
#define LCD_OUT  P2OUT
#define LCD_IN   P2IN
#define LCD_DDR  P2DIR
#define LCD_RS P2OUT_bit.P4
#define LCD_RW P2OUT_bit.P5
#define LCD_E  P2OUT_bit.P6


//---------------------------------------------------
// LCD Interface
//---------------------------------------------------

void LCD_init(void);
void LCD_ready(void);
void LCD_ir(char ch);
void LCD_dr(char ch);
void LCD_txt(char *s);
void LCD_home(void);
void LCD_clear(void);
void LCD_Line1(void);
void LCD_Line2(void);
void LCD_Line3(void);
void LCD_Line4(void);
void LCD_posn(char n);