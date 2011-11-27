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


#include "LCDScreen5.h"

#include "WProgram.h"
#include "MultidisplayDefines.h"
#include "MultidisplayController.h"
#include "LCDController.h"

LCDScreen5::LCDScreen5() {
	flags.f.doCal=1;
	flags.f.doTypK = 1;
	flags.f.serial = SERIALOUT_BINARY;
	flags.f.refreshRate = 10;
	flags.f.loadBar = 0;
	flags.f.loadDot = 0;
	flags.f.loadVBar = 0;
	flags.f.loadBigFont4 = 1;
}


void LCDScreen5::customInit() {
	lcdp->lcdCommandWriteAndPrintIn_P (0x80, PSTR("Boost:"));
	lcdp->lcdCommandWriteAndPrintIn_P (0x94, PSTR("Max:"));
}


void LCDScreen5::customDraw() {

	lcdp->commandWrite(0xC0+7);
	if(data.calBoost < 0.0 ) {
		lcdp->print(2);
		lcdp->print(2);
	} else {
		lcdp->printIn("  ");
	}

	//bigNum(abs(CalBoost)*1000,0,9,1);

	lcdController.printBigNum ( (double)data.calBoost, LCD_WIDTH-9, 9, 0, LCD_BIGFONT_4);


	//Check for new MaxLD
	if(data.calBoost > data.maxLd) {
		data.maxLd  = data.calBoost;
	}

//	lcdp->commandWrite(0xD4);
//	lcdController.printFloat2DP(data.maxLd);                    //Max Boost
	lcdController.printFloat2DP(0xD4,data.maxLd);                    //Max Boost
 }
