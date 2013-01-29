///
/// @file 	proxySerial.h
/// @brief	Serial port hardware abstraction layer
/// @details 	proxySerial handles hardware, software and I2C serial ports
/// @n
/// @n
/// @n @a	Member of Serial_LCD Library Suite for 4D Systems uLCD-uLED-uVGA screens
/// @n @a	on Arduino 0023 and 1.0, chipKIT MPIDE 0023, Wiring 1.0
///
/// @a		Developed with [embedXcode](http://embedXcode.weebly.com)
/// @n
/// @author	Rei VILO
/// @author	http://embeddedcomputing.weebly.com
/// @date 	Sep 01, 2012
/// @version	release 312
/// @n
/// @copyright	© Rei VILO, 2010-2013
/// @copyright	CC = BY NC SA [Attribution-NonCommercial-ShareAlike](http://creativecommons.org/licenses/by-nc-sa/3.0/)
/// @n		http://embeddedcomputing.weebly.com/serial-lcd.html
/// @n		http://github.com/rei-vilo/Serial_LCD
///
/// @see	4D Systems Goldelox and Picaso SGC Command Set
/// @n		http://www.4dsystems.com.au/
///
#define PROXYSERIAL_RELEASE 313
#ifndef proxy_Serial_h
#define proxy_Serial_h
// Core library - IDE-based
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(ENERGIA) // LaunchPad specific
#include "Energia.h"
#elif defined(ARDUINO) && (ARDUINO >= 100) // Arduino 1.0 specific
#include "Arduino.h"
#elif defined(ARDUINO) && (ARDUINO < 100) // Arduino 23 specific
#include "WProgram.h"
#else // error
#error Board not supported.
#endif
// Other libraries
#include "Stream.h"
#define SECURITYDELAY 0
String ftoa(float number, uint8_t precision=0, uint8_t size=0);
String u64toa(uint64_t number, uint8_t precision=0, uint8_t size=0);
String htoa(uint32_t number, uint8_t size=0);
String ttoa(uint64_t number, uint8_t precision=0, uint8_t size=0);
class ProxySerial
{
public:
  ProxySerial(Stream * proxyPort);
  void begin(uint16_t dummy);  // to be managed at serial port level
  void setXY16(boolean flag);
  void print(int8_t i);
  void print(uint8_t ui);
  void print(int16_t i);
  void print(uint16_t ui);
  void print(char c);
  void print(String s);
  uint8_t read();
  uint16_t read16();
  int8_t available();
  void flush();
private:
  uint16_t _millis;
#if (SECURITYDELAY>0)
  void _checkSpeed();
#endif
  Stream * _proxyPort;
};
#endif
