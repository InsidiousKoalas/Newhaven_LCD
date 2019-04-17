/*
 * LCD_funks.c
 *
 *  Created on: Apr 15, 2019
 *      Author: bhunt
 */
#include <msp430.h>
#include "LCD_funks.h"

#define		RS		0x04
#define		MOSI	0x08
#define		CLK		0x10
#define		CS		0x20
#define		HOLD	50


void init(){


	char commands[13] = {0x30, 0x30, 0x39, 0x14,
						 0x56, 0x6D, 0x70, 0x0F,
						 0x06, 0x01};

	lcdPrint(commands,0);


}

void lcdPrint(char* outStr, char dataBit){
	int leng, i, k;


	leng = stringLeng(outStr);


//	P2OUT &= ~CLK;
	P2OUT &= ~CS;
	if(dataBit == 1){
		(P2OUT |= RS);
	}
	else{
		(P2OUT &= ~RS);
	}

	__delay_cycles(HOLD);

	for(i = 0; i<leng; i++){		// over each char in array
		for(k = 0; k<8; k++){		// for each bit in char
			if(outStr[i] & 0x80)(P2OUT |= MOSI);
			else (P2OUT &= ~MOSI);

			outStr[i] <<= 1;

			P2OUT &= ~CLK;
			__delay_cycles(HOLD);
			P2OUT |= CLK;
			__delay_cycles(HOLD);
			P2OUT &= ~CLK;
		}
		if((i == 0) && (dataBit == 0))(__delay_cycles(HOLD*20));
		__delay_cycles(HOLD);
	}


	P2OUT |= CS;

}

void moveCursor(char loc){
	unsigned char address[32] = {0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,
							     0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
							     0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,
							     0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF};

	char g = address[loc];
	char *aPntr;

	aPntr = &g;

	lcdPrint(aPntr,0);
}


int stringLeng(char* inStr){
	unsigned int k = 0;

	while(inStr[k] != '\0'){
		k++;
	}

	return k;
}


