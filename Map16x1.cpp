/*
 * Map16x1.cpp
 *
 *  Created on: 03.11.2011
 *      Author: bofh
 */

#include <avr/eeprom.h>

#include "Map16x1.h"
#include "stdlib.h"
#include "EEPROM.h"

Map16x1::Map16x1() {
	data = (uint8_t*) malloc ( sizeof(uint8_t) * 16 );
	for ( uint8_t i = 0 ; i < 16 ; i++ )
		*(data+i) = i;
}

Map16x1::~Map16x1() {
	free (data);
}

uint8_t Map16x1::map (uint8_t idx) {
    uint8_t l = idx & 0xF;
    idx = idx >> 4;
    return idx < 0xF ? (( ( l * data[idx+1] ) + ( -1 * (l-16) * data[idx]) ) / 16) : data[idx];
}

void Map16x1::loadFromEeprom (int address) {
	for ( uint8_t i = 0 ; i < 16 ; i++ )
		*(data+i) = EEPROM.read(address + i);
}

void Map16x1::writeToEeprom (int address) {
	for ( uint8_t i = 0 ; i < 16 ; i++ )
		EEPROM.write(address + i, *(data+i));
}


/* ================================================================================================ */

Map16x1Double::Map16x1Double() {
	data = (double*) malloc ( sizeof(double) * 16 );
	for ( uint8_t i = 0 ; i < 16 ; i++ )
		*(data+i) = i;
}

Map16x1Double::~Map16x1Double() {
	free (data);
}

uint8_t Map16x1Double::map (uint8_t idx) {
    uint8_t l = idx & 0xF;
    idx = idx >> 4;
    return idx < 0xF ? (( ( l * data[idx+1] ) + ( -1 * (l-16) * data[idx]) ) / 16) : data[idx];
}

void Map16x1Double::loadFromEeprom (int address) {
	for ( uint8_t i = 0 ; i < 16 ; i++ )
		*(data+i) = (float)  eeprom_read_word ( (uint16_t*) address + i) / 100.0;
}

void Map16x1Double::writeToEeprom (int address) {
	for ( uint8_t i = 0 ; i < 16 ; i++ )
		eeprom_write_word ( (uint16_t*) address + i, (uint16_t) ((*(data+i))*100.0)  );
}

