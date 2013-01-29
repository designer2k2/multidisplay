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

class OledController {
public:
	OledController();
	OledController( ProxySerial ps );
	virtual ~OledController();

	void init ();
	void draw ();

	void toggleScreen ();
	void setActiveScreen (uint8_t as);

	uint8_t activeScreen;
	uint8_t brightness;

	Serial_LCD display;
};

#endif /* OLEDCONTROLLER_H_ */
