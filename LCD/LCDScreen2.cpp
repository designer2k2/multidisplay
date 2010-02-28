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


#include "LCDScreen2.h"
#include "MultidisplayDefines.h"
#include "MultidisplayController.h"

LCDScreen2::LCDScreen2() {
	flags.f.doCal=1;
	flags.f.doTypK = 1;
	flags.f.serial = SERIALOUT_RAW;
	flags.f.refreshRate = 2;
}


void LCDScreen2::customInit() {
	if (flags.f.mode > 1) {
		//Line1
		lcdp->lcdCommandWriteAndPrintIn_P  (0x80, PSTR("A1:       A2:"));
		//Line=2
		lcdp->lcdCommandWriteAndPrintIn_P(0xC0, PSTR("A3:       A4:"));
		//Line=3
		lcdp->lcdCommandWriteAndPrintIn_P(0x94, PSTR("A5:       A6:"));
		//Line=4
		lcdp->lcdCommandWriteAndPrintIn_P(0xD4, PSTR("A7:       A8:"));
	} else {
		//Line=4
		lcdp->lcdCommandWriteAndPrintIn_P(0x80, PSTR("B1:       B2:"));
		//Line=2
		lcdp->lcdCommandWriteAndPrintIn_P(0xC0, PSTR("B3:       B4:"));
		//Line=3
		lcdp->lcdCommandWriteAndPrintIn_P(0x94, PSTR("B5:       B6:"));
		//Line=4
		lcdp->lcdCommandWriteAndPrintIn_P(0xD4, PSTR("B7:       B8:"));
	}
}

void LCDScreen2::customDraw()  {
	uint8_t mcp = flags.f.mode == SCREENA ? 0 : 8;

	//FIXME why do we need to print 2 blanks? if we need them, we should do it with a custom format string
	//A1
	lcdController.printInt(0x80+4, data.anaIn[1+mcp]);
	//A2
	lcdController.printInt(0x80+14, data.anaIn[2+mcp]);
	//A3
	lcdController.printInt(0xC0+4, data.anaIn[3+mcp]);
	lcdController.printInt(0xC0+14, data.anaIn[4+mcp]);
	lcdController.printInt(0x94+4, data.anaIn[5+mcp]);
	lcdController.printInt(0x94+14, data.anaIn[6+mcp]);
	lcdController.printInt(0xD4+4, data.anaIn[7+mcp]);
	lcdController.printInt(0xD4+14, data.anaIn[8+mcp]);
}


