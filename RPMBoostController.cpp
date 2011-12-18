/*
 * RPMBoostController.cpp
 *
 *  Created on: 05.11.2011
 *      Author: bofh
 */

#include "RPMBoostController.h"
#include "Map16x1.h"
#include "SensorData.h"
#include <wiring.h>
#include <HardwareSerial.h>

RPMBoostController::RPMBoostController() {
}

void RPMBoostController::myconstructor() {
	for ( uint8_t i = 0; i < GEARS ; i++ ) {
		highboost_duty_cycle[i] = new Map16x1();
		highboost_pid_boost[i] = new Map16x1Double();

		lowboost_duty_cycle[i] = new Map16x1();
		lowboost_pid_boost[i] = new Map16x1Double();
	}
	loadFromEEprom();
	boostOutput = 0;
}

void RPMBoostController::toggleMode (uint8_t nmode) {
	if (mode != nmode) {
		mode = nmode;
	}
}


void RPMBoostController::compute () {
	uint8_t gear_index = constrain (data.gear - 1, 0, GEARS-1);
	if ( mode == BOOST_RACE )
		boostOutput = highboost_duty_cycle[gear_index]->map(data.rpm_map_idx);
	else
		boostOutput = lowboost_duty_cycle[gear_index]->map(data.rpm_map_idx);
}

void RPMBoostController::serialSendDutyMap ( uint8_t gear, uint8_t mode, uint8_t serial ) {
	Serial.print("\2");
	uint8_t outbuf = SERIALOUT_BINARY_TAG_N75_DUTY_MAP;
	Serial.write ( (uint8_t*) &(outbuf), sizeof(uint8_t) );
	Serial.write ( (uint8_t*) &(gear), sizeof(uint8_t) );
	Serial.write ( (uint8_t*) &(mode), sizeof(uint8_t) );
	Serial.write ( (uint8_t*) &(serial), sizeof(uint8_t) );
	gear = constrain (gear, 0, GEARS-1);
	for ( uint8_t i = 0 ; i<16 ; i++ )
		if ( mode == 0 )
			Serial.write ( (uint8_t*) &( lowboost_duty_cycle[gear]->data[i] ), sizeof(uint8_t) );
		else
			Serial.write ( (uint8_t*) &( highboost_duty_cycle[gear]->data[i] ), sizeof(uint8_t) );

	Serial.print("\3");
}

void RPMBoostController::serialSendSetpointMap ( uint8_t gear, uint8_t mode, uint8_t serial ) {
	Serial.print("\2");
	int outbuf = SERIALOUT_BINARY_TAG_N75_SETPOINT_MAP;
	Serial.write ( (uint8_t*) &(outbuf), sizeof(uint8_t) );
	Serial.write ( (uint8_t*) &(gear), sizeof(uint8_t) );
	Serial.write ( (uint8_t*) &(mode), sizeof(uint8_t) );
	Serial.write ( (uint8_t*) &(serial), sizeof(uint8_t) );
	gear = constrain (gear, 0, GEARS-1);
	for ( uint8_t i = 0 ; i<16 ; i++ ) {
		if ( mode == 0 )
			outbuf = float2fixedintb100 (lowboost_pid_boost[gear]->data[i]);
		else
			outbuf = float2fixedintb100 (highboost_pid_boost[gear]->data[i]);
		Serial.write ( (uint8_t*) &outbuf, sizeof(int) );
	}
	Serial.print("\3");
}

void RPMBoostController::setDutyMap ( uint8_t gear, uint8_t mode, uint8_t *data ) {
	gear = constrain (gear, 0, GEARS-1);
	for ( uint8_t i = 0 ; i<16 ; i++ ) {
		if ( mode == 0 )
			lowboost_duty_cycle[gear]->data[i] = *(data+i);
		else
			highboost_duty_cycle[gear]->data[i] = *(data+i);
	}
}

void RPMBoostController::setSetpointMap ( uint8_t gear, uint8_t mode, uint16_t *data ) {
	gear = constrain (gear, 0, GEARS-1);
	for ( uint8_t i = 0 ; i<16 ; i++ ) {
		if ( mode == 0 )
			lowboost_pid_boost[gear]->data[i] = fixedintb1002float( *(data+i) );
		else
			highboost_pid_boost[gear]->data[i] = fixedintb1002float( *(data+i) );
	}
}

void RPMBoostController::loadFromEEprom () {
	for ( uint8_t i = 0; i < GEARS ; i++ ) {
		highboost_duty_cycle[i]->loadFromEeprom( EEPROM_N75_HIGH_DUTY_CYCLE_MAPS + i*16 );
		highboost_pid_boost[i]->loadFromEeprom( EEPROM_N75_PID_HIGH_SETPOINT_MAPS + i*32 );
		lowboost_duty_cycle[i]->loadFromEeprom( EEPROM_N75_LOW_DUTY_CYCLE_MAPS + i*16 );
		lowboost_pid_boost[i]->loadFromEeprom( EEPROM_N75_PID_LOW_SETPOINT_MAPS + i*32);
	}
}

void RPMBoostController::writeToEEprom () {
	for ( uint8_t i = 0; i < GEARS ; i++ ) {
		highboost_duty_cycle[i]->writeToEeprom( EEPROM_N75_HIGH_DUTY_CYCLE_MAPS + i*16 );
		highboost_pid_boost[i]->writeToEeprom( EEPROM_N75_PID_HIGH_SETPOINT_MAPS + i*32 );
		lowboost_duty_cycle[i]->writeToEeprom( EEPROM_N75_LOW_DUTY_CYCLE_MAPS + i*16 );
		lowboost_pid_boost[i]->writeToEeprom( EEPROM_N75_PID_LOW_SETPOINT_MAPS + i*32);
	}
}
