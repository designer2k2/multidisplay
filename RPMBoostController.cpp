/*
 * RPMBoostController.cpp
 *
 *  Created on: 05.11.2011
 *      Author: bofh
 */

#include "RPMBoostController.h"
#include "Map16x1.h"
#include "SensorData.h"
#include <Arduino.h>
#include <HardwareSerial.h>
#include "EEPROM.h"

RPMBoostController::RPMBoostController() {
}

void RPMBoostController::myconstructor() {
	for ( uint8_t i = 0; i < GEARS ; i++ ) {
		highboost_duty_cycle[i] = new Map16x1();
		highboost_pid_boost[i] = new Map16x1Double();

		lowboost_duty_cycle[i] = new Map16x1();
		lowboost_pid_boost[i] = new Map16x1Double();
	}
	loadMapsFromEEprom();
	loadParamsFromEEprom();

	boostOutput = 0;

//	PID::PID(double* Input, double* Output, double* Setpoint, double Kp, double Ki, double Kd, int ControllerDirection)
	pid = new PID( (double*) &data.calBoost, &pidBoostOutput, &pidBoostSetPoint, aKp, aKi, aKd, DIRECT);

	//default sample time is 100ms
	//pid->SetSampleTime(50);
	pid->SetMode(AUTOMATIC);

	pidBoostSetPoint = 0;
	pidBoostOutput = 0;

	req_Boost_PWM = 0;
	req_Boost = 0;

}

void RPMBoostController::toggleMode (uint8_t nmode) {
	//FIXME
	if (mode != nmode) {
		mode = nmode;
	}
}


void RPMBoostController::compute () {
	uint8_t gear_index = constrain (data.gear - 1, 0, GEARS-1);

	if ( mode == BOOST_MODE_RACE ) {
		req_Boost_PWM = highboost_duty_cycle[gear_index]->map(data.rpm_map_idx);
		req_Boost = highboost_pid_boost[gear_index]->map(data.rpm_map_idx);
//		boostOutput = highboost_duty_cycle[gear_index]->map(data.rpm_map_idx);
	} else {
//		boostOutput = lowboost_duty_cycle[gear_index]->map(data.rpm_map_idx);
		req_Boost_PWM = lowboost_duty_cycle[gear_index]->map(data.rpm_map_idx);
		req_Boost = lowboost_pid_boost[gear_index]->map(data.rpm_map_idx);
	}

	if ( usePID ) {
		//give the PID the requested boost level
		pidBoostSetPoint = req_Boost;
		//activate the PID only if a stable boost is reached
		double aat = pidBoostSetPoint * apidActivationThresholdFactor;
		double cat = pidBoostSetPoint * cpidActivationThresholdFactor;

		if ( data.calBoost > aat  ) {
			if ( data.calBoost > cat  ) {
				//double Kp, double Ki, double Kd
				pid->SetTunings(cKp, cKi, cKd);
				aggressiveSettings = false;
			} else {
				pid->SetTunings(aKp, aKi, aKd);
				aggressiveSettings = true;
			}
			//TODO test this on the road!
			//check if current boost is too far away from the map values
			//-> do we need to adjust the output pwm to a value nearer at the map value?
			//if ( abs(req_Boost - data.calBoost) > (req_Boost/2) )
			//	pidBoostOutput = req_Boost_PWM;

			pid->Compute();
			boostOutput = pidBoostOutput;
		} else {
			//we're under the PID activation thresholds
			//set the map pwm value as base for the pid controller
			pidBoostOutput = req_Boost_PWM;
			//set the map pwm as output pwm
			//we want a silent N75 on idle
			if ( data.calRPM > 1200 && data.calThrottle > 50 )
				boostOutput = req_Boost_PWM;
			else boostOutput = 0;
//			boostOutput = req_Boost_PWM;
		}
	} else {
		//no PID
		//we want a silent N75 on idle
//		if ( data.calRPM > 1200 && data.calThrottle > 50 )
//			boostOutput = req_Boost_PWM;
//		else
//			boostOutput = 0;
//		//test
		boostOutput = req_Boost_PWM;
	}

	//overboost protection
	if ( data.calBoost > n75_max_boost ) {
		boostOutput = boostOutput * 0.75;
		pidBoostOutput = boostOutput;
	}
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

void RPMBoostController::loadMapsFromEEprom () {
	if ( EEPROMReaduint16(EEPROM_N75_PID_LOW_SETPOINT_MAPS) == 0xFFFF ) {
		//load default map values
		for ( uint8_t i = 0; i < GEARS ; i++ ) {
			for ( uint8_t j = 0; j < 16 ; j++ ) {
			*(highboost_duty_cycle[i]->data + j) = 180;
			*(highboost_pid_boost[i]->data + j) = 1.0;
			*(lowboost_duty_cycle[i]->data + j) = 100;
			*(lowboost_pid_boost[i]->data + j) = 0.7;
			}
		}
	} else {
		for ( uint8_t i = 0; i < GEARS ; i++ ) {
			highboost_duty_cycle[i]->loadFromEeprom( EEPROM_N75_HIGH_DUTY_CYCLE_MAPS + i*16 );
			highboost_pid_boost[i]->loadFromEeprom( EEPROM_N75_PID_HIGH_SETPOINT_MAPS + i*32 );
			lowboost_duty_cycle[i]->loadFromEeprom( EEPROM_N75_LOW_DUTY_CYCLE_MAPS + i*16 );
			lowboost_pid_boost[i]->loadFromEeprom( EEPROM_N75_PID_LOW_SETPOINT_MAPS + i*32);
		}
	}
}
void RPMBoostController::loadParamsFromEEprom () {
	uint16_t raw = 0;
	raw = EEPROMReaduint16 (EEPROM_N75_PID_aKp);
	if ( raw < 0xFFFF )
		aKp = fixedintb1002float(raw);
	else
		aKp = 4;
	raw = EEPROMReaduint16 (EEPROM_N75_PID_aKd);
	if ( raw < 0xFFFF )
		aKd = fixedintb1002float(raw);
	else
		aKd = 0.2;
	raw = EEPROMReaduint16 (EEPROM_N75_PID_aKi);
	if ( raw < 0xFFFF )
		aKi = fixedintb1002float(raw);
	else
		aKi = 1;

	raw = EEPROMReaduint16 (EEPROM_N75_PID_cKp);
	if ( raw < 0xFFFF )
		cKp = fixedintb1002float(raw);
	else
		cKp = 1;
	raw = EEPROMReaduint16 (EEPROM_N75_PID_cKd);
	if ( raw < 0xFFFF )
		cKd = fixedintb1002float(raw);
	else
		cKd = 0.05;
	raw = EEPROMReaduint16 (EEPROM_N75_PID_cKi);
	if ( raw < 0xFFFF )
		cKi = fixedintb1002float(raw);
	else
		cKi = 0.25;

	raw = EEPROMReaduint16 (EEPROM_N75_PID_aAT);
	if ( raw < 0xFFFF )
		apidActivationThresholdFactor = fixedintb1002float(raw);
	else
		apidActivationThresholdFactor = 0.5;
	raw = EEPROMReaduint16 (EEPROM_N75_PID_cAT);
	if ( raw < 0xFFFF )
		cpidActivationThresholdFactor = fixedintb1002float(raw);
	else
		cpidActivationThresholdFactor = 0.85;

	uint8_t b = EEPROM.read(EEPROM_N75_ENABLE_PID);
	if (b==0 || b==255)
		usePID = false;
	else
		usePID = true;

	uint16_t t2 = EEPROMReaduint16(EEPROM_N75_MAX_BOOST);
	if ( t2 < 0xFFFF )
		n75_max_boost = fixedintb1002float(t2);
	else
		n75_max_boost = 1.6;
}

void RPMBoostController::writeParamsToEEprom () {
	EEPROMWriteuint16 (EEPROM_N75_PID_aKp, float2fixedintb100(aKp) );
	EEPROMWriteuint16 (EEPROM_N75_PID_aKd, float2fixedintb100(aKd) );
	EEPROMWriteuint16 (EEPROM_N75_PID_aKi, float2fixedintb100(aKi) );
	EEPROMWriteuint16 (EEPROM_N75_PID_cKp, float2fixedintb100(cKp) );
	EEPROMWriteuint16 (EEPROM_N75_PID_cKd, float2fixedintb100(cKd) );
	EEPROMWriteuint16 (EEPROM_N75_PID_cKi, float2fixedintb100(cKi) );
	EEPROMWriteuint16 (EEPROM_N75_PID_aAT, float2fixedintb100(apidActivationThresholdFactor) );
	EEPROMWriteuint16 (EEPROM_N75_PID_cAT, float2fixedintb100(cpidActivationThresholdFactor) );
	if ( usePID )
		EEPROM.write(EEPROM_N75_ENABLE_PID, 1);
	else
		EEPROM.write(EEPROM_N75_ENABLE_PID, 0);
}

void RPMBoostController::writeMapsToEEprom () {
	for ( uint8_t i = 0; i < GEARS ; i++ ) {
		highboost_duty_cycle[i]->writeToEeprom( EEPROM_N75_HIGH_DUTY_CYCLE_MAPS + i*16 );
		highboost_pid_boost[i]->writeToEeprom( EEPROM_N75_PID_HIGH_SETPOINT_MAPS + i*32 );
		lowboost_duty_cycle[i]->writeToEeprom( EEPROM_N75_LOW_DUTY_CYCLE_MAPS + i*16 );
		lowboost_pid_boost[i]->writeToEeprom( EEPROM_N75_PID_LOW_SETPOINT_MAPS + i*32);
	}
}

void RPMBoostController::setN75Params (uint16_t *data) {
	aKp = fixedintb1002float ( *data );
	++data;
	aKi = fixedintb1002float ( *data );
	++data;
	aKd = fixedintb1002float ( *data );
	++data;
	cKp = fixedintb1002float ( *data );
	++data;
	cKi = fixedintb1002float ( *data );
	++data;
	cKd = fixedintb1002float ( *data );
	++data;
	apidActivationThresholdFactor = fixedintb1002float ( *data );
	++data;
	cpidActivationThresholdFactor = fixedintb1002float ( *data );
	++data;
	if ( ((uint8_t) (*data)) & 1 )
		usePID = true;
	else
		usePID = false;
	uint8_t* t = (uint8_t*) data;
	++t;
	data = (uint16_t*) t;
	n75_max_boost = fixedintb1002float ( *data );
}

void RPMBoostController::serialSendN75Params (uint8_t serial) {
	//STX tag=21 serial aKp aKi aKd cKp cKi cKd aAT cAT (16bit fixed uint16 base 100) flags (uint8 bit0=pid enable) ETX

	Serial.print("\2");
	uint8_t outbuf = SERIALOUT_BINARY_TAG_N75_PARAMS;
	Serial.write ( (uint8_t*) &(outbuf), sizeof(uint8_t) );
	Serial.write ( (uint8_t*) &(serial), sizeof(uint8_t) );
	uint16_t outbuf16 = float2fixedintb100(aKp);
	Serial.write ( (uint8_t*) &outbuf16, sizeof(uint16_t) );
	outbuf16 = float2fixedintb100(aKi);
	Serial.write ( (uint8_t*) &outbuf16, sizeof(uint16_t) );
	outbuf16 = float2fixedintb100(aKd);
	Serial.write ( (uint8_t*) &outbuf16, sizeof(uint16_t) );

	outbuf16 = float2fixedintb100(cKp);
	Serial.write ( (uint8_t*) &outbuf16, sizeof(uint16_t) );
	outbuf16 = float2fixedintb100(cKi);
	Serial.write ( (uint8_t*) &outbuf16, sizeof(uint16_t) );
	outbuf16 = float2fixedintb100(cKd);
	Serial.write ( (uint8_t*) &outbuf16, sizeof(uint16_t) );

	outbuf16 = float2fixedintb100(apidActivationThresholdFactor);
	Serial.write ( (uint8_t*) &outbuf16, sizeof(uint16_t) );
	outbuf16 = float2fixedintb100(cpidActivationThresholdFactor);
	Serial.write ( (uint8_t*) &outbuf16, sizeof(uint16_t) );

	outbuf = 0;
	if ( usePID )
		outbuf |= 1;
	Serial.write ( (uint8_t*) &outbuf, sizeof(uint8_t) );

	outbuf16 = float2fixedintb100(n75_max_boost);
	Serial.write ( (uint8_t*) &outbuf16, sizeof(uint16_t) );

	Serial.print("\3");

}
