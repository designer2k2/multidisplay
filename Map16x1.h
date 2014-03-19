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

#ifndef MAP16X1_H_
#define MAP16X1_H_

#include <avr/pgmspace.h>

#include "util.h"

class Map16x1 {
public:
	Map16x1();
	virtual ~Map16x1();

	uint8_t map (uint8_t idx);
	void loadFromEeprom (int address);
	void writeToEeprom (int address);

	uint8_t *data;
};

class Map16x1Double {
public:
	Map16x1Double();
	virtual ~Map16x1Double();

//	uint8_t map (uint8_t idx);
	double map (uint8_t idx);
	void loadFromEeprom (int address);
	void writeToEeprom (int address);

	double *data;
};


class MapVdo5Bar {
public:
	MapVdo5Bar();

//	uint16_t map16 (uint8_t);
//	uint16_t map32 (uint8_t);
	uint16_t map32 (uint16_t);

	static const uint16_t PROGMEM lookupVDOPressure5Bar[];
};

class MapVdo10Bar {
public:
	MapVdo10Bar();

//	uint16_t map16 (uint8_t);
//	uint16_t map32 (uint8_t);
	uint16_t map32 (uint16_t);

	static const uint16_t PROGMEM lookupVDOPressure10Bar[];
};

extern MapVdo5Bar mapVdo5Bar;
extern MapVdo10Bar mapVdo10Bar;

#endif /* MAP16X1_H_ */
