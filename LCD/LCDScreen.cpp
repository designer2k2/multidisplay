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

#include "LCDController.h"
#include "LCDScreen.h"
#include "LCD4Bit.h"
#include "wiring.h"
#include "MultidisplayController.h"

LCDScreen::LCDScreen() {
	dataP = &(mControllerP->data);
	lcdControllerP = mControllerP->lcdControllerP;
}

LCDScreen::LCDScreen(LCD4Bit *lcdp, uint16_t flags) : lcdp(lcdp) {
	this->flags.all = flags;
	dataP = &(mControllerP->data);
	lcdControllerP = mControllerP->lcdControllerP;
}

void LCDScreen::init() {
	lcdp->init ();

	//Load the specific data:
	if (flags.f.loadBar)
		lcdControllerP->cgramBar();
	if (flags.f.loadDot)
		lcdControllerP->cgramDot();
	if (flags.f.loadBigFont4)
		lcdControllerP->cgramBigFont4();


 	if (flags.f.doCal)
 		mControllerP->DoCal = 1;

 	//Fetch Typ K
 	if (flags.f.doTypK)
 		mControllerP->DoTypK = 1;

 	//Select Serial Out Mode "Convertet":
 	mControllerP->SerOut = flags.f.serial;

 	//print the Serial Header:
 	mControllerP->HeaderPrint();

 	refreshCounter = flags.f.refreshRate;

	this->customInit();
}

void LCDScreen::toggleScreenAB() {
	if ( flags.f.mode == SCREENA )
		flags.f.mode = SCREENB;
	else
		flags.f.mode = SCREENA;
}

void LCDScreen::toggleRefreshCounter () {
	flags.f.refreshRate += 10;
	if (flags.f.refreshRate >= 110)
		flags.f.refreshRate = 0;
	refreshCounter = 0;
}

void LCDScreen::draw () {
	if ( refreshCounter == flags.f.refreshRate ) {

		customDraw();

		refreshCounter = 0;
	} else
		refreshCounter++;
}



