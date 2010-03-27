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

#ifndef MULTIDISPLAYCONTROLLER_H_
#define MULTIDISPLAYCONTROLLER_H_

#include"MultidisplayDefines.h"
#include "util.h"

#include<Wire.h>
#include <avr/pgmspace.h>
#include"SensorData.h"
#include <LCD/LCDController.h>

class PID;

/**
 * taken from the PID lib...
 *
 * This Data structure lets us take the byte array
 * sent from processing and easily convert it to a float array
 *
 * getting float values from processing into the arduino
 * was no small task.  the way this program does it is
 * as follows:
 *  * a float takes up 4 bytes.  in processing, convert
 *    the array of floats we want to send, into an array
 *    of bytes.
 *  * send the bytes to the arduino
 *  * use a data structure known as a union to convert
 *    the array of bytes back into an array of floats
 *
 *  the bytes coming from the arduino follow the following
 *  format:
 *  0: 0=Manual, 1=Auto, 2=multidisplay command, else = ? error ?
 *  for pid lib
 *  1-4: float setpoint
 *  5-8: float input
 *  9-12: float output
 *  13-16: float P_Param
 *  17-20: float I_Param
 *  21-24: float D_Param
 *
 *  multidisplay command
 *  1: buttons: 1=a pressed, 2=a hold, 3=b pressed, 4=b hold
 *
 *  attention, you have to send ints, not chars over the serial line!
 */


class MultidisplayController {
public:
	MultidisplayController();

	uint8_t SerOut;
    uint8_t DoCal;
    uint8_t DoTypK;
    uint8_t DoCheck;
    unsigned long ScreenSave;
    unsigned long time;

    SensorData data;
    LCDController lcdController;
    LCDController *lcdControllerP;

	void mainLoop();
	void ChangeSerOut();
	void CalibrateLD();
	void HeaderPrint();



private:
    static const unsigned int PROGMEM tempTypK[];
    static const unsigned int PROGMEM tempVDOPressure[];
    static const unsigned int PROGMEM tempVDOTemp[];
    uint8_t IOport2;
    TwoWire wire;
    unsigned long FlashETimeU;
    unsigned long FlashTimeU;

    unsigned long serialTime;
    union {
      byte asBytes[24];
      float asFloat[6];
    } srData;

    //PID controller for the boost
    PID *boostPidP;

//    int val3;

	int read_adc(uint8_t channel);
	void expanderWrite(byte _data);
	byte expanderRead();
	void expanderWrite2(byte _data);
	byte expanderRead2();
	void AnaConversion();
	void Shiftlight();

	void serialSend();
	void serialReceive();

	int GetTypKTemp(unsigned int microVolts);
	int GetVDOTemp(unsigned int ADWreading);
	int GetVDOPressure(unsigned int ADWreading);
	void FetchTypK();
	void CheckLimits();
	void SaveMax(uint8_t Num);

	//button related functions
	unsigned long buttonTime;   //Button time

	//TODO documentation!
	void buttonCheck(int buttonState);
	void buttonAHold();
	void buttonAPressed();
	void buttonBHold();
	void buttonBPressed();
};

extern MultidisplayController *mControllerP;

#endif /* MULTIDISPLAYCONTROLLER_H_ */
