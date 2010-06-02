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

#include "BoostController.h"
#include "SensorData.h"
#include "MultidisplayDefines.h"

BoostController::BoostController() {
	// TODO Auto-generated constructor stub

}

void BoostController::myconstructor() {
	boostOutput = 0;
	boostSetPoint = BOOST_NORMAL;
	mode = BOOST_MODE_NORMAL;
	boostPid = new PID ( (double*) &data.calBoost, &boostOutput, &boostSetPoint,10,5,1 );
	boostPid->SetOutputLimits(0,255);
	boostPid->SetInputLimits(-1.0, 2.0);
	boostPid->SetMode(AUTO);
	boostPid->SetSampleTime(10);
}


void BoostController::toggleMode (uint8_t nmode) {
	if (mode != nmode) {
//		if ( nmode == BOOST_MODE_NORMAL || nmode == BOOST_MODE_RACE )
		mode = nmode;
		if ( nmode == BOOST_MODE_NORMAL )
			boostSetPoint = BOOST_NORMAL;
		else
			boostSetPoint = BOOST_RACE;
	}
}
