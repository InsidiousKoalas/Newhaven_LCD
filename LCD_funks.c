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
#define		HOLD	100


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
			if(outStr[k] & 0x80){
				(P2OUT |= MOSI);
			}
			else{
				(P2OUT &= ~MOSI);
			}

			outStr[k] <<= 1;

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

int stringLeng(char* inStr){
	unsigned int k = 0;

	while(inStr[k] != '\0'){
		k++;
	}

	return k;
}



