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

#ifndef LCDSCREEN1_H_
#define LCDSCREEN1_H_

#include "LCDScreen.h"

class LCDScreen1: public LCDScreen {
public:
	LCDScreen1();
	virtual ~LCDScreen1();

	void customInit ();
	void customDraw ();
};

#endif /* LCDSCREEN1_H_ */
