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


#include "LCDScreen8.h"
#include "WProgram.h"
#include "MultidisplayDefines.h"
#include "MultidisplayController.h"
#include "LCDController.h"

LCDScreen8::LCDScreen8() {
	flags.f.doCal=1;
	flags.f.doTypK = 1;
	flags.f.serial = SERIALOUT_ENABLED;
	flags.f.refreshRate = 2;
	flags.f.loadBar = 0;
	flags.f.loadDot = 0;
	flags.f.loadVBar = 0;
	flags.f.loadBigFont4 = 0;
}


void LCDScreen8::customInit() {
	lcdp->lcdCommandWriteAndPrintIn_P (0x80, PSTR("Max "));
	lcdp->lcdCommandWriteAndPrintIn_P (0xC0, PSTR("LD:       RPM:"));
	lcdp->lcdCommandWriteAndPrintIn_P (0x94, PSTR("LMM:      AGT:"));
	//line 4
	//lcdp->lcdCommandWriteAndPrintIn_P(0xD4, PSTR("T7:       T8:"));
}


//FIXME broken! screen8Val gets not set yet!
void LCDScreen8::customDraw()  {
	uint8_t screen8Val = 0;

	//Header:
	lcdController.printInt(0x80+4, screen8Val);


	lcdp->commandWrite(0xC0+3);                  //Max1LD

	if(data.maxLdE[screen8Val]<0.0)  {
		lcdp->printIn("-");
	} else {
		lcdp->printIn(" ");
	}
	lcdController.printFloat2DP(abs(data.maxLdE[screen8Val]));

	//Max1RPM
	lcdController.printInt(0xC0+14, data.maxRpmE[screen8Val]);
	//FIXME removed lcdController.print2Blanks(); after each print. if we need it -> custom formatstring!

	//LMM
	lcdController.printInt(0x94+4, data.maxLmmE[screen8Val]);

	lcdController.printInt(0x94+14, data.maxAgtValE[screen8Val]);
}
