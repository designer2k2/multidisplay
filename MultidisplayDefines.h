/*w
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

#ifndef MULTIDISPLAY_DEFINES_H_
#define MULTIDISPLAY_DEFINES_H_

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define MULTIDISPLAY_V2
#else
#define MULTIDISPLAY_V1
#endif


//choose your engines ecu
//only one of them should be defined!
//#define VR6_MOTRONIC
#define DIGIFANT
#define DIGIFANT_KLINE

#define LAMBDA_WIDEBAND

//define this if you measure the boost with SM-VAC/Boost instead of Motorola MPX4250AP / Bosch 200kpa
//#define BOOST_PLX_SMVACBOOST
#define BOOST_MOTOROLA_MPX4250
//on boostpin2
//#define BOOST_BOSCH_200KPA

#define GEARS 6

#define BOOSTN75
//#define BOOSTPID
#define BOOST_NORMAL 1.0
#define BOOST_RACE 1.5
#define N75_MANUALDUTY_NORMAL 96
#define N75_MANUALDUTY_RACE 230
#define BOOST_MODE_NORMAL 1
#define BOOST_MODE_RACE 0
#define RPM_MAX_FOR_BOOST_CONTROL 7000

#define READFROMEEPROM

#define TYPE_K

//----------------------------------------------------------------------------------------------------
//Max Values:
#define MaxAGT 1200            //Maximum AGT
#define MaxOP 8                //Maximum Oilpressure
//----------------------------------------------------------------------------------------------------

//Options:

#define V2DEVDEBUG 1

//#define DEBUG 1
//#define LCDTEST 1
//#define RPM_DEBUG 0             //Little Frequency Generator
//#define FREEMEM 0              //Outputs how much free RAM is

#define INITTIME 2000          //How long the Init screen will be shown
//--------------------------------------

// Shiftlight Config:

#define RPM_SHIFT_LIGHT
#define RPM_SHIFT_LOW 6000             //Shiftlight half brightness
#define RPM_LOW_BRIGHT 40
#define RPM_SHIFT_HIGH 6500            //Shiftlight full brightness
#define RPM_HIGH_BRIGHT 255
#define RPM_NO_BRIGHT 0                //Shiftlight normal brightness

//--------------------------------------

#define BUTTONHOLD 1000         //ms to trigger a Hold (Every Xms a hold is triggered)
#define SCREENSAVEDELAY 10000	//ms after what it will save the Screen

#define FLASH_TIME 100            //ms the interval when a Limit is exceeded, the LCD will flash with this interval)
#define FLASH_E_TIME 5000          //how long the flash is shown at a minimum

//--------------------------------------
//Pin Config:
//arduino
#ifdef MULTIDISPLAY_V2
#define DATAOUT 57              //MOSI  17 on UNO/Duemilanove, 57 on MEGA
#define DATAIN 12               //MISO
//#define SPICLOCK 23             //Clock  13 on UNO/Duemilanove, 23 on MEGA (13 cant bring the power!) Just bridge 23 and 13 with jumperwire.
#define SPICLOCK 26             //Clock  26 on the V2 pcb

//V2 Buttons - pull high
#define V2_BUTTON1 22
#define V2_BUTTON2 23
#define V2_BUTTON3 24
#define V2_BUTTON4 25
#endif

#ifdef MULTIDISPLAY_V1
#define DATAOUT 17              //MOSI  17 on UNO/Duemilanove, 57 on MEGA
#define DATAIN 12               //MISO
#define SPICLOCK 13             //Clock  13 on UNO/Duemilanove, 23 on MEGA (13 cant bring the power!) Just bridge 23 and 13 with jumperwire.
#endif


#define BATTERYPIN 0           //The Signal from the voltage divider (Analog Pin on the Arduino!)
#define LCDBRIGHTPIN 5		     //The Pin for the Brightness of the LCD, it will be PWM.
#define RPMSHIFTLIGHTPIN 3           //where is the Shiftlight connected

/*
 * K10 2xPWM (arduino pin 10 und 11):
 * n75 (out) (right)
 * free (warn led) (left)
 *
 * K11 2xanalog (arduino pin 1 und 2)
 * 2xdigital: 15,16
 * nordschleifen switch (switch must be connected to ground!) (left)
 * free (lambda relais) (right)
 */
#define NORDSCHLEIFENPIN 15
#define FREEANALOG2 16
//left of lcd pins
#define FREEPWM2 10
#define N75PIN 11

// The Sensors: (On the MD03 Print, see the Layout for the KLX Connectors)
#define BOOSTPIN 1             //(KL7-2) Currently supports the Motorola 250kpa Sensor
#define BOOST2PIN 2            //(KL7-3) Currently supports the Motorola 250kpa Sensor

#define VDOT1PIN 3             //(KL6) T1 VDO Temperature Pin
#define VDOT2PIN 4             //(KL5) T2 VDO Temperature Pin
#define VDOT3PIN 5             //(KL4) T3 VDO Temperature Pin
#define VDOP1PIN 6             //(KL3) P1 VD0 Pressure Pin
#define VDOP2PIN 7             //(KL2) P1 VD0 Pressure Pin
#define VDOP3PIN 8             //(KL1) P1 VD0 Pressure Pin

#define LAMBDAPIN 11           //(KL9-4) Narrow Band Lambda Sensor Input
#define THROTTLEPIN 12         //(KL9-3) 0-5V Signal from the trothle signal
#define RPMPIN 13              //(KL9-2) needs a small Circuit to converts the RPM Signal into Analog Voltages
#define LMMPIN 14              //(KL9-1)Signal from the LMM


#define CASETEMPPIN 15         //The LM35 Signal.
#define AGTPIN 16              //Input from the Mux


//--------------------------------------
//Others:

#define LAMBDAMIN 100            //Calibration for Lambda
#define LAMBDAMAX 1100
#define LAMBDALOWERLIMIT 3
#define LAMBDAUPPERLIMIT 15

#define THROTTLEMIN 620            //Calibration for Throttle
#define THROTTLEMAX 3530

#define RPMFACTOR 2.34              //Calibration for RPM (its 2.34!)
#define RPMSMOOTH 5                 //how many readings will be taken for the smoothing (5 is quick and smooth, 10 would be supersmooth but its getting slow)
#define RPMMAX 3400                 //its about 8k

#define TEMPTYPKREADINGS 28         //how many entrys are in the Lookup Table
#define NUMBER_OF_ATTACHED_TYPK 4                   //how many TypK`s are attached
#define MAX_ATTACHED_TYPK 8			//max typk which can be attached
#define MAXTYPK 1170                //over that will be seen as Open.


#define EXPANDER B0100001       // Address with 2 address pins grounded. (from the PCF8574)
#define EXPANDER2 B0100000      // Address with three address pins grounded. (from the PCF8574)

#define DEBUGRPM 10

//temp hack
typedef unsigned char byte;

//serial output
#define SERIALOUT_DISABLED 0
#define SERIALOUT_BINARY 1
#define SERIALOUT_ENABLED 2
#define SERIALOUT_TUNERPRO_ADX 3

//84 bytes
#define SERIALOUT_BINARY_TAG 84

//send and receive each SERIALFREQ millisecs!
#define SERIALFREQ 10
//#define SERIALFREQ 800


#define SCREENA 1
#define SCREENB 2

#define SCOPE_BOOST 1
#define SCOPE_LMM 2
#define SCOPE_RPM 3
#define SCOPE_THROTTLE 4
#define SCOPE_LAMBDA 5

//buffer in LCDController
#define LCD_BUFSIZE 21

#define LCD_WIDTH 20
#define LCD_HEIGHT 4
#define LCD_BIGFONT_2 2
#define LCD_BIGFONT_4 4

#define BAR2PSI 14.50377

/**
 * EEPROM Layout:
 *
 * Start Byte | type | name
 * 0   | byte | n75 manual normal
 * 1   | byte | n75 manual race
 * 100 | byte | activeScreen
 * 105 | byte | brightness
 * 200 | double | calLD
 * 205 | byte | ldCalPoint
 */
#define EEPROM_N75_MANUALDUTY_NORMAL 0
#define EEPROM_N75_MANUALDUTY_RACE 1
#define EEPROM_ACTIVESCREEN 100
#define EEPROM_BRIGHTNESS 105
#define EEPROM_AMBIENTPRESSURE 200
#define EEPROM_LDCALPOINT 205


#define DF_KLINESERIALFREQ 1
#define DF_KLINEFRAMESIZE 34 //including stx / etx bytes
#define DF_KLINE_STORE_FRAME_COUNT 2
#define DF_KLINEBEGIN 2
#define DF_KLINEEND 3
#define DF_KLINE_STATUS_RECEIVING 1
#define DF_KLINE_STATUS_FRAME_COMPLETE 2
#define DF_KLINE_STATUS_FRAMEERROR 3


#endif /* MULTIDISPLAY_DEFINES_H_ */
