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

#include <avr/pgmspace.h>

#include "WProgram.h"
#include "MultidisplayDefines.h"
#include "MultidisplayController.h"
#include "LCDController.h"
#include "LCDScreen1.h"


LCDScreen1::LCDScreen1() {
	flags.f.doCal=1;
	flags.f.doTypK = 1;
	flags.f.serial = SERIALOUT_BINARY;
	flags.f.refreshRate = 0;
	flags.f.mode = 0;

	flags.f.loadBar=1;
	flags.f.loadDot=1;
	flags.f.loadVBar=0;
	flags.f.loadBigFont4=0;
	flags.f.loadBigFont2=0;
}

void LCDScreen1::customInit() {
	//Line1
	lcd.lcdCommandWriteAndPrintIn_P(0x80, PSTR( "EGT:"));
	//Line=3
	lcd.lcdCommandWriteAndPrintIn_P(0x94, PSTR("LD:        Max:"));
}

void LCDScreen1::customDraw() {
	int val = 0;
	int boostMapped = 0;

	//Calibrate it:
	val = data.calLambda/10;
	boostMapped = map(data.anaIn[BOOSTPIN], 0, 4096, 0, 200);
	boostMapped = constrain(boostMapped, 0, 200);

#ifdef LCDTEST
	//just to test the output :-)
	boostMapped = random (200);
	val = random (20);
#endif

	lcd.commandWrite(0x80+5);               //Line 1, position 5!

	//AGT1+AGT2/2 thats because off my 2 Probes
//	char buf[12];
//	lcd.printIn( itoa((data.calAgt[0]+data.calAgt[1])/2, buf, 10));
	lcdController.printInt( 0x80+5, (data.calEgt[0] + data.calEgt[1])/2);

	lcd.print(32);                           //Print a " " at the end, to clear in case its needed

	lcd.commandWrite(0x80+14);              //Line 3,

	lcdController.printFloat (data.calCaseTemp, 10);

	lcd.print(223);                         //Print a " " at the end, to clear in case its needed
	lcd.printIn_P( PSTR("C") );                       //Print a " " at the end, to clear in case its needed

	lcd.commandWrite(0x94+3);

	if (data.calBoost < 0.0) {
		lcd.printIn_P (PSTR("-"));
	} else {
		lcd.printIn_P (PSTR(" "));
	}
	lcdController.printFloat2DP (abs(data.calBoost));              //Shows current Boost

	//Check for new MaxLD, and then save the point where to make the square
	if(data.calBoost > data.maxLd) {
		data.maxLd  = data.calBoost;
		data.maxLdt = boostMapped/10;
	}

//	lcd.command(0x94+16);
	lcdController.printFloat2DP(0x94+16, data.maxLd);                    //Max Boost

	//Lets draw the dot:
	lcd.commandWrite(0xC0);              // Line 2

	for (int i=0; i <= 19; i++) {
		if(i==val) {
			lcd.print(8);        //thats the Dot
		} else {
			if(i <= LAMBDALOWERLIMIT) {
				//from 0-4 a minus then nothing
				lcd.print(6);
			} else {
				if(i <= LAMBDAUPPERLIMIT) {
					//from 5-15 ist nothing and then a plus
					lcd.print(5);
				} else {
					lcd.print(7);
				}
			}
		}
	}

	//Now lets make the Bar:
	lcd.commandWrite(0xD4);                // Line 4
	lcdController.drawBar(20,boostMapped);

	//And draw a Dot at the max pos & 0 Pos

	lcd.commandWrite(0xD4 + data.maxLdt);                // Line 4
	lcd.print(255);

	lcd.commandWrite(0xD4 + data.ldCalPoint);                // Line 4
	lcd.print(255);

}


