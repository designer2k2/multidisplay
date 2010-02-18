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

LCDScreen8::~LCDScreen8() {
	// TODO Auto-generated destructor stub
}

void LCDScreen8::customInit() {
	lcdp->lcdCommandWriteAndPrintIn_P (0x80, PSTR("Max "));
	lcdp->lcdCommandWriteAndPrintIn_P (0xC0, PSTR("LD:       RPM:"));
	lcdp->lcdCommandWriteAndPrintIn_P (0x94, PSTR("LMM:      AGT:"));
	//line 4
	//lcdp->lcdCommandWriteAndPrintIn_P(0xD4, PSTR("T7:       T8:"));
}


//FIXME broken! screen8Val gets no set yet!
void LCDScreen8::customDraw()  {
	uint8_t screen8Val = 0;

	//Header:
	lcdp->commandWrite(0x80+4);                  //Line
	lcdp->printIn(itoa(screen8Val, buf, 10));


	lcdp->commandWrite(0xC0+3);                  //Max1LD

	if(mController.sdata.maxLdE[screen8Val]<0.0)  {
		lcdp->printIn("-");
	} else {
		lcdp->printIn(" ");
	}
	lcdController.printfloat(abs(mController.sdata.maxLdE[screen8Val]),2);              //Shows current Boost

	lcdp->commandWrite(0xC0+14);                 //Max1RPM
	lcdp->printIn(itoa(mController.sdata.maxRpmE[screen8Val], buf, 10));
	lcdController.print2Blanks();
	lcdp->commandWrite(0x94+4);                  //LMM
	lcdp->printIn(itoa((mController.sdata.maxLmmE[screen8Val]), buf, 10));
	lcdController.print2Blanks();
	lcdp->commandWrite(0x94+14);                 //Max1AGTVal
	lcdp->printIn(itoa(mController.sdata.maxAgtValE[screen8Val], buf, 10));
	lcdController.print2Blanks();
}
