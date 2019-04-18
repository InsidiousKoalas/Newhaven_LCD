#include <msp430.h> 
#include "LCD_funks.h"
#include "lab2funks.h"
/*
 * main.c
 *
 *
 *
 */
//void charHandler(char*);

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    // LCD initialization
    P2DIR |= 0x3C;
    P2OUT |= 0x20;

    // Keypad initialization
    P1DIR |= 0x03;	// P1.0-P1.1 as outties
    P1DIR &= ~0x3C;	// set P1.2-P1.5 as innies
    P1REN |= 0x3C;	// enable pulling rezzies
    P1OUT |= 0x3C;	// pully rezzies set to high

    int col = 0;	// declare row index
    int j;
    char buttons[4][4] = {
    		{'1','2','3','A'},
    		{'4','5','6','B'},
    		{'7','8','9','C'},
    		{'*','0','#','D'}};

    int row, poll, numFlag = 0;

    char temp;


    int leng;
    char buffer[32] = "Testing...";
    char press[1] = '1';
//    char * pPtr = &press;

    int i = 1, csrNdx = 0;
    char bkSp = 0x20;
    char *bPtr = &bkSp;
    char clr = 0x01;
    char *clrPtr = &clr;

    init();
    csrNdx = lcdPrint(buffer,1,0);



//	csrNdx = 0;


    while(1){
    	// iterate through 00,01,10,11
    	col++;
    	if(col==0x04)(col=0);
    	P1OUT &= ~0x03;
    	P1OUT |= col;
    	row = 0x04;


    	for(j=0; j<4; j++){         // iterate through columns
    		poll = P1IN;
    		if(~poll & row){    // poll button

//    			press = buttons[j][col];
    			press[1] = buttons[j][col];

    			deb(row);
    			while(~poll & row){
    				poll = P1IN;
    			}
    			deb(row);

    			numFlag = 1;
    			break;
    		}

    		row <<= 0x01;
    	}

    	if(numFlag==1){
    		if(press[1] == '#'){
    			csrNdx = backSpace(csrNdx);
    			numFlag = 0;
    		}
    		else{
    			csrNdx = lcdPrintByte(press[1],csrNdx);
    			numFlag = 0;
    		}
    	}
	}

	return 0;

}

