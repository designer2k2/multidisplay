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

#define USE_DIGIFANT_MAPSENSOR
//#define BOOST_PLX_SMVACBOOST
//#define BOOST_MOTOROLA_MPX4250
//#define BOOST_FREESCALE_MPXA6400A
//on boostpin2
//#define BOOST_BOSCH_200KPA

#define GEARS 6
#define GEAR_RECOGNITION
//215 40 R16
#define ABROLLUMFANG 1.764
//195 50 R15
//#define ABROLLUMFANG 1.757

#define BOOSTN75
#define BOOST_NORMAL 1.0
#define BOOST_RACE 1.5
#define N75_MANUALDUTY_NORMAL 96
#define N75_MANUALDUTY_RACE 230
#define BOOST_MODE_NORMAL 1
#define BOOST_MODE_RACE 0
#define RPM_MIN_FOR_BOOST_CONTROL 0
#define RPM_MAX_FOR_BOOST_CONTROL 9000
#define BOOST_EFR_SPEEDLIMIT_PROTECTION

#define READFROMEEPROM

#define TYPE_K

#define BW_EFR_SPEEDSENSOR
//Port PL0 / Timer 4
#define BW_EFR_SPEEDSENSOR_PIN 49
#define BW_EFR_SMOOTH 5
//for 12 wheel blades (67mm)
#define BW_EFR_TIME_2_RPM 40000000
//14 wheel blades
//#define BW_EFR_TIME_2_RPM 34285712


//Options:

#define V2DEVDEBUG 1
//#define BLUETOOTH_ON_SERIAL2 1
//#define BLUETOOTH_SETUP_ON_SERIAL2

//#define DEBUG 1
//#define LCDTEST 1
//#define RPM_DEBUG 0             //Little Frequency Generator
//#define FREEMEM 0              //Outputs how much free RAM is

#define INITTIME 2000          //How long the Init screen will be shown
//--------------------------------------

// V1 Shiftlight Config:
//#define V1_RPM_SHIFT_LIGHT
#define V1_RPM_SHIFT_LOW 6000             //Shiftlight half brightness
#define V1_RPM_LOW_BRIGHT 40
#define V1_RPM_SHIFT_HIGH 6500            //Shiftlight full brightness
#define V1_RPM_HIGH_BRIGHT 255
#define V1_RPM_NO_BRIGHT 0                //Shiftlight normal brightness

//--------------------------------------
// V2 RGB LED on Shiftlight 1-3 RGB
#define V2_RGB_WARNLED_ON_SHIFTLED123
#define V2_RGB_WARNLED_EFR_SPEED_REDLINE 120000
#define V2_SHIFTLED1PIN 3
#define V2_SHIFTLED2PIN 38
#define V2_SHIFTLED3PIN 39

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
#define V1_RPMSHIFTLIGHTPIN 3           //where is the Shiftlight connected

/*
 * K10 2xPWM (arduino pin 10 und 11):
 * n75 (out) (right)
 * free (warn led) (left)
 *
 * K11 2xanalog (arduino pin A1 und A2)
 * 2xdigital: 15,16
 * nordschleifen switch (switch must be connected to ground!) (left)
 * oled reset (right)
 */
#ifdef MULTIDISPLAY_V1
#define NORDSCHLEIFENPIN 15
#else
#define NORDSCHLEIFENPIN A1
#endif

#define LAMBDA_WIDEBAND
#define LAMBDA_PLX_SMAFR
//#define LAMBDA_PLX_INNOVATE_LC1
//#define LAMBDASTANDALONE A2

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

//choose only 1 of them
//#define USE_RPM_ONBOARD
#define USE_DIGIFANT_RPM
#define RPMFACTOR 2.34              //Calibration for RPM (its 2.34!)
#define RPMSMOOTH 5                 //how many readings will be taken for the smoothing (5 is quick and smooth, 10 would be supersmooth but its getting slow)
#define RPMMAX 3400                 //its about 8k

//#define SPEEDFACTOR 0.390625
#define SPEEDFACTOR 0.38
#define SPEEDCORRECTIONFACTOR 0.9
#define SPEEDSMOOTH 5
#define SPEEDPIN 4 //analog 4

#define TEMPTYPEKREADINGS 28         //how many entrys are in the Lookup Table
#define NUMBER_OF_ATTACHED_TYPE_K 5   //how many TypK`s are attached
#define MAX_ATTACHED_TYPE_K 8			//max typk which can be attached
#define MAX_TYPE_K 1200                //over that will be seen as Open.


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

#define SERIALOUT_BINARY_TAG 95
//#define SERIALOUT_BINARY_TAG 92
//#define SERIALOUT_BINARY_TAG 88
#define SERIALOUT_BINARY_TAG_N75_DUTY_MAP 22
#define SERIALOUT_BINARY_TAG_N75_SETPOINT_MAP 38
#define SERIALOUT_BINARY_TAG_ACK 4
#define SERIALOUT_BINARY_TAG_N75_PARAMS 23
#define SERIALOUT_BINARY_TAG_GEAR_RATIO_6G 17

//send and receive each SERIALFREQ millisecs!
#define SERIALFREQ 100
//#define SERIALFREQ 50
//#define SERIALFREQ 800

//#define OLED
#define OLED_RESET_PIN 53

#define LCD
#define LCD_DRAW_TIME 300

//buffer in LCDController
#define LCD_BUFSIZE 21

#define LCD_WIDTH 20
#define LCD_HEIGHT 4
#define LCD_BIGFONT_2 2
#define LCD_BIGFONT_4 4

#define SCREENA 1
#define SCREENB 2

#define SCOPE_BOOST 1
#define SCOPE_LMM 2
#define SCOPE_RPM 3
#define SCOPE_THROTTLE 4
#define SCOPE_LAMBDA 5


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
 * 3511 | word | 6 n75 boost low setpoint maps for pid (16x1 maps)
 * 3707 | word | 6 n75 boost high setpoint maps for pid (16x1 maps)
 * 3903 | byte | 6 n75 low duty cycle maps (16x1 maps)
 * 3999 | byte | 6 n75 high duty cycle maps (16x1 maps)
 */
#define EEPROM_N75_MANUALDUTY_NORMAL 0
#define EEPROM_N75_MANUALDUTY_RACE 1
#define EEPROM_N75_PID_aKp 2
#define EEPROM_N75_PID_aKd 4
#define EEPROM_N75_PID_aKi 6
#define EEPROM_N75_PID_cKp 8
#define EEPROM_N75_PID_cKd 10
#define EEPROM_N75_PID_cKi 12
#define EEPROM_N75_PID_aAT 14
#define EEPROM_N75_PID_cAT 16
#define EEPROM_N75_ENABLE_PID 18
//16bit
#define EEPROM_N75_MAX_BOOST 19

//6 x 16bit fixed int base 1000
#define EEPROM_GEAR_RATIO_MAP_START 25

//1 byte
#define EEPROM_ACTIVESCREEN 100
//2 bytes
#define EEPROM_SERIALFREQ 101
//1 byte
#define EEPROM_BRIGHTNESS 105

#define EEPROM_AMBIENTPRESSURE 200
#define EEPROM_LDCALPOINT 205
#define EEPROM_N75_PID_LOW_SETPOINT_MAPS 3511
#define EEPROM_N75_PID_HIGH_SETPOINT_MAPS 3707
#define EEPROM_N75_LOW_DUTY_CYCLE_MAPS 3903
#define EEPROM_N75_HIGH_DUTY_CYCLE_MAPS 3999

#define DF_KLINESERIALFREQ 1
#define DF_KLINEFRAMESIZE 34 //including stx / etx bytes
#define DF_KLINE_STORE_FRAME_COUNT 2
#define DF_KLINEBEGIN 2
#define DF_KLINEEND 3
#define DF_KLINE_STATUS_RECEIVING 1
#define DF_KLINE_STATUS_FRAME_COMPLETE 2
#define DF_KLINE_STATUS_FRAMEERROR 3




#endif /* MULTIDISPLAY_DEFINES_H_ */
