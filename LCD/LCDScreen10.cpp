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
#include "RPMBoostController.h"

#ifdef LCD

LCDScreen10::LCDScreen10() {
	flags.f.doCal=1;
	flags.f.doTypK = 1;
//	flags.f.serial = SERIALOUT_ENABLED;
	flags.f.serial = SERIALOUT_BINARY;
	flags.f.refreshRate = 10;
	flags.f.loadBar = 0;
	flags.f.loadDot = 0;
	flags.f.loadVBar = 0;
	flags.f.loadBigFont2 = 0;
	flags.f.loadBigFont4 = 0;
}

LCDScreen10::~LCDScreen10() {
}


void LCDScreen10::customInit() {
    lcd.lcdCommandWriteAndPrintIn_P (lcdController.ystart[0], PSTR("L  :"));
    lcd.lcdCommandWriteAndPrintIn_P (lcdController.ystart[0+9], PSTR("AGT:"));
    lcd.lcdCommandWriteAndPrintIn_P (lcdController.ystart[1], PSTR("BST:"));
    lcd.lcdCommandWriteAndPrintIn_P (lcdController.ystart[1+9], PSTR("P1 :"));
}


void LCDScreen10::customDraw() {
    lcdController.printFloat2DP(lcdController.ystart[0] + 4, data.calLambdaF);
//    lcdController.printInt(lcdController.ystart[0] + 13, data.calThrottle, 3 );

    lcdController.printFloat2DP(lcdController.ystart[1] + 4, data.calBoost);
    lcdController.printInt(lcdController.ystart[1] + 13, data.VDOPres1, 4);
 }

#endif /* LCD */
