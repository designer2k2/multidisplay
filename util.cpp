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

#include <stdlib.h>
#include <avr/common.h>
#include <avr/io.h>
#include "util.h"
#include "EEPROM.h"

extern char * const __brkval;

uint16_t freeMem(void) {
	//Free Mem: http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1246358387

	char *brkval;
	char *cp;

	brkval = __brkval;
	if (brkval == 0) {
		brkval = __malloc_heap_start;
	}
	cp = __malloc_heap_end;
	if (cp == 0) {
		cp = ((char *)AVR_STACK_POINTER_REG) - __malloc_margin;
	}
	if (cp <= brkval) return 0;

	return cp - brkval;
}

//http://www.openobject.org/opensourceurbanism/Storing_Data
void EEPROMWriteLong(int p_address, long p_value)
{
	byte Byte1 = ((p_value >> 0) & 0xFF);
	byte Byte2 = ((p_value >> 8) & 0xFF);
	byte Byte3 = ((p_value >> 16) & 0xFF);
	byte Byte4 = ((p_value >> 24) & 0xFF);

	EEPROM.write(p_address, Byte1);
	EEPROM.write(p_address + 1, Byte2);
	EEPROM.write(p_address + 2, Byte3);
	EEPROM.write(p_address + 3, Byte4);
}

long EEPROMReadLong(int p_address) {
	byte Byte1 = EEPROM.read(p_address);
	byte Byte2 = EEPROM.read(p_address + 1);
	byte Byte3 = EEPROM.read(p_address + 2);
	byte Byte4 = EEPROM.read(p_address + 3);

	long firstTwoBytes = ((Byte1 << 0) & 0xFF) + ((Byte2 << 8) & 0xFF00);
	long secondTwoBytes = (((Byte3 << 0) & 0xFF) + ((Byte4 << 8) & 0xFF00));
	secondTwoBytes *= 65536; // multiply by 2 to power 16 - bit shift 24 to the left

	return (firstTwoBytes + secondTwoBytes);
}

void EEPROMWriteuint16(int p_address, uint16_t p_value) {
	byte l = ((p_value >> 0) & 0xFF);
	byte h = ((p_value >> 8) & 0xFF);

	EEPROM.write(p_address, l);
	EEPROM.write(p_address + 1, h);

}
uint16_t EEPROMReaduint16(int p_address) {
	byte l = EEPROM.read(p_address);
	byte h = EEPROM.read(p_address + 1);
	uint16_t r = (h << 8) + l;
	return r;
}

uint16_t float2fixedintb100 (float in) {
	return (uint16_t) (in * 100.0);
}
float fixedintb1002float (int in) {
	return (float) (in / 100.0);
}
float fixedintb1002float (uint16_t in) {
	return (float) (in / 100.0);
}


