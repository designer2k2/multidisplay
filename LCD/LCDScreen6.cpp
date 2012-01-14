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


#include "LCDScreen6.h"
#include "WProgram.h"
#include "MultidisplayDefines.h"
#include "MultidisplayController.h"
#include "LCDController.h"

LCDScreen6::LCDScreen6() {
	flags.f.doCal=1;
	flags.f.doTypK = 1;
	flags.f.serial = SERIALOUT_BINARY;
	flags.f.refreshRate = 10;
	flags.f.loadBar = 0;
	flags.f.loadDot = 0;
	flags.f.loadVBar = 0;
	flags.f.loadBigFont4 = 0;
	flags.f.loadBigFont2 = 1;
	flags.f.mode = SCREENA;
}


void LCDScreen6::customInit() {
	if ( flags.f.mode == SCREENA ) {
		lcdp->lcdCommandWriteAndPrintIn_P (0x80, PSTR("Boost:"));
		lcdp->lcdCommandWriteAndPrintIn_P (0x94, PSTR("Temp:"));
	 } else {
		 lcdp->lcdCommandWriteAndPrintIn_P (0x80, PSTR("T1:   "));
		 lcdp->lcdCommandWriteAndPrintIn_P (0x94, PSTR("T2:   "));

		 //Clear the - from the screen A
		 lcdp->lcdCommandWriteAndPrintIn_P (0x80+7, PSTR("  "));
	 }

	refreshCounter  = 5;
}


void LCDScreen6::customDraw()  {

	if ( flags.f.mode == SCREENA ){
		//The Boost:

		lcdp->commandWrite(0x80+7);
		if(data.calBoost<0.0) {
			lcdp->print(2);
			lcdp->print(2);
		} else {
			lcdp->printIn("  ");
		}

//		lcdController.bigNum(abs(data.calBoost)*1000,0,9,1);
		lcdController.printBigNum ( (double) data.calBoost, LCD_WIDTH-9, 9, 0, LCD_BIGFONT_2 );

		//The Temp:
//		lcdController.bigNum(abs(data.calCaseTemp)*1000,20,9,1);
		lcdController.printBigNum( (double) data.calCaseTemp, LCD_WIDTH-9, 9, 2, LCD_BIGFONT_2);

	} else {
		//T1:
		// 4 digits
//		snprintf(lcdController.cbuf, LCD_BUFSIZE 21, "%4d", data.calAgt[0]);
//		lcdController.printBigNum (lcdController.cbuf, LCD_WIDTH-8, 8, 0, LCD_BIGFONT_2);

		lcdController.printBigNum ( (uint16_t) data.calEgt[0], 6, 8, 0, LCD_BIGFONT_2);
//		lcdController.bigNum24(data.calAgt[0],0+8);

		//T2:
		//bigNum24(CalAGT[1],20);
//		lcdController.bigNum24(data.calAgt[1],20+8);
		//sollte ganz rechts ausgegeben werden!
		lcdController.printBigNum ( (uint16_t) data.calEgt[1], LCD_WIDTH-8, 8, 2, LCD_BIGFONT_2);
	}
}
