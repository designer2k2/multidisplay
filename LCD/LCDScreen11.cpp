/*
 * LCDScreen11.cpp
 *
 *  Created on: 06.11.2011
 *      Author: bofh
 */

#include "LCDScreen11.h"

#include "WProgram.h"
#include "MultidisplayDefines.h"
#include "MultidisplayController.h"
#include "LCDController.h"
#include "BoostController.h"
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
    lcdp->lcdCommandWriteAndPrintIn_P (lcdController.ystart[0], PSTR("RPM:"));
    lcdp->lcdCommandWriteAndPrintIn_P (lcdController.ystart[2], PSTR("gear:"));
    lcdp->lcdCommandWriteAndPrintIn_P (lcdController.ystart[1]+10, PSTR("DF:"));
}


void LCDScreen11::customDraw() {
    lcdController.printInt(0x80 + 5, data.calRPM, 4 );
    lcdController.printInt(0x80 + 10, data.rpm_map_idx, 3 );

    lcdController.printInt(lcdController.ystart[1], digitalRead(V2_BUTTON1), 1);
    lcdController.printInt(lcdController.ystart[1]+2, digitalRead(V2_BUTTON2), 1);
    lcdController.printInt(lcdController.ystart[1]+4, digitalRead(V2_BUTTON3), 1);
    lcdController.printInt(lcdController.ystart[1]+6, digitalRead(V2_BUTTON4), 1);

    lcdController.printInt(lcdController.ystart[2]+6, digitalRead(data.gear), 1);

#ifdef BOOSTN75
    lcdController.printInt(lcdController.ystart[3], digitalRead(NORDSCHLEIFENPIN), 1);
    lcdController.printInt(lcdController.ystart[3] + 2, (int) boostController.boostOutput, 3);
#endif

#ifdef DIGIFANT_KLINE
    lcdController.printInt(lcdController.ystart[2]+10, mController.df_kline_active_frame, 1);
    lcdController.printInt(lcdController.ystart[2]+15, mController.df_kline_freq_milliseconds, 2);
    lcdController.printInt(lcdController.ystart[3]+10, mController.df_kline_freq_helper0, 2);
    lcdController.printInt(lcdController.ystart[3]+17, mController.df_kline_discarded_frames, 2);
#endif

 }
