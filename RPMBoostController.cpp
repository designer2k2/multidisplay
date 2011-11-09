/*
 * RPMBoostController.cpp
 *
 *  Created on: 05.11.2011
 *      Author: bofh
 */

#include "RPMBoostController.h"
#include "Map16x1.h"
#include "SensorData.h"

RPMBoostController::RPMBoostController() {
}

void RPMBoostController::myconstructor() {
	for ( uint8_t i = 0; i < GEARS ; i++ ) {
		highboost_duty_cycle[i] = new Map16x1();
		highboost_duty_cycle[i]->loadFromEeprom( EEPROM_N75_HIGH_DUTY_CYCLE_MAPS + i*16 );
		highboost_pid_boost[i] = new Map16x1Double();
		highboost_pid_boost[i]->loadFromEeprom( EEPROM_N75_PID_HIGH_SETPOINT_MAPS + i*16 );

		lowboost_duty_cycle[i] = new Map16x1();
		lowboost_duty_cycle[i]->loadFromEeprom( EEPROM_N75_LOW_DUTY_CYCLE_MAPS + i*16 );
		lowboost_pid_boost[i] = new Map16x1Double();
		lowboost_pid_boost[i]->loadFromEeprom( EEPROM_N75_PID_LOW_SETPOINT_MAPS + i*16);
	}
	boostOutput = 0;
}

void RPMBoostController::toggleMode (uint8_t nmode) {
	if (mode != nmode) {
		mode = nmode;
	}
}


void RPMBoostController::compute () {
	if ( mode == BOOST_RACE )
		boostOutput = highboost_duty_cycle[data.gear]->map(data.rpm_map_idx);
	else
		boostOutput = lowboost_duty_cycle[data.gear]->map(data.rpm_map_idx);
}
