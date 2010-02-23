/*
    Copyright 2009-10 Stephan Martin, Dominik Gummel

    This file is part of Multidisplay.

    Multidisplay is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Multidisplay is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Multidisplay.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LCDCONTROLLER_H_
#define LCDCONTROLLER_H_

#include <stdlib.h>
#include <avr/pgmspace.h>
#include <LCD4Bit.h>

class LCDScreen;

/**
 * controlls the LCD screen
 * provides helper methods for drawing
 */
class LCDController {
public:
	LCDController();

	LCD4Bit* lcdp;

	/***
	 * screen layout:
	 * screen1: egt, boost, maxboost, lambda bar, boost bar, castetemp, ldcalpoint
	 * screen2: raw value from 8 inputs
	 * screen3: lambda, boost, throttle, rpm
	 * screen4: 8 TypeK
	 * screen5: boost, maxboost (bigfont)
	 * screen6: boost / casetemp or agt 1 and 2 ( 2 row bigfont )
	 * screen7: scope screen (boost, throttle, lmm or rpm)
	 * screen8: max boost, rpm, lmm, agt
	 * screen9: vdo temp and pressures
	 */

	LCDScreen* myScreens[9];

	void init ();
	void draw ();

	void toggleScreen ();

	uint8_t activeScreen;
	uint8_t brightness;

	const static uint8_t PROGMEM LCDBrightnessD[] ;
	void setBrightness(uint8_t bright);
	void toggleBrightness ();

    void lcdShowIntro(int delayValue);

	void print2Blanks();
	void cgramBigFont2();
	void cgramBigFont4();

	void printBigNum (uint16_t value, uint8_t length, uint8_t x_offset, uint8_t y_offset, uint8_t type);
	void printBigNum (double value, uint8_t length, uint8_t x_offset, uint8_t y_offset, uint8_t type);
	void printBigNum (char* str, uint8_t length, uint8_t x_offset, uint8_t y_offset, uint8_t type);
	//first char of each line
	static const uint8_t ystart[];


	void blanks(uint8_t c);

	// Array index into parts of big numbers. Numbers consist of 6 custom characters in 2 lines
	//              0             1          2        3          4             5         6       7               8       9
//	static const prog_uchar bn12[];
	static char bn12[];
	static char bn22[];
	// Array index into parts of big numbers. Numbers consist of 12 custom characters in 4 lines
	//              0               1            2              3                4             5             6              7              8              9
	static char bn14[];
	static char bn24[];
	static char bn34[];
	static char bn44[];

	//from functions.pde
	void printfloat(float all , int decimals);

	// data has to point to PMEM!
	void lcdUploadUdef5x8_P (uint8_t charIndex, const uint8_t *data);

	void cgramDot();
	void cgramBar();
	void cgramVertBar();

	void cgramIntro();
	void cgramIntro2();
	void drawBar(int Digits, int Value);
	void drawVertBar(int Val, int Pos);
	void scopeMode();

	int scopeInt[20];      //needed for the Scrolling Scope

private:
	char *cbuf;

	void printOneNumber2(uint8_t digit, uint8_t x_offset, uint8_t y_offset=0);
	void printOneNumber4(uint8_t digit, uint8_t x_offset, uint8_t y_offset=0);

};

extern LCDController lcdController;

#endif /* LCDCONTROLLER_H_ */
