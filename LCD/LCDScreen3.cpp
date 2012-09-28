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


#include "LCDScreen3.h"
#include "Arduino.h"
#include "MultidisplayDefines.h"
#include "MultidisplayController.h"
#include "LCDController.h"

LCDScreen3::LCDScreen3() {
	flags.f.doCal=1;
	flags.f.doTypK = 1;
	flags.f.serial = SERIALOUT_BINARY;
	flags.f.refreshRate = 10;
	flags.f.loadBar = 1;
}

void LCDScreen3::customInit() {
	//Line1
	lcd.lcdCommandWriteAndPrintIn_P (0x80, PSTR("LAM:"));
	//Line=2
	lcd.lcdCommandWriteAndPrintIn_P(0xC0, PSTR("LD :"));
	//Line=3
	lcd.lcdCommandWriteAndPrintIn_P(0x94, PSTR("THR:"));
	//Line=4
	lcd.lcdCommandWriteAndPrintIn_P(0xD4, PSTR("RPM:"));
}


void LCDScreen3::customDraw()  {

#ifdef LCDTEST
	data.anaIn[BOOSTPIN] = random (4096);
	data.anaIn[RPMPIN] = random (RPMMAX);
	data.calLambda = random (200);
	data.calThrottle = random (100);
#endif

	//First, show the Bars on the last 10 Chars:

	int boostMapped  = map(data.anaIn[BOOSTPIN], 0, 4096, 0, 100);
	boostMapped = constrain(boostMapped, 0, 100);
	int rpmMapped = map(data.anaIn[RPMPIN], 0, RPMMAX, 0, 100);
	rpmMapped = constrain(rpmMapped, 0, 100);

	//Print the Values:


//	lcd.command(0x80+5);
//	lcd.printIn(itoa(data.calLambda, buf, 10));
#ifdef LAMBDA_WIDEBAND
	lcdController.printFloat2DP(0x80+5, data.calLambdaF);
#else
	lcdController.printInt(0x80+5, data.calLambda);
#endif
	lcd.print(32);
	lcd.commandWrite(0xC0+4);

	if(data.calBoost<0.0) {
		lcd.printIn_P(PSTR("-"));
	} else {
		lcd.printIn_P(PSTR(" "));
	}
	lcdController.printFloat2DP(abs(data.calBoost));              //Shows current Boost

	lcd.print(32);
	lcdController.printInt(0x94+5, data.calThrottle);
	lcd.print(32);
	//gets rid of the last 10, thats not needed.
	lcdController.printInt(0xD4+5, (data.calRPM/10)*10);
	lcd.print(32);


	//Now lets make the Bars:
	lcd.commandWrite(0x80+10);
	lcdController.drawBar(10,data.calLambda/2);
	lcd.commandWrite(0xC0+10);
	lcdController.drawBar(10,boostMapped);
	lcd.commandWrite(0x94+10);
	lcdController.drawBar(10,data.calThrottle);
	lcd.commandWrite(0xD4+10);
	lcdController.drawBar(10,rpmMapped);
}

