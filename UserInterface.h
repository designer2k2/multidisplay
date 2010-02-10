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

#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

#include <LCD4Bit.h>
#include <avr/pgmspace.h>
#include "SensorData.h"

class MultidisplayController;

/**
 * this class does all user interface related stuff:
 * x draw the data onto the lcd
 * x handle the buttons
 * x future: handle commands from a pc over serial input?
 */
class UserInterface {

public:
	UserInterface();
	UserInterface(SensorData* p, MultidisplayController *c);
	virtual ~UserInterface();


	void screenInit();
	void screenDraw();

	void buttonCheck(int buttonState);
	int getActiveScreen() const;
	void setActiveScreen(int activeScreen);
    int getBrightness() const
    {
        return brightness;
    }

    void setBrightness(int brightness);
    void lcdShowIntro(int delayValue);

private:
	LCD4Bit* lcdp;
	int activeScreen;
	int brightness;
	SensorData* sdataP;
	MultidisplayController* controllerP;

	uint8_t mcp;
	uint8_t toggleAB;
	uint8_t screen6Var;
	uint8_t screen6DataSel;
	int screen6Val1;
	int screen6Val2;
	int screen8Val;

	char fBuff[7];

	//TODO FIXME change name
	//used by the draw methods
	char buf[12];           // "-2147483648\0"

	unsigned int buttonInt;     //Button state
	unsigned long buttonTime;   //Button time
	int refresh;
	int refreshCounter;
	int scopeInt[20];      //needed for the Scrolling Scope

	const static prog_uint32_t PROGMEM test123[];
	const static unsigned int PROGMEM LCDBrightnessD[] ;


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
	void screen1Init();
	void screen2Init(uint8_t mode=1);
	void screen3Init();
	void screen4Init();
	void screen5Init();
	void screen6Init( uint8_t mode = SCREENA );
	void screen7Init();
	void screen8Init();
	void screen9Init();

	void screen1Draw();
	void screen2Draw();
	void screen3Draw();
	void screen4Draw();
	void screen5Draw();
	void screen6Draw();
	void screen7Draw();
	void screen8Draw();
	void screen9Draw();

	//TODO documentation!
	void buttonAHold();
	void buttonAPressed();
	void buttonBHold();
	void buttonBPressed();

	void print2Blanks();
	void cgramBigFont2();
	void cgramBigFont4();
	void printOneNumber2(uint8_t digit, uint8_t leftAdjust, int LineOffset);
	void printOneNumber4(uint8_t digit, uint8_t leftAdjust);
	void bigNum (unsigned long t, int LineOffset, uint8_t leftAdjust, int d);
	void bigNum4 (unsigned long t, uint8_t leftAdjust, int d);
	void bigNum24(int Value,int LineOffset);
	void blanks3();
	char* format(unsigned long num);

	// Array index into parts of big numbers. Numbers consist of 6 custom characters in 2 lines
	//              0             1          2        3          4             5         6       7               8       9
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
	void lcdUploadUdef5x8(int character, int *data);
	void cgramDot();
	void cgramBar();
	void cgramIntro();
	void cgramIntro2();
	void drawBar(int Digits, int Value);
	void drawVertBar(int Val, int Pos);
	void cgramVertBar();
	void scopeMode();

};

#endif /* USERINTERFACE_H_ */
