/*
 * lab2funks.c
 *
 *  Created on: Mar 4, 2019
 *      Author: bhunt
 */

#include <msp430.h>

// Debounce wait timer
void deb(int row){
	int poll;
    unsigned int debounce = 0;

    poll = P1IN;
    while (debounce <= 100){
    	poll = P1IN;
        if (!(~poll&row)){
            debounce++;
        }
        else {
            debounce = 0;
        }
    }
}

//// Print num to LEDs, big endian
//void printLED(int num){
//	int i = 0;
//	P1OUT |= BIT0;
//	__delay_cycles(500000);
//
//	for(i=0; i<4; i++){
//		P1OUT ^= BIT0;
//		if(num&0x08)(P4OUT |= BIT7);
//		else(P4OUT &= ~BIT7);
//		__delay_cycles(250000);
//		num<<=0x01;
//	}
//	P1OUT &= ~BIT0;
//	P4OUT &= ~BIT7;
//}
