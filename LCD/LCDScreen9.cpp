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
	flags.f.serial = SERIALOUT_ENABLED;
	flags.f.refreshRate = 2;
	flags.f.loadBar = 0;
	flags.f.loadDot = 0;
	flags.f.loadVBar = 0;
	flags.f.loadBigFont4 = 0;
}

LCDScreen9::~LCDScreen9() {
	// TODO Auto-generated destructor stub
}

//VDO Screen
void LCDScreen9::customInit() {
	lcdp->lcdCommandWriteAndPrintIn_P (0x80, PSTR("VDO Screen:"));
	lcdp->lcdCommandWriteAndPrintIn_P (0xC0, PSTR("T1:       P1:"));
	lcdp->lcdCommandWriteAndPrintIn_P (0x94, PSTR("T2:       P2:"));
	lcdp->lcdCommandWriteAndPrintIn_P (0xD4, PSTR("T3:       P3:"));
}

void LCDScreen9::customDraw() {

	lcdp->commandWrite(0xC0+4);                  //T1
	lcdp->printIn(itoa(data.VDOTemp1, buf, 10));
	lcdController.print2Blanks();
	lcdp->commandWrite(0xC0+14);                 //P1
	lcdController.printfloat(float(data.VDOPres1/10.0),1);
	lcdController.print2Blanks();
	lcdp->commandWrite(0x94+4);                  //T2
	lcdp->printIn(itoa(data.VDOTemp2, buf, 10));
	lcdController.print2Blanks();
	lcdp->commandWrite(0x94+14);                 //P2
	lcdController.printfloat(float(data.VDOPres2/10.0),1);
	lcdController.print2Blanks();
	lcdp->commandWrite(0xD4+4);                  //T3
	lcdp->printIn(itoa(data.VDOTemp3, buf, 10));
	lcdController.print2Blanks();
	lcdp->commandWrite(0xD4+14);                 //P3
	lcdController.printfloat(float(data.VDOPres3/10.0),1);
	lcdController.print2Blanks();
}

