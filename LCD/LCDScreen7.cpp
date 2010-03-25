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


#include "LCDScreen7.h"
#include "WProgram.h"
#include "MultidisplayDefines.h"
#include "MultidisplayController.h"
#include "LCDController.h"

LCDScreen7::LCDScreen7() {
	flags.f.doCal=0;
	flags.f.doTypK = 0;
	flags.f.serial = SERIALOUT_DISABLED;
	flags.f.refreshRate = 10;
	flags.f.loadBar = 0;
	flags.f.loadDot = 0;
	flags.f.loadVBar = 1;
	flags.f.loadBigFont4 = 0;

}


void LCDScreen7::customInit() {
//	val1 = 0;
//	val2 = 5000;
}


//FIXME i think i broke it !
void LCDScreen7::customDraw() {

	//new Constrains if needed
	int ScopeVal = 0;

	switch(flags.f.mode){
	case SCOPE_BOOST:
		ScopeVal = dataP->anaIn[BOOSTPIN];
		break;
	case SCOPE_THROTTLE:
		ScopeVal = dataP->calThrottle;
		break;
	case SCOPE_LMM:
		ScopeVal = dataP->calLMM;
		break;
	case SCOPE_RPM:
		ScopeVal = dataP->calRPM;
		break;
	case SCOPE_LAMBDA:
			ScopeVal = dataP->calLambda;
			break;
	default:
		break;
	}


	//FIXME min and max values are needed!
//	if(ScopeVal<val2)    {
//		val2 = ScopeVal;
//	}
//
//	if(ScopeVal>val1)    {
//		val1 = ScopeVal;
//	}

	//Get the Data:
	int val3 = 0;
	//FIXME fix the data mapping
//	val3 = map(ScopeVal, val2, val1, 0, 33);
	//val3 = constrain(val3, 0, 32);

	//Show only the rightmost bar: (live screen)
	lcdControllerP->drawVertBar(val3,19);

	//Feed it into the Buffer (on the right side)
	lcdControllerP->scopeInt[19] = val3;
	lcdControllerP->scopeMode();



	//Draw the Caption:
	//Line1
//	lcdp->commandWrite(0x80);
//	lcdp->printIn(itoa(flags.f.mode, buf, 10));
	lcdControllerP->printInt(0x80, flags.f.mode);
	lcdp->printIn(":");
//	lcdp->printIn(itoa(refreshCounter, buf, 10));
	lcdControllerP->printInt (0, refreshCounter);

	Serial.println(mControllerP->time);
}


