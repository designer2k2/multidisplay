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


#include "LCDScreen9.h"
#include "WProgram.h"
#include "MultidisplayDefines.h"
#include "MultidisplayController.h"
#include "LCDController.h"

LCDScreen9::LCDScreen9() {
	flags.f.doCal=1;
	flags.f.doTypK = 1;
	flags.f.serial = SERIALOUT_BINARY;
	flags.f.refreshRate = 10;
	flags.f.loadBar = 0;
	flags.f.loadDot = 0;
	flags.f.loadVBar = 0;
	flags.f.loadBigFont4 = 0;

}

//VDO Screen
void LCDScreen9::customInit() {
	lcd.lcdCommandWriteAndPrintIn_P(0x80, PSTR("VDO Screen:"));
	lcd.lcdCommandWriteAndPrintIn_P(0xC0, PSTR("T1:       P1:"));
	lcd.lcdCommandWriteAndPrintIn_P(0x94, PSTR("T2:       P2:"));
	lcd.lcdCommandWriteAndPrintIn_P(0xD4, PSTR("T3:       P3:"));
}

void LCDScreen9::customDraw() {

	//FIXME removed lcdController.print2Blanks(); after each print. if we need it -> custom formatstring!

	lcdController.printInt(0xC0+4, data.VDOTemp1);

	 //P1
	lcdController.printFloat2DP(0xC0+14, float(data.VDOPres1/10.0));
	//T2
	lcdController.printInt(0x94+4, data.VDOTemp2);
	//P2
	lcdController.printFloat2DP(0x94+14, float(data.VDOPres2/10.0));
	//T3
	lcdController.printInt(0xD4+4, data.VDOTemp3);
	//P3
	lcdController.printFloat2DP(0xD4+14, float(data.VDOPres3/10.0));
}

