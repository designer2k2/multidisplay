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
	flags.f.serial = SERIALOUT_BINARY;
	flags.f.refreshRate = 2;
	flags.f.loadBar = 0;
	flags.f.loadDot = 0;
	flags.f.loadVBar = 0;
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

	//FIXME removed lcdController.print2Blanks(); after each print. if we need it -> custom formatstring!
	//T1
	lcdController.printInt(0x80+4, data.calEgt[0]);
	//T2
	lcdController.printInt(0x80+14, data.calEgt[1]);
	//T3
	lcdController.printInt(0xC0+4, data.calEgt[2]);
	//T4
	lcdController.printInt(0xC0+14, data.calEgt[3]);
	//T5
	lcdController.printInt(0x94+4, data.calEgt[4]);
	//T6
	lcdController.printInt(0x94+14, data.calEgt[5]);
	//T7
	lcdController.printInt(0xD4+4, data.calEgt[6]);
	//T8
	lcdController.printInt(0xD4+14, data.calEgt[7]);
}
