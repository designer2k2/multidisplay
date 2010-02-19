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


#include "LCDScreen4.h"
#include "WProgram.h"
#include "MultidisplayDefines.h"
#include "MultidisplayController.h"
#include "LCDController.h"

LCDScreen4::LCDScreen4() {
	flags.f.doCal=1;
	flags.f.doTypK = 1;
	flags.f.serial = SERIALOUT_ENABLED;
	flags.f.refreshRate = 2;
	flags.f.loadBar = 0;
	flags.f.loadDot = 0;
	flags.f.loadVBar = 0;
}

LCDScreen4::~LCDScreen4() {
	// TODO Auto-generated destructor stub
}

void LCDScreen4::customInit() {
	//Typ K Screen

	//Line1
	lcdp->lcdCommandWriteAndPrintIn_P (0x80, PSTR("T1:       T2:"));
	lcdp->lcdCommandWriteAndPrintIn_P (0xC0, PSTR("T3:       T4:"));
	lcdp->lcdCommandWriteAndPrintIn_P (0x94, PSTR("T5:       T6:"));
	lcdp->lcdCommandWriteAndPrintIn_P (0xD4, PSTR("T7:       T8:"));
}


void LCDScreen4::customDraw()  {

	//This just printÂ´s all the Thermocouple values:
	//like screen2 it only updates every X repeatÂ´s


	lcdp->commandWrite(0x80+4);                  //T1
	lcdp->printIn(itoa((data.calAgt[0]), buf, 10));
	lcdController.print2Blanks();
	lcdp->commandWrite(0x80+14);                 //T2
	lcdp->printIn(itoa((data.calAgt[1]), buf, 10));
	lcdController.print2Blanks();
	lcdp->commandWrite(0xC0+4);                  //T3
	lcdp->printIn(itoa((data.calAgt[2]), buf, 10));
	lcdController.print2Blanks();
	lcdp->commandWrite(0xC0+14);                 //T4
	lcdp->printIn(itoa((data.calAgt[3]), buf, 10));
	lcdController.print2Blanks();
	lcdp->commandWrite(0x94+4);                  //T5
	lcdp->printIn(itoa((data.calAgt[4]), buf, 10));
	lcdController.print2Blanks();
	lcdp->commandWrite(0x94+14);                 //T6
	lcdp->printIn(itoa((data.calAgt[5]), buf, 10));
	lcdController.print2Blanks();
	lcdp->commandWrite(0xD4+4);                  //T7
	lcdp->printIn(itoa((data.calAgt[6]), buf, 10));
	lcdController.print2Blanks();
	lcdp->commandWrite(0xD4+14);                 //T8
	lcdp->printIn(itoa((data.calAgt[7]), buf, 10));
	lcdController.print2Blanks();
}
