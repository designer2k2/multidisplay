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

#ifndef BOOSTCONTROLLER_H_
#define BOOSTCONTROLLER_H_

#include <PID_Beta6.h>
#include <inttypes.h>

class BoostController {
public:
	BoostController();

	void myconstructor ();
	void toggleMode (uint8_t nmode);
	void compute ();

	PID *boostPid;
	double boostSetPoint;
	//used to save the setpoint while throttle is closed
	double boostSetPointSave;
	uint8_t idleSetPointActive;
	double boostOutput;
	double boostOutputSave;
	uint8_t mode;

	uint8_t n75_manual_normal;
	uint8_t n75_manual_race;
};

extern BoostController boostController;

#endif /* BOOSTCONTROLLER_H_ */
