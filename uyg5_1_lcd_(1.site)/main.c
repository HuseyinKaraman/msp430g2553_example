#include "io430.h"

#include "lcd_4bit.h"


void main(void)

{

WDTCTL = WDTPW + WDTHOLD;// Stop Watchdog

BCSCTL1=CALBC1_1MHZ;
DCOCTL=CALDCO_1MHZ;
__delay_cycles(100000);

lcdInit();// Initialize LCD


lcdSetText("Go ", 0, 0);

lcdSetText("Miners! ", 0,1);

 __bis_SR_register(LPM0_bits);   // Enter Low Power Mode 0 without interrupts

}