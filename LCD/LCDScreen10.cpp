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
#include "BoostController.h"
#include "RPMBoostController.h"

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
	flags.f.loadBigFont4 = 1;
}

LCDScreen10::~LCDScreen10() {
}


void LCDScreen10::customInit() {
    lcdp->lcdCommandWriteAndPrintIn_P (lcdController.ystart[0], PSTR("THR:"));
//    lcdp->lcdCommandWriteAndPrintIn_P (lcdController.ystart[0]+10, PSTR("CT:"));
    lcdp->lcdCommandWriteAndPrintIn_P (lcdController.ystart[0]+10, PSTR("AGT:"));
    lcdp->lcdCommandWriteAndPrintIn_P (lcdController.ystart[1], PSTR("L:"));
    lcdp->lcdCommandWriteAndPrintIn_P (lcdController.ystart[2], PSTR("RPM:"));
//    lcdp->lcdCommandWriteAndPrintIn_P (lcdController.ystart[2], PSTR("P1:"));

    lcdp->lcdCommandWriteAndPrintIn_P (lcdController.ystart[2]+10, PSTR("BST:"));
//    lcdp->lcdCommandWriteAndPrintIn_P (lcdController.ystart[3], PSTR("N75:"));

//
//    lcdp->lcdCommandWriteAndPrintIn_P (lcdController.ystart[0], PSTR("THR:"));
//    lcdp->lcdCommandWriteAndPrintIn_P (lcdController.ystart[0]+10, PSTR("CT:"));
//    lcdp->lcdCommandWriteAndPrintIn_P (lcdController.ystart[1], PSTR("LAMBDA:"));
//    lcdp->lcdCommandWriteAndPrintIn_P (lcdController.ystart[2], PSTR("RPM:"));
//    lcdp->lcdCommandWriteAndPrintIn_P (lcdController.ystart[2]+10, PSTR("BST:"));
}


void LCDScreen10::customDraw() {
    lcdp->commandWrite(0x80 + 5);
    lcdController.printInt(0x80 + 5, data.calThrottle, 3 );
//    lcdController.printFloat2DP(0x80 + 14, data.calCaseTemp);
    lcdController.printInt(0x80 + 16, data.calEgt[0], 4);
    lcdController.printFloat2DP(lcdController.ystart[1] + 3, data.calLambdaF);
#ifdef LAMBDA_E85
    lcdp->lcdCommandWriteAndPrintIn_P (lcdController.ystart[0]+16, PSTR("E85"));
#endif
//    lcdController.printInt (lcdController.ystart[1]+ 14, data.anaIn[LAMBDAPIN], 4);

    lcdController.printInt(lcdController.ystart[2] + 5, data.calRPM, 4);
//    lcdController.printInt(lcdController.ystart[2] + 4, data.VDOPres1, 4);

    lcdController.printFloat2DP(lcdController.ystart[2] + 14, data.calBoost);


#ifdef BOOSTN75
    lcdController.printInt(lcdController.ystart[3], digitalRead(NORDSCHLEIFENPIN), 1);
//    lcdController.printInt(lcdController.ystart[3] + 1, digitalRead(FREEANALOG2), 1);
    lcdController.printInt(lcdController.ystart[3] + 5, (int) boostController.n75_manual_normal);
    lcdController.printInt(lcdController.ystart[3] + 9, (int) boostController.n75_manual_race);
    lcdController.printInt(lcdController.ystart[3] + 13, (int) boostController.boostOutput);
#endif

 }
