/*
 * Map32x1.h
 *
 *  Created on: 26.03.2014
 *      Author: bofh
 */

#ifndef MAP32X1_H_
#define MAP32X1_H_

#include <avr/pgmspace.h>

#include "util.h"

class Map32x1 {
public:
	Map32x1();
	virtual ~Map32x1();
};

class MapVdo5Bar {
public:
	MapVdo5Bar();

	uint16_t map32 (uint16_t);

	static const uint16_t PROGMEM lookupVDOPressure5Bar[];
};

class MapVdo10Bar {
public:
	MapVdo10Bar();

	uint16_t map32 (uint16_t);

	static const uint16_t PROGMEM lookupVDOPressure10Bar[];
};

extern MapVdo5Bar mapVdo5Bar;
extern MapVdo10Bar mapVdo10Bar;


#endif /* MAP32X1_H_ */
