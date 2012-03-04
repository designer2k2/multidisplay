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


/* ================================================================================================ */

MapVdo5Bar::MapVdo5Bar () {
}

uint16_t MapVdo5Bar::map(uint8_t idx) {
    uint8_t l = idx & 0xF;
    idx = idx >> 4;
    return idx < 0xF ? (( ( l * pgm_read_word(&lookupVDOPressure5Bar[idx+1]) ) + ( -1 * (l-16) * pgm_read_word(&lookupVDOPressure5Bar[idx])) ) / 16) : pgm_read_word(&lookupVDOPressure5Bar[idx]);
}

/*
 *
 * when supplied with 5V and a 220Ohm Resistor in series (measuring the Voltage on the Sensor)
 *it has a increasing Resistance with the Pressure (11-184 Ohm).
 *
 * potential divider (Spannungssteiler)
 *
 * +5V ---- [ R1 = 220 Ohm ] --|-- [ VDO Sensor R2 ] ------------ GND
 *                             |
 *                             |- AVR ADC ----------------------- GND
 *         |        U1         |     U2                |
 *         U2 = U * R2/(R1+R2)
 *
 *
 * 12 bit AD -> max 4096
 * >> 7 gives 32 map cells. max is 5Bar = 1866 => we store only the lower 16 values!
 *
 * stores the pressure in mBar for 16 steps of ad readings
 * [0, 128, 256, 384, 512, 640, 768, 896, 1024, 1152, 1280, 1408, 1536, 1664, 1792, 1920]
 */
const uint16_t PROGMEM MapVdo5Bar::lookupVDOPressure5Bar[] =
	{ 0, 0, 107, 333, 571, 834, 1110, 1410, 1752, 2119, 2541, 2950, 3445, 3987, 4622, 5321 };

MapVdo5Bar mapVdo5Bar;
