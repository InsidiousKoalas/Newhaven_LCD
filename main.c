#include <msp430.h> 
#include "LCD_funks.h"

/*
 * main.c
 *
 *
 *
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    P2DIR |= 0x3C;
    P2OUT |= 0x20;

	int leng;
	char test[5] = "hello";

	init();
	lcdPrint(test,1);

	while(1){

	}

	return 0;

}
