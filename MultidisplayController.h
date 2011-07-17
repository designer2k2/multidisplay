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
#include "LCD/LCDController.h"

class PID;



class MultidisplayController {
public:
	MultidisplayController();
	//dont use the constructor because I want to control when the global objects get initialized
	void myconstructor();

	uint8_t SerOut;
    uint8_t DoCal;
    uint8_t DoTypK;
    uint8_t DoCheck;
    unsigned long ScreenSave;
    unsigned long time;

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

    /**
     *  ============ pc -> arduino ==================
     *  the messages to the arduino follow the following format:
     *  0: 0=Manual, 1=Auto, 2=multidisplay command, else = ignore
     *
     *  for pid lib
     *  1-4: fixed point value (scale factor 1000) setpoint
     *  5-8: fixed point value (scale factor 1000) input
     *  9-12: fixed point value (scale factor 1000) output
     *  13-16: fixed point value (scale factor 1000) P_Param
     *  17-20: fixed point value (scale factor 1000) I_Param
     *  21-24: fixed point value (scale factor 1000) D_Param
     *
     *  multidisplay command (from pc to arduino)
     *  2: buttons: 1=a pressed, 2=a hold, 3=b pressed, 4=b hold
     *
     *  3: change serial mode: 2=enabled(string), 3=tunerpro adx request data
     *
     *  4: eeprom: 1=save settings, 2 calibrate boost (measure and store ambient pressure)
     *
     *  attention, you have to send ints, not chars over the serial line!
     *
     *  ============ arduino -> pc ==================
     *  pid messages from arduino to pc are prefixed with "PID " as chars!
     */
    union {
      byte asBytes[24];
      float asFloat[6];
      int32_t asFixedInt32[6];
    } srData;

	int read_adc(uint8_t channel);

	int read_adc_fast(uint8_t channel);
	int read_adc_fast_mega(uint8_t channel);

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
	void buttonCheck_V1(int buttonState);
	void buttonAHold();
	void buttonAPressed();
	void buttonBHold();
	void buttonBPressed();

	void readSettingsFromEeprom();
	void saveSettings2Eeprom();

#if defined(MULTIDISPLAY_V2) && defined(DIGIFANT_KLINE)

	void DFKlineSerialReceive();

	union {
	    	byte asBytes[DF_KLINEFRAMESIZE];
	    } df_klineData[DF_KLINE_STORE_FRAME_COUNT];
	unsigned long DF_KlineSerialTime;
	//! holds the index of the last frame (255 if no frame was received)
	uint8_t df_kline_last_frame_completely_received;
	uint16_t df_kline_freq_milliseconds;
	unsigned long df_kline_freq_helper0;
	uint8_t df_kline_active_frame;
	uint8_t df_kline_index;
	uint8_t df_kline_status;
	uint16_t df_kline_discarded_frames;

#endif

};


extern MultidisplayController mController;



#endif /* MULTIDISPLAYCONTROLLER_H_ */
