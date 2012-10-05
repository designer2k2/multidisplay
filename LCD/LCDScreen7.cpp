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
	flags.f.mode=SCOPE_BOOST;
	flags.f.doCal=0;
	flags.f.doTypK = 0;
	flags.f.serial = SERIALOUT_BINARY;
	flags.f.refreshRate = 10;
	flags.f.loadBar = 0;
	flags.f.loadDot = 0;
	flags.f.loadVBar = 1;
	flags.f.loadBigFont4 = 0;

}


void LCDScreen7::customInit() {
	max = 0;
	min = 5000;
}

void LCDScreen7::toggleMode() {
	flags.f.mode++;
	if ( flags.f.mode > SCOPE_LAMBDA )
		flags.f.mode = SCOPE_BOOST;
}

void LCDScreen7::customDraw() {

	//new Constrains if needed
	int ScopeVal = 0;

	switch(flags.f.mode){
	case SCOPE_BOOST:
		ScopeVal = data.anaIn[BOOSTPIN];
		break;
	case SCOPE_THROTTLE:
		ScopeVal = data.calThrottle;
		break;
	case SCOPE_LMM:
		ScopeVal = data.calLMM;
		break;
	case SCOPE_RPM:
		ScopeVal = data.calRPM;
		break;
	case SCOPE_LAMBDA:
			ScopeVal = data.calLambda;
			break;
	default:
		break;
	}

#ifdef LCDTEST
	ScopeVal = random (5000);
#endif

	if(ScopeVal<min)    {
		min = ScopeVal;
	}
	if(ScopeVal>max)    {
		max = ScopeVal;
	}

	//Get the Data:
	int d = 0;
	d = map(ScopeVal, min, max, 0, 33);
	d = constrain(d, 0, 32);

	//Show only the rightmost bar: (live screen)
	lcdController.drawVertBar(d,19);

	//Feed it into the Buffer (on the right side)
	lcdController.scopeInt[19] = d;
	lcdController.scopeMode();



	//Draw the Caption:
	//Line1
	lcdController.printInt(0x80, flags.f.mode);
	lcd.printIn(":");
	lcdController.printInt (0, refreshCounter);

	Serial.println(mController.time);
}


