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

#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

#include <LCD4Bit.h>
#include <avr/pgmspace.h>
#include "SensorData.h"
#include"LCDController.h"

class MultidisplayController;

/**
 * this class does all user interface related stuff:
 * x draw the data onto the lcd
 * x handle the buttons
 * x future: handle commands from a pc over serial input?
 */
class UserInterface {

public:
	UserInterface();
	virtual ~UserInterface();

	void buttonCheck(int buttonState);

private:
    LCDController* lcdControllerP;
	SensorData* sdataP;

	uint8_t buttonInt;     //Button state
	unsigned long buttonTime;   //Button time



	//TODO documentation!
	void buttonAHold();
	void buttonAPressed();
	void buttonBHold();
	void buttonBPressed();
};

extern UserInterface ui;

#endif /* USERINTERFACE_H_ */
