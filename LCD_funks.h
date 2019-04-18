/*
 * LCD_funks.h
 *
 *  Created on: Apr 15, 2019
 *      Author: bhunt
 */

#ifndef LCD_FUNKS_H_
#define LCD_FUNKS_H_


void init();
int lcdPrint(char*, char, int);
void moveCursor(char);
int stringLeng(char*);
int backSpace(int);
void jmpLine(char);
int lcdPrintByte(char,int);


#endif /* LCD_FUNKS_H_ */
