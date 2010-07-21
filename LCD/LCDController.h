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

#define SCREENCOUNT 3

/**
 *  controlls the LCD screen
 * provides helper methods for drawing
 */
class LCDController {
public:

	LCDController();
	//dont use the constructor because I want to control when the global objects get initialized
	void myconstructor();

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

	LCDScreen* myScreens[SCREENCOUNT];

	void init ();
	void draw ();

	void toggleScreen ();
	void setActiveScreen (uint8_t as);

	LCDScreen* getLCDScreen ( uint8_t num );

	uint8_t activeScreen;
	uint8_t brightness;

	const static uint8_t PROGMEM LCDBrightnessD[] ;
	void setBrightness(uint8_t bright);
	void toggleBrightness ();

    void lcdShowIntro(int delayValue);

    static const uint8_t PROGMEM bigfont2data1[];
    static const uint8_t PROGMEM bigfont2data2[];
    static const uint8_t PROGMEM bigfont2data3[];
    static const uint8_t PROGMEM bigfont2data6[];
    static const uint8_t PROGMEM bigfont2data8[];
	void cgramBigFont2();


	static const uint8_t PROGMEM bigfont4data1[];
	static const uint8_t PROGMEM bigfont4data2[];
	static const uint8_t PROGMEM bigfont4data3[];
	static const uint8_t PROGMEM bigfont4data4[];
	static const uint8_t PROGMEM bigfont4data5[];
	static const uint8_t PROGMEM bigfont4data6[];
	static const uint8_t PROGMEM bigfont4data7[];
	static const uint8_t PROGMEM bigfont4data8[];
	void cgramBigFont4();

	void printInt (uint8_t pos, int value, uint8_t length=0);
//	void printInt (uint8_t pos, int value);
	void printString (uint8_t pos, char* str);

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


	void printFloat2DP(uint8_t pos, float f);
	void printFloat2DP(float f);
	void printFloat(uint8_t pos, float f, uint16_t dp);
	void printFloat(float f, uint16_t dp=100);

	// data has to point to PMEM!
	void lcdUploadUdef5x8_P (uint8_t charIndex, const uint8_t *data);

	void lcdUploadUdef5x8(uint8_t charIndex, const uint8_t *data);

	static const uint8_t PROGMEM dotdata1[];
	static const uint8_t PROGMEM dotdata2[];
	static const uint8_t PROGMEM dotdata3[];
	static const uint8_t PROGMEM dotdata4[];
	void cgramDot();

	static const uint8_t PROGMEM bardata1[];
	static const uint8_t PROGMEM bardata2[];
	static const uint8_t PROGMEM bardata3[];
	static const uint8_t PROGMEM bardata4[];
	void cgramBar();

	static const uint8_t PROGMEM vbardata1[];
	static const uint8_t PROGMEM vbardata2[];
	static const uint8_t PROGMEM vbardata3[];
	static const uint8_t PROGMEM vbardata4[];
	static const uint8_t PROGMEM vbardata5[];
	static const uint8_t PROGMEM vbardata6[];
	static const uint8_t PROGMEM vbardata7[];
	void cgramVertBar();

	static const unsigned char PROGMEM idata1[];
	static const unsigned char PROGMEM idata2[];
	static const uint8_t PROGMEM idata3[];
	static const uint8_t PROGMEM idata4[];
	static const unsigned char PROGMEM i2data1[];
	static const unsigned char PROGMEM i2data2[];
	static const unsigned char PROGMEM i2data3[];
	static const unsigned char PROGMEM i2data4[];



	void cgramIntro();
	void cgramIntro2();
	void drawBar(uint8_t digits, uint8_t value);
	void drawVertBar(uint8_t Val, uint8_t Pos);
	void scopeMode();

	int scopeInt[20];      //needed for the Scrolling Scope


private:
	char *cbuf;

	void printOneNumber2(uint8_t digit, uint8_t x_offset, uint8_t y_offset=0);
	void printOneNumber4(uint8_t digit, uint8_t x_offset, uint8_t y_offset=0);
	void float2string ( char* buffer, float f, int dp );
};

extern LCDController lcdController;

#endif /* LCDCONTROLLER_H_ */
