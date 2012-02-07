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

#ifndef UTIL_H_
#define UTIL_H_

#include <stdint.h>

typedef uint8_t byte;

uint16_t freeMem(void);

void EEPROMWriteLong(int p_address, long p_value);
long EEPROMReadLong(int p_address);

void EEPROMWriteuint16(int p_address, uint16_t p_value);
uint16_t EEPROMReaduint16(int p_address);

uint16_t float2fixedintb100 (float in);
float fixedintb1002float (int in);
float fixedintb1002float (uint16_t in);

uint16_t float2fixedintb1000 (float in);
float fixedintb10002float (uint16_t in);

#endif /* UTIL_H_ */
