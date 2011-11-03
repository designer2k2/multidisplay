/*
 * Map16x1.cpp
 *
 *  Created on: 03.11.2011
 *      Author: bofh
 */

#include "Map16x1.h"
#include "stdlib.h"
#include "EEPROM.h"

Map16x1::Map16x1() {
	data = (uint8_t*) malloc ( sizeof(uint8_t) * 16 );
	for ( uint8_t i = 0 ; i < 16 ; i++ )
		*(data+i) = 0;
}

Map16x1::~Map16x1() {
	// TODO Auto-generated destructor stub
}

uint8_t Map16x1::map (uint8_t idx) {
    uint8_t l = idx & 0xF;
    idx = idx >> 4;
    return ( ( l * data[idx+1] ) + ( -1 * (l-16) * data[idx]) ) / 16;
}

void Map16x1::loadFromEeprom (int address) {
	for ( uint8_t i = 0 ; i < 16 ; i++ )
		*(data+i) = EEPROM.read(address + i);
}
