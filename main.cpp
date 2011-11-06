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


#include <wiring.h>

#include "LCDController.h"
#include "MultidisplayController.h"
#include "SensorData.h"
#include "BoostController.h"
#include "RPMBoostController.h"

/*
ToDo:
- (5) Throttle Calib
- (5) RPM Calib (there should be a real method, not just a factor...)
- (4) Setup Screen
- (5) Battery Meter
- (3) Make the Bignum Screen flexible for all Values
- Reserv Anain and a PWM Out for Wastegate controlling...
- (1) Change tge Typ K reading for the selection to work together with the MCP3208 Chip select !!  be aware read_adc has some debug on the led´s going on!!!
*/

SensorData data;
LCDController lcdController;
MultidisplayController mController;
#ifdef BOOSTN75
RPMBoostController boostController;
#endif

int main(void) {
	/* Must call init for arduino to work properly */
    init();

    data.myconstructor();
    lcdController.myconstructor();
#ifdef BOOSTN75
	boostController.myconstructor();
#endif
	mController.myconstructor();

	for (;;)
		mController.mainLoop();

	// we arent allowed to return from this main function!
}

//Gentoo Bug
//http://bugs.gentoo.org/show_bug.cgi?id=147155
//ln -s /usr/i686-pc-linux-gnu/avr/lib/ldscripts in Debug and Release dir!
//or -T /usr/lib/binutils/avr/2.19.1/ldscripts/avr5.x as linker option
