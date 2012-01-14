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


#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <stdint.h>
#include "MultidisplayDefines.h"

#define MAXVALUES 8
#define MAXVAL_BOOST 0
#define MAXVAL_LMM 1
#define MAXVAL_LAMBDA 2
#define MAXVAL_RPM 3
#define MAXVAL_SPEED 4
#define MAXVAL_OILPRES 5
#define MAXVAL_GASPRES 6
#define MAXVAL_EGT 7

class MaxDataSet {
public:
	MaxDataSet () : boost(0), lmm(0), lambda(0), rpm(0), speed(0), oilpres(0), gaspres(0), gear(0) {};
	float boost;
	float lmm;
	float lambda;
	uint16_t rpm;
	uint16_t speed;
	float oilpres;
	float gaspres;
	uint8_t gear;
	uint16_t egt[NUMBER_OF_ATTACHED_TYPK];
};

class SensorData {
public:
	int anaIn[17];
	float boostAmbientPressureBar;          //calibration for Boost: ambient pressur in bar
	float maxLd;
	int maxLdt;               //max LD for the screen
	int ldCalPoint;             //where to draw the dot from the calibration


	uint16_t calEgt[NUMBER_OF_ATTACHED_TYPK];  //All Typ K Values will be stored inside this array

	//RPM Smoothing:
	int rpmReadings[RPMSMOOTH];                // the readings from the analog input
	int rpmIndex;                            // the index of the current reading
	int rpmTotal;                            // the running total
	int rpmAverage;                          // the average

	//convertet values:
	float calBoost; //relative boost
	float calAbsoluteBoost;
//	float calBoostPSI; //for PID
	float calLambdaF;
	int calLambda;
	int calRPM;
	uint8_t rpm_map_idx;
	int calThrottle;
	float calCaseTemp;
	float calLMM;
	int VDOTemp1;
	int VDOTemp2;
	int VDOTemp3;
	int VDOPres1;
	int VDOPres2;
	int VDOPres3;
	float batVolt;

	//new V2 style
	MaxDataSet maxValues[MAXVALUES];


#ifdef MULTIDISPLAY_V2
	uint16_t speed;
	uint8_t gear;

	int speedReadings[SPEEDSMOOTH];                // the readings from the analog input
	uint8_t speedIndex;                            // the index of the current reading
	int speedTotal;                            // the running total
	int speedAverage;

	/*
	 * Digifant
	 * 4 x knock cyl 1-4 retard
	 * 1 x knock total retard
	 * 1 x ignition advance
	 */
	float df_cyl1_retard;
	float df_cyl2_retard;
	float df_cyl3_retard;
	float df_cyl4_retard;
	float df_total_retard;
	float df_ignition;

	float df_boost;
	float df_ect;
	float df_iat;
	int df_rpm;

#endif

	SensorData();

	//dont use the constructor because I want to control when the global objects get initialized
	void myconstructor();

	void generate_debugData();

	void saveMax(uint8_t maxEv);
	void checkAndSaveMaxEgt ();

};

extern SensorData data;

#endif
