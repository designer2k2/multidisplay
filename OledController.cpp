/*
    Copyright 2009-13 Stephan Martin, Dominik Gummel

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


#include "OledController.h"
#include "MultidisplayDefines.h"

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

