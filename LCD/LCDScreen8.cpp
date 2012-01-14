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
#include "SensorData.h"

LCDScreen8::LCDScreen8() {
	flags.f.doCal=1;
	flags.f.doTypK = 1;
	flags.f.serial = SERIALOUT_BINARY;
	flags.f.refreshRate = 2;
	flags.f.loadBar = 0;
	flags.f.loadDot = 0;
	flags.f.loadVBar = 0;
	flags.f.loadBigFont4 = 0;

	maxr = 0;
}


void LCDScreen8::customInit() {
	lcdp->lcdCommandWriteAndPrintIn_P (lcdController.ystart[0], PSTR("Max "));
	lcdp->lcdCommandWriteAndPrintIn_P (lcdController.ystart[1], PSTR("LD:       RPM:"));
	lcdp->lcdCommandWriteAndPrintIn_P (lcdController.ystart[2], PSTR("LMM:      EGT:"));
	//line 4
	//lcdp->lcdCommandWriteAndPrintIn_P(lcdController.ystart[3], PSTR("T7:       T8:"));
}


void LCDScreen8::toggleMax () {
	//changes the MAX screen through all values
	maxr++;
	if ( maxr >= MAXVALUES )
		maxr = 0;
}

void LCDScreen8::customDraw()  {
	//Header:
	lcdController.printInt(0x80+4, maxr);

	lcdController.printFloat2DP ( lcdController.ystart[1] + 3, data.maxValues[maxr].boost );

	//Max1RPM
	lcdController.printInt ( lcdController.ystart[1]+14, data.maxValues[maxr].rpm );
	//FIXME removed lcdController.print2Blanks(); after each print. if we need it -> custom formatstring!

	//LMM
	lcdController.printInt ( lcdController.ystart[2]+4, data.maxValues[maxr].lmm );

	if ( NUMBER_OF_ATTACHED_TYPK > 0 ) {
		//FIXME
		lcdController.printInt(lcdController.ystart[2]+14, data.maxValues[maxr].egt[0]);
	}
}
