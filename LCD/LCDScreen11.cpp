/*
 * LCDScreen11.cpp
 *
 *  Created on: 06.11.2011
 *      Author: bofh
 */

#include "LCDScreen11.h"

#include "Arduino.h"
#include "MultidisplayDefines.h"
#include "MultidisplayController.h"
#include "LCDController.h"
#include "RPMBoostController.h"

LCDScreen11::LCDScreen11() {
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

LCDScreen11::~LCDScreen11() {
}


void LCDScreen11::customInit() {
    lcd.lcdCommandWriteAndPrintIn_P(lcdController.ystart[0], PSTR("gear:"));
    lcd.lcdCommandWriteAndPrintIn_P(lcdController.ystart[0+9], PSTR("speed:"));
    lcd.lcdCommandWriteAndPrintIn_P(lcdController.ystart[1], PSTR("Digifant:"));
//    lcd.lcdCommandWriteAndPrintIn_P(lcdController.ystart[2] + 0, PSTR("ECT:"));
//    lcd.lcdCommandWriteAndPrintIn_P(lcdController.ystart[2] + 9, PSTR("Ign:"));
    lcd.lcdCommandWriteAndPrintIn_P(lcdController.ystart[3] + 0, PSTR("LC :"));
}


void LCDScreen11::customDraw() {
    lcdController.printInt(0x80 + 6, data.gear, 1 );
    lcdController.printInt(0x80 + 15, data.speed, 3 );


#ifdef DIGIFANT_KLINE
    lcdController.printInt(lcdController.ystart[2]+10, mController.df_kline_active_frame, 1);
    lcdController.printInt(lcdController.ystart[2]+15, mController.df_kline_freq_milliseconds, 2);
    lcdController.printInt(lcdController.ystart[3]+10, mController.df_kline_freq_helper0, 2);
    lcdController.printInt(lcdController.ystart[3]+17, mController.df_kline_discarded_frames, 2);
#endif

 }
