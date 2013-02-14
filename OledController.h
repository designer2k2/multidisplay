/*
 * OledController.h
 *
 *  Created on: 29.01.2013
 *      Author: bofh
 */

#ifndef OLEDCONTROLLER_H_
#define OLEDCONTROLLER_H_

#include <libs/Serial_LCD/Serial_LCD.h>
#include <libs/Serial_LCD/proxySerial.h>

#include <libs/Goldelox_Serial_4DLib.h>

class OledController {
public:
	OledController();
	virtual ~OledController();
	void myconstructor();
	void reset();

	static void callback(int ErrCode, unsigned char ErrByte);

	void init ();
	void draw ();
	void sendDebugData();

	void toggleScreen ();
	void setActiveScreen (uint8_t as);

	uint8_t activeScreen;
	uint8_t brightness;

	ProxySerial *ps;
	Serial_LCD *display;
//	Goldelox_Serial_4DLib display;
};

extern Goldelox_Serial_4DLib display;

#endif /* OLEDCONTROLLER_H_ */
