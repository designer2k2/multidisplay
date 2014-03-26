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

//uint8_t Map16x1Double::map (uint8_t idx) {
//    uint8_t l = idx & 0xF;
//    idx = idx >> 4;
//    return idx < 0xF ? (( ( l * data[idx+1] ) + ( -1 * (l-16) * data[idx]) ) / 16) : data[idx];
//}
double Map16x1Double::map (uint8_t idx) {
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


/* ================================================================================================ */

MapThrottleBoostReduction::MapThrottleBoostReduction() {
}

double MapThrottleBoostReduction::map (uint8_t idx) {
    uint8_t l = idx & 0xF;
    idx = idx >> 4;
    return idx < 0xF ? (( ( l * pgm_read_word(&lookupThrottleBoostReduction[idx+1]) ) + ( -1 * (l-16) * pgm_read_word(&lookupThrottleBoostReduction[idx])) ) / 16) : pgm_read_word(&lookupThrottleBoostReduction[idx]);
}

const float PROGMEM MapThrottleBoostReduction::lookupThrottleBoostReduction[] =
	{ 0.0, 0.0, 0.0, 0.0, 0.3, 0.8, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
	//0,   16,  32,  48,  64,  80,  96,  112, 128, ... % throttle open

MapThrottleBoostReduction mapThrottleBoostReduction;

