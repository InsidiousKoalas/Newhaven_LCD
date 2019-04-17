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
	char buffer[32] = "Testing...";

	init();
	lcdPrint(buffer,1);

	int i = 1;

	while(1){
		if(i==1){
			moveCursor(16);
			buffer[0] = 'L';
			buffer[1] = 'o';
			buffer[2] = 'w';
			lcdPrint(buffer,1);
			i++;
		}

	}

	return 0;

}
