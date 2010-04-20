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


	for ( int i = 0 ; i < 4 ; i++ ) {
		maxAgtValE[i] = 0;
		maxLdE[i] = 0;
		maxRpmE[i] = 0;
		maxLmmE[i] = 0;
		maxOilE[i] = 0;
	}


	//rpm Smoothing init:
	for (uint8_t i = 0; i < RPMSMOOTH; i++) {
		rpmReadings[i] = 0;                      // initialize all the readings to 0
	}

	//TypK Array init:
	for (uint8_t i = 0; i < NUMTYPK ; i++) 	{
		calAgt[i] = 0;                      // initialize all the readings to 0
	}


}
