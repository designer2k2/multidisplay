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

#include "MultidisplayDefines.h"

class SensorData {
public:
	int anaIn[17];
	float calLd;          //calibration from the Boost
	float maxLd;
	int maxLdt;               //max LD for the screen
	int ldCalPoint;             //where to draw the dot from the calibration

	//	// PID Boost Controller
	double boostSetPoint;
	double boostOutput;

	int calAgt[NUMTYPK+1];  //All Typ K Values will be stored inside this array

	//RPM Smoothing:
	int rpmReadings[RPMSMOOTH];                // the readings from the analog input
	int rpmIndex;                            // the index of the current reading
	int rpmTotal;                            // the running total
	int rpmAverage;                          // the average

	//convertet values:
	float calBoost;
	float calRAWBoost;
	int calLambda;
	long calRPM;
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

	//Saving of the MAx Values: (0=AGT, 1=Boost, 2=RPM)
	unsigned int maxAgtValE[4];          //maximum AGT
	float maxLdE[4];
	long maxRpmE[4];            //maximum RPM
	float maxLmmE[4];         //maximum LMM
	int maxOilE[4];             //maximum Oilpressure

	SensorData();
};


extern SensorData data;

#endif
