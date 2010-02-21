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


#ifndef LCDSCREEN6_H_
#define LCDSCREEN6_H_

#include "LCDScreen.h"

/**
 * 2 Row Big Font Screen
 * SCREENA Boost, CaseTemp
 * SCREENB AGT
 */
class LCDScreen6: public LCDScreen {
public:
	LCDScreen6();

	void customInit ();
	void customDraw ();

private:
};

#endif /* LCDSCREEN6_H_ */
