/*
 * OledController.cpp
 *
 *  Created on: 29.01.2013
 *      Author: bofh
 */

#include "OledController.h"

#include <libs/Serial_LCD/proxySerial.h>
#include <libs/Serial_LCD/Serial_LCD.h>
#include <Arduino.h>

OledController::OledController () : display(NULL) {
}

OledController::OledController( ProxySerial ps ) : display ( &ps)
{
	//arduino pin connected to reset
	//TODO
//	display.begin(4);

	delay(10);
	  // === Serial port speed up
	display.setSpeed(115200);
	Serial3.begin(115200);

	display.setOrientation(0x03);
	display.setPenSolid(true);
	display.setFontSolid(true);
	display.setFont(0);


//	if (display.checkScreenType()==1) {
//		display.setTouch(true);
//	}
}

OledController::~OledController() {
}
