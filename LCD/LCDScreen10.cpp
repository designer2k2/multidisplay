/*
 * LCDScreen10.cpp
 *
 *  Created on: 02.04.2010
 *      Author: bofh
 */

#include "LCDScreen10.h"

#include "WProgram.h"
#include "MultidisplayDefines.h"
#include "MultidisplayController.h"
#include "LCDController.h"

LCDScreen10::LCDScreen10() {
	flags.f.doCal=1;
	flags.f.doTypK = 1;
	flags.f.serial = SERIALOUT_ENABLED;
	flags.f.refreshRate = 10;
	flags.f.loadBar = 0;
	flags.f.loadDot = 0;
	flags.f.loadVBar = 0;
	flags.f.loadBigFont2 = 0;
	flags.f.loadBigFont4 = 1;
}

LCDScreen10::~LCDScreen10() {
}


void LCDScreen10::customInit() {
}


void LCDScreen10::customDraw() {
//	lcdController.printBigNum ( (double)data.calBoost, LCD_WIDTH-9, 9, 0, LCD_BIGFONT_4);
	lcdp->commandWrite(0x80);
//	lcdController.printFloat2DP(-1.56789);

	lcdController.printBigNum ( -1.594, LCD_WIDTH-0, 0, 0, LCD_BIGFONT_4);
//	lcdController.printBigNum ( -1.55432, LCD_WIDTH-0, 0, 0, LCD_BIGFONT_2);
//	lcdController.printBigNum ( -1.554, LCD_WIDTH-10, 5, 0, LCD_BIGFONT_2);
//	lcdController.printBigNum ( 0.864, LCD_WIDTH-10, 5, 2, LCD_BIGFONT_2);

//	lcdController.printOneNumber2 (0, 0, 0);
//	lcdController.printOneNumber2 (1, 3, 0);
//	lcdController.printOneNumber2 (2, 6, 0);
//	lcdController.printOneNumber2 (3, 9, 0);
//	lcdController.printOneNumber2 (4, 12, 0);
//	lcdController.printOneNumber2 (5, 15, 0);
//	lcdController.printOneNumber2 (6, 18, 0);

//	lcdController.printOneNumber4 (0, 0, 0);
//	lcdController.printOneNumber4 (1, 3, 0);
//	lcdController.printOneNumber4 (2, 6, 0);
//	lcdController.printOneNumber4 (3, 9, 0);
//	lcdController.printOneNumber4 (4, 12, 0);
//	lcdController.printOneNumber4 (5, 15, 0);
//	lcdController.printOneNumber4 (6, 18, 0);
 }
