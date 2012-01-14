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

#include "SensorData.h"
#include <LCDController.h>
#include <EEPROM.h>
#include <util.h>
#include <stdint.h>
#include <wiring.h>

SensorData::SensorData () {
}


void SensorData::myconstructor () {
//	IOport2 = 0b11111111;

	for ( uint8_t i = 0 ; i <= 16 ; i++ )
		anaIn[i] = 0;

//	FlashETimeU = 0;
//	FlashTimeU = 0;
//	ScreenSave = 0;
//	time = 0;
//	val3=0;
	boostAmbientPressureBar = 1.0;

	maxLd = 0;
	maxLdt=0;               //max LD for the screen
	ldCalPoint = 0;

	rpmIndex = 0;                            // the index of the current reading
	rpmTotal = 0;                            // the running total
	rpmAverage = 0;                          // the average

	calBoost = 0.0;
	calAbsoluteBoost = 0.0;
	calLambda = 0;
	calLambdaF = 0;
	calRPM = 0;
	rpm_map_idx = 0;
	calThrottle = 0;
	calCaseTemp = 0.0;
	calLMM = 0.0;
	VDOTemp1 = 0;
	VDOTemp2 = 0;
	VDOTemp3 = 0;
	VDOPres1 = 0;
	VDOPres2 = 0;
	VDOPres3 = 0;
	batVolt = 0.0;

	//rpm Smoothing init:
	for (uint8_t i = 0; i < RPMSMOOTH; i++) {
		rpmReadings[i] = 0;                      // initialize all the readings to 0
	}

	//TypK Array init:
	for (uint8_t i = 0; i < NUMBER_OF_ATTACHED_TYPK ; i++) 	{
		calEgt[i] = 0;
	}

#ifdef MULTIDISPLAY_V2
	speed = 0;
	gear = 5;

	df_cyl1_retard = 0;
	df_cyl2_retard = 0;
	df_cyl3_retard = 0;
	df_cyl4_retard = 0;
	df_total_retard = 0;
	df_ignition = 0;

	df_boost = 0;
	df_ect = 0;
	df_iat = 0;
	df_rpm = 0;

	speedIndex = 0;
	speedTotal = 0;
	speedAverage = 0;
	//speed Smoothing init:
	for (uint8_t i = 0; i < SPEEDSMOOTH; i++) {
		speedReadings[i] = 0;                      // initialize all the readings to 0
	}

#endif
}

void SensorData::generate_debugData () {
	calRPM=1;
	calAbsoluteBoost=2;
	calThrottle=3;
	calLambdaF=4;
	calLMM=5;
	calCaseTemp=6;

#ifdef TYPE_K
	for ( uint8_t i = 0 ; i < NUMBER_OF_ATTACHED_TYPK ; i++)
		calEgt[i] = 88;
#endif

	batVolt=7;
	VDOPres1=8;
	VDOPres2=9;
	VDOPres3=10;
	VDOTemp1=11;
	VDOTemp2=12;
	VDOTemp3=13;
	speed=14;
	gear=15;

	df_ignition = 6.0;
	df_total_retard = 3;
}

void SensorData::saveMax(uint8_t maxEv) {
	maxEv = constrain (maxEv, 0, MAXVALUES-1);

	maxValues[maxEv].boost = calBoost;
	maxValues[maxEv].lmm = calLMM;
	maxValues[maxEv].lambda = calLambda;
	maxValues[maxEv].rpm = calRPM;
	maxValues[maxEv].speed = speed;
	//FIXME change VDO storage to float and array!
	maxValues[maxEv].oilpres = VDOPres1;
	maxValues[maxEv].gaspres = VDOPres2;
	maxValues[maxEv].gear = gear;
	for ( uint8_t i = 0 ; i < NUMBER_OF_ATTACHED_TYPK ; i++ )
		maxValues[maxEv].egt[i] = calEgt[i];
}

void SensorData::checkAndSaveMaxEgt () {
	//we're saving if me have a new max value over all channels!
	uint16_t maxCur = 0;
	uint16_t maxSaved = 0;
	for ( uint8_t i = 0 ; i < NUMBER_OF_ATTACHED_TYPK ; i++ ) {
		if ( calEgt[i] > maxCur )
			maxCur = calEgt[i];
		if ( maxValues[MAXVAL_EGT].egt[i] > maxSaved )
			maxSaved = maxValues[MAXVAL_EGT].egt[i];
	}
	if ( maxCur >= maxSaved )
		saveMax (MAXVAL_EGT);
}
