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
#include"UserInterface.h"
#include"SensorData.h"

class MultidisplayController {
public:
	MultidisplayController();
	virtual ~MultidisplayController();

	uint8_t SerOut;
    uint8_t DoCal;
    uint8_t DoTypK;
    uint8_t DoCheck;


	void mainLoop();
	void ChangeSerOut();
	void CalibrateLD();
	void HeaderPrint();
    unsigned long getTime() const
    {
        return time;
    }

    unsigned long getScreenSave() const
    {
        return ScreenSave;
    }

    void setScreenSave(unsigned long  ScreenSave)
    {
        this->ScreenSave = ScreenSave;
    }

private:
    static const unsigned int PROGMEM tempTypK[];
    static const unsigned int PROGMEM tempVDOPressure[];
    static const unsigned int PROGMEM tempVDOTemp[];
    uint8_t IOport2;
    TwoWire wire;
    UserInterface ui;
    SensorData sdata;
    unsigned long FlashETimeU;
    unsigned long FlashTimeU;
    unsigned long ScreenSave;
    unsigned long time;
    int val3;



	int read_adc(uint8_t channel);
	void expanderWrite(byte _data);
	byte expanderRead();
	void expanderWrite2(byte _data);
	byte expanderRead2();
	void AnaConversion();
	void Shiftlight();

	void SerialPrint();

	int GetTypKTemp(unsigned int microVolts);
	int GetVDOTemp(unsigned int ADWreading);
	int GetVDOPressure(unsigned int ADWreading);
	void FetchTypK();
	void CheckLimits();
	void SaveMax(int Num);
};

#endif /* MULTIDISPLAYCONTROLLER_H_ */
