/*
    Copyright 2009-14 Stephan Martin, Dominik Gummel

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

 	MultiDisplayDefinesDigifant.h
    Created on: 31.07.2014

 */

#ifndef MULTIDISPLAYDEFINESDIGIFANT_H_
#define MULTIDISPLAYDEFINESDIGIFANT_H_


/* ECU config file for Digifant 1 ecu */

#define DIGIFANT_KLINE
#define DIGIFANT_DK_POTI

/* ===============  Boost map sensor configuration ==================== */
#define USE_DIGIFANT_MAPSENSOR
//#define BOOST_PLX_SMVACBOOST
//#define BOOST_MOTOROLA_MPX4250
//#define BOOST_FREESCALE_MPXA6400A
//on boostpin2
//#define BOOST_BOSCH_200KPA

/* ===============  Lambda ==================== */
#define LAMBDA_WIDEBAND
#define LAMBDA_PLX_SMAFR
//#define LAMBDA_INNOVATE_LC1
//#define LAMBDASTANDALONE A2

/* ===============  RPM ==================== */
//choose only 1 of them
//#define USE_RPM_ONBOARD
#define USE_DIGIFANT_RPM
//20K
//#define RPMFACTOR 5.21
//factor for 75K 4 cyl rpm signal
#define RPMFACTOR 2.34              //Calibration for RPM (its 2.34!)

/* ===============  Speed ==================== */
#define SPEEDFACTOR 0.38
#define SPEEDCORRECTIONFACTOR 0.9
//#define SPEEDFACTOR 0.390625
//#define SPEEDCORRECTIONFACTOR 1

/* ===============  Type K EGT ==================== */
#define NUMBER_OF_ATTACHED_TYPE_K 5   //how many TypK`s are attached



#endif /* MULTIDISPLAYDEFINESDIGIFANT_H_ */
