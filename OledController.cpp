/*
 * OledController.cpp
 *
 *  Created on: 29.01.2013
 *      Author: bofh
 */

#include "OledController.h"
#include "MultidisplayDefines.h"

#include <libs/Serial_LCD/proxySerial.h>
#include <libs/Serial_LCD/Serial_LCD.h>
#include <Arduino.h>

Goldelox_Serial_4DLib display(&Serial3);

OledController::OledController()
{
//	display.TimeLimit4D   = 1000 ;
//	display.Callback4D =   &(OledController::callback) ; // NULL ;
//
//	//we have to start with 9600 baud
//	Serial3.begin(9600);


}

void OledController::reset () {
//	pinMode (OLED_RESET_PIN, OUTPUT);
//	digitalWrite(OLED_RESET_PIN, LOW);
//	delay(10);
//	digitalWrite(OLED_RESET_PIN, HIGH);
//	delay(10);
//	Serial3.flush();
}

void OledController::myconstructor () {
	ps = new ProxySerial(&Serial3);
	display = new Serial_LCD(ps);
	Serial3.begin(9600);
	display->begin(OLED_RESET_PIN);

	// Serial port speed up
	display->setSpeed(115200);
	Serial3.begin(115200);

	display->clear();
	display->tText(0,0,"Hello World");

	sendDebugData();

//	Serial3.print('U');    // connect
//	while (Serial3.read()!=0x06)  {
//		delay(100);
//	}
//
//	display.gfx_Cls() ;
////
//////  fmediatests = trymount() ;
//	char wks[20];
//	display.sys_GetModel(wks) ; // length is also returned, but we don't need that here
//	Serial.print(F("Display model: ")) ;
//	Serial.println(wks) ;
//	display.putstr("Model:\n") ;
//	display.putstr(wks) ;
//	int i = strlen(wks) ;
//	Serial.print(F("SPE2 Version: ")) ;
//	Serial.println(display.sys_GetVersion()) ;
//	Serial.print(F("PmmC Version: ")) ;
//	Serial.println(display.sys_GetPmmC()) ;
//
//	display.setDisplayBaud(BAUD_115200);
//	Serial.begin(115200);
//	display.GetAck();
//
//	delay(1000);
//	display.gfx_Cls();
//
//	display.txt_MoveCursor(0,0);
//	display.putstr("Hello World (1)");
//
//	display.txt_MoveCursor(0,2);
//	display.putstr("2");
//
////	//arduino pin connected to reset
////	display.begin(OLED_RESET_PIN);
////
////	delay(10);
////	  // === Serial port speed up
////	display.setSpeed(115200);
////	Serial3.begin(115200);
////
////	display.initSD();
////
////	display.setOrientation(0x03);
////	display.setPenSolid(true);
////	display.setFontSolid(true);
////	display.setFont(0);
////
////	display.clear();
////	display.gText(0,0,"1");
////	display.gText(0,64,"Hello World");



}

OledController::~OledController() {
}

void OledController::callback(int ErrCode, unsigned char ErrByte) {
	  const char *Error4DText[] = {"OK\0", "Timeout\0", "NAK\0", "Length\0", "Invalid\0"} ;
	  Serial.print(F("Serial 4D Library reports error ")) ;
	  Serial.print(Error4DText[ErrCode]) ;

	  if (ErrCode == Err4D_NAK)
	  {
		Serial.print(F(" returned data= ")) ;
		Serial.println(ErrByte) ;
	  }
	  else
		Serial.println(F("")) ;
}

void OledController::sendDebugData() {
	display->checkScreenType();
	display->checkControllerType();
	display->checkHardwareVersion();
	display->checkSoftwareVersion();
	Serial.print ( display->WhoAmI() );
}
