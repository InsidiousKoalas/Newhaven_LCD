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
#define		HOLD	70


/*
 * calls lcdPrint; initializes LCD to default settings
 */
void init(){
	char commands[13] = {0x30, 0x30, 0x39, 0x14,
						 0x56, 0x6D, 0x70, 0x0F,
						 0x06, 0x01};

	lcdPrint(commands,0,0);
}

/*
 * Sends data (either commands or characters to display) to
 * the LCD.
 *
 * Inputs:
 *
 * 		outStr:		data sent to LCD
 * 		dataBit:	if command, == 0; data, == 1
 * 		csrPos:		the current position of the cursor
 * 					(does not matter for command)
 *
 * If a command, returns a 0 when complete. If data,
 * returns the new cursor position.
 *
 * Calls jmpLine(dir) when reaches end of line
 *
 */
int lcdPrint(char* outStr, char dataBit, int csrPos){
	int leng, i, k;

	leng = stringLeng(outStr);
	if(leng==0)(leng=1);

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

		// delay cycle for initial 'wake up' command
		if((i == 0) && (dataBit == 0))(__delay_cycles(HOLD*20));
		__delay_cycles(HOLD);

		// increment position index
		csrPos++;

		// jump to next line?
		if(csrPos==16){
			jmpLine(1);
		}

		// return to line 1?
		else if(csrPos==32){
			jmpLine(0);
			csrPos = 0;
		}

	}

	__delay_cycles(HOLD*10);
	P2OUT |= CS;

	if(dataBit == 1){
		return csrPos;
	}
	else{
		return 0;
	}
}


/*
 *	Jumps cursor to character index input as a char
 *	by calling lcdPrint
 */
void moveCursor(char loc){
	unsigned char address[32] = {0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,
							     0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
							     0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,
							     0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF};

	char g = address[loc];
	char *aPntr;

	aPntr = &g;

	g = lcdPrint(aPntr,0,loc-1);
}

/*
 * returns the length of a string from the char pointer to it
 */
int stringLeng(char* inStr){
	unsigned int k = 0;

	while(inStr[k] != '\0'){
		k++;
	}

	return k;
}


/*
 * This function removes the character
 * immediately preceding the cursor.
 */
int backSpace(int csrPos){
	char bkSp = 0x20;		// empty character
	char *bPtr = &bkSp;		// pointer to char (bc lcdPrint takes char*)


	moveCursor(csrPos-1);	// send empty character to preceding space
	csrPos = lcdPrint(bPtr,1,csrPos);	// write character

	// prevents backspacing beyond screen boundary,
	// does not enable text wrapping from row 0 to
	// row 1
	if(csrPos>1){
		moveCursor(csrPos-2);
		return csrPos-2;
	}
	else{
		moveCursor(0);
		return 0;
	}
}


/*
 * Jump line command. Moves cursor to lines on LCD
 * following the convention:
 *
 * 				 Display
 * 		 	 ----------------
 * 	Row 0:	|################|
 * 	Row 1:	|################|
 *  	 	 ----------------
 *
 *  jmpLine(0) moves the cursor to line 0.
 *  jmpLine(1) moves the cursor to line 1.
 *
 *  differs from moveCursor because moveCursor calls
 *  lcdPrint whereas jmpLine interrupts lcdPrint
 *
 */
void jmpLine(char row){
	int k, line;
	int line1 = 0x80;
	int line2 = 0xC0;

	if(row == 1)(line = line2);
	else(line = line1);

	P2OUT &= ~RS;	// send command bit

	__delay_cycles(HOLD);
	for(k = 0; k<8; k++){		// for each bit in char
		if(line & 0x80)(P2OUT |= MOSI);
		else (P2OUT &= ~MOSI);

		line <<= 1;

		P2OUT &= ~CLK;
		__delay_cycles(HOLD);
		P2OUT |= CLK;
		__delay_cycles(HOLD);
		P2OUT &= ~CLK;
		__delay_cycles(HOLD);
	}

	P2OUT |= RS;
	__delay_cycles(HOLD*3);

}

int lcdPrintByte(char byte, int csrPos){
	int k;



	P2OUT &= ~CS;
	P2OUT |= RS;	// send command bit

	__delay_cycles(HOLD);
	for(k = 0; k<8; k++){		// for each bit in char
		if(byte & 0x80)(P2OUT |= MOSI);
		else (P2OUT &= ~MOSI);

		byte <<= 1;

		P2OUT &= ~CLK;
		__delay_cycles(HOLD);
		P2OUT |= CLK;
		__delay_cycles(HOLD);
		P2OUT &= ~CLK;
		__delay_cycles(HOLD);
	}

	csrPos++;

	if(csrPos==16)(jmpLine(1));
	else if(csrPos==32){
		jmpLine(0);
		csrPos = 0;
	}

	P2OUT |= CS;
	__delay_cycles(HOLD*3);



	return csrPos;

}
