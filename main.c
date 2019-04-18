#include <msp430.h> 
#include "LCD_funks.h"

/*
 * main.c
 *
 *
 *
 */
//void charHandler(char*);

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    P2DIR |= 0x3C;
    P2OUT |= 0x20;

	int leng;
	char buffer[32] = "Testing the limits of sw";

	init();
	leng = lcdPrint(buffer,1,0);

	int i = 1, csrNdx = 0;
	char bkSp = 0x20;
	char *bPtr = &bkSp;

	while(1){
		if(i==1){
			csrNdx = 16;
			moveCursor(csrNdx);
			strcpy(buffer,"lego");
			csrNdx = lcdPrint(buffer,1,csrNdx);
			i++;
		}
//		if(i==2){
//			backSpace(buffer);
//		}
	}

	return 0;

}
//
//
//void charHandler(char* buffer){
//	int leng = stringLeng(buffer);
//	int csrPos;
//
//	if(leng<15)(csrPos = lcdPrint(buffer, 1, csrNdx);
//	else{
//		lcdPrint()
//	}
//}

