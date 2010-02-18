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

#ifndef LCDSCREEN_H_
#define LCDSCREEN_H_

#include <stdint.h>
#include <avr/pgmspace.h>

class LCD4Bit;

struct s_screen_flags_t {
	uint8_t doCal : 1;
	uint8_t doTypK : 1;
	uint8_t mode : 3;

	uint8_t serial : 2;

	uint8_t loadBar :1 ;
	uint8_t loadDot :1 ;
	uint8_t loadVBar :1 ;
	uint8_t loadBigFont4 : 1;
	uint8_t loadBigFont2 : 1;

	uint8_t refreshRate : 8;
	//20 bits used!
};

union screen_flags_t {
	struct s_screen_flags_t f;
	uint32_t all;
};

class LCDScreen {
public:
	LCDScreen();
	LCDScreen (LCD4Bit* lcdp, uint16_t flags);

	virtual ~LCDScreen();

	void init ();
	virtual void customInit () = 0;
	void draw ();
	virtual void customDraw () = 0;

	void toggleScreenAB();
	void toggleRefreshCounter ();

	LCD4Bit* lcdp;
	screen_flags_t flags;


protected:
	uint8_t refreshCounter;

	//TODO FIXME change name
	//used by the draw methods
	char buf[12];           // "-2147483648\0"

};

#endif /* LCDSCREEN_H_ */
