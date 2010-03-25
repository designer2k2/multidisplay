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

	maxr = 0;
}


void LCDScreen8::customInit() {
	lcdp->lcdCommandWriteAndPrintIn_P (0x80, PSTR("Max "));
	lcdp->lcdCommandWriteAndPrintIn_P (0xC0, PSTR("LD:       RPM:"));
	lcdp->lcdCommandWriteAndPrintIn_P (0x94, PSTR("LMM:      AGT:"));
	//line 4
	//lcdp->lcdCommandWriteAndPrintIn_P(0xD4, PSTR("T7:       T8:"));
}


void LCDScreen8::toggleMax () {
	//changes the MAX screen through all values
	maxr++;
	if ( maxr >= 4 )
		maxr = 0;
}

void LCDScreen8::customDraw()  {

	//Header:
	lcdControllerP->printInt(0x80+4, maxr);


	lcdp->commandWrite(0xC0+3);                  //Max1LD

	if(dataP->maxLdE[maxr]<0.0)  {
		lcdp->printIn("-");
	} else {
		lcdp->printIn(" ");
	}
	lcdControllerP->printFloat2DP(abs(dataP->maxLdE[maxr]));

	//Max1RPM
	lcdControllerP->printInt(0xC0+14, dataP->maxRpmE[maxr]);
	//FIXME removed lcdControllerP->print2Blanks(); after each print. if we need it -> custom formatstring!

	//LMM
	lcdControllerP->printInt(0x94+4, dataP->maxLmmE[maxr]);

	lcdControllerP->printInt(0x94+14, dataP->maxAgtValE[maxr]);
}
