/*
    Copyright 2009-13 Stephan Martin, Dominik Gummel

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

#ifndef OLEDCONTROLLER_H_
#define OLEDCONTROLLER_H_

#include <libs/Serial_LCD/Serial_LCD.h>
#include <libs/Serial_LCD/proxySerial.h>

#include <libs/Goldelox_Serial_4DLib.h>

class OledController {
public:
	OledController();
	virtual ~OledController();
	void myconstructor();
	void reset();

	static void callback(int ErrCode, unsigned char ErrByte);

	void init ();
	void draw ();
	void sendDebugData();

	void toggleScreen ();
	void setActiveScreen (uint8_t as);

	uint8_t activeScreen;
	uint8_t brightness;

	ProxySerial *ps;
	Serial_LCD *display;
//	Goldelox_Serial_4DLib display;
};

extern Goldelox_Serial_4DLib display;

#endif /* OLEDCONTROLLER_H_ */
