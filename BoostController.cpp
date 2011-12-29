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
}

void BoostController::myconstructor() {

	//PID doc
	//http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-introduction/

	boostOutput = 0;
	n75_manual_normal = N75_MANUALDUTY_NORMAL;
	n75_manual_race = N75_MANUALDUTY_RACE;
	boostSetPoint = BOOST_NORMAL;
	mode = BOOST_MODE_NORMAL;
	boostSetPointSave = 0.0;
	idleSetPointActive = 0;
#ifdef BOOSTPID
	boostPid = new PID_Beta6 ( (double*) &data.calBoost, &pidBoostOutput, &pidBoostSetPoint,10,5,1 );
	boostPid->SetOutputLimits(0,255);
	boostPid->SetInputLimits(-1.0, 2.0);
	boostPid->SetMode(MANUAL);
	boostPid->SetSampleTime(20);
#else
	boostPid = 0;
#endif
	boostOutput = n75_manual_normal;
	boostOutputSave = 0.0;
}


void BoostController::toggleMode (uint8_t nmode) {
	if (mode != nmode) {
		mode = nmode;
		if ( nmode == BOOST_MODE_NORMAL ) {
			boostSetPoint = BOOST_NORMAL;
#ifdef BOOSTPID
			if ( boostPid->GetMode() == MANUAL ) {
#endif
				if (boostOutputSave > 0)
					boostOutputSave = n75_manual_normal;
				else
					boostOutput = n75_manual_normal;
#ifdef BOOSTPID
			}
#endif
		} else {
			boostSetPoint = BOOST_RACE;
#ifdef BOOSTPID
			if ( boostPid->GetMode() == MANUAL ) {
#endif
				if (boostOutputSave > 0)
					boostOutputSave = n75_manual_race;
				else
					boostOutput = n75_manual_race;
#ifdef BOOSTPID
			}
#endif
		}
	}
}

void BoostController::compute () {
#ifdef BOOSTPID
	if ( boostPid->GetMode() == AUTO ) {
		if ((data.calThrottle == 0) && !idleSetPointActive) {
			idleSetPointActive = 1;
			boostSetPointSave = pidBoostSetPoint;
			pidBoostSetPoint = 0.0;
		} else if ((data.calThrottle >= 0) && idleSetPointActive) {
			idleSetPointActive = 0;
			pidBoostSetPoint = boostSetPointSave;
		}
		boostPid->Compute();
	} else {
#endif
		//MANUAL
		if ( (data.calThrottle == 0) && boostOutputSave==0.0 ) {
			boostOutputSave = boostOutput;
			boostOutput = 0.0;
		} else if ( (data.calThrottle > 0) && boostOutputSave > 0.0 ) {
			//restore
			boostOutput = boostOutputSave;
			boostOutputSave = 0.0;
		}
#ifdef BOOSTPID
	}
#endif
}
