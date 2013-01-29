// 
// 4D Systems uLCD-uLED-uVGA Serial_LCD Library Suite
// Arduino 0023 and 1.0, chipKIT MPIDE 0023, Wiring 1.0
// ----------------------------------
//
//  Energy.cpp 
//  Library C++ code
//  ----------------------------------
//  Developed with embedXcode
//
// Sep 01, 2012 release 307
// See README.txt
//
// © Rei VILO, 2010-2013
//   CC = BY NC SA
//   http://embeddedcomputing.weebly.com/serial-lcd.html
//   http://github.com/rei-vilo/Serial_LCD
//
// For 
//   4D Systems Goldelox and Picaso SGC Command Set
//   http://www.4dsystems.com.au/
//
//

//// Core library - MCU-based
//#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega32U4__) || defined(__SAM3X8E__) // Arduino specific
//#if (ARDUINO>=100)
//#include "Arduino.h" // for Arduino 1.0
//#else
//#include "WProgram.h" // for Arduino 23
//#endif
//#elif defined(__32MX320F128H__) || defined(__32MX795F512L__) // chipKIT specific 
//#include "WProgram.h"
//#elif defined(__AVR_ATmega644P__) // Wiring specific
//#include "Wiring.h"
//#elif defined(__LM4F120H5QR__) // StellarPad specific
//#include "Energia.h"
//#else
//#error Board not supported.
//#endif

// Library header
#include "Energy.h"

// Code
Energy::Energy() {
}

void Energy::begin(Serial_LCD * lcd, uint32_t seconds) {
  _pscreen = lcd;
  _ms = (uint32_t)seconds*1000;
  _chrono = millis();
  _state = true; // backlight on
}

void Energy::check(boolean flag) {
  if ( flag ) {
    _chrono = millis();
    if ( _state==false ) {
      _state = true;
      _pscreen->setBacklight(true); 
    }
  } else if ( (_state==true) & (millis()>_chrono+_ms) ) {
    _state = false;
    _pscreen->setBacklight(false);   
  }
}