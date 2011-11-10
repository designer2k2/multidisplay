/*
 * RPMBoostController.h
 *
 *  Created on: 05.11.2011
 *      Author: bofh
 */

#ifndef RPMBOOSTCONTROLLER_H_
#define RPMBOOSTCONTROLLER_H_

#include "MultidisplayDefines.h"
#include "util.h"

class Map16x1;
class Map16x1Double;

class RPMBoostController {
public:
	RPMBoostController();
	void myconstructor ();

	void serialSendDutyMap ( uint8_t gear, uint8_t mode, uint8_t serial );
	void serialSendSetpointMap ( uint8_t gear, uint8_t mode, uint8_t serial );

	void setDutyMap ( uint8_t gear, uint8_t mode, uint8_t *data );
	void setSetpointMap ( uint8_t gear, uint8_t mode, uint16_t *data );

	void loadFromEEprom ();
	void writeToEEprom ();

	void toggleMode (uint8_t nmode);
	void compute ();

	uint8_t mode;
	Map16x1* highboost_duty_cycle[GEARS];
	Map16x1Double* highboost_pid_boost[GEARS];

	Map16x1* lowboost_duty_cycle[GEARS];
	Map16x1Double* lowboost_pid_boost[GEARS];

	double boostOutput;

	uint8_t cur_gear;
	uint8_t cur_idx;

	//FIXME unused
	double boostSetPoint;
	uint8_t n75_manual_normal;
	uint8_t n75_manual_race;
};

extern RPMBoostController boostController;

#endif /* RPMBOOSTCONTROLLER_H_ */
