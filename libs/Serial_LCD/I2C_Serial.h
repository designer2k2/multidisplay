///
/// @file	I2C_Serial.h
/// @brief	SC16IS7xx I2C slave bridge to serial
/// @details	I2C to serial bridge and 8 I/Os
/// @n
/// @n
/// @n @a	Developed with [embedXcode](http://embedXcode.weebly.com)
/// @n
/// @author	Rei VILO, http://embeddedcomputing.weebly.com
/// @date 	Sep 01, 2012
/// @version 	release 308
/// @n
/// @copyright	© Rei VILO, 2010-2013
/// @copyright	CC = BY NC SA [Attribution-NonCommercial-ShareAlike](http://creativecommons.org/licenses/by-nc-sa/3.0/)
/// @n		http://embeddedcomputing.weebly.com/
///
/// @see	NXP SC16IS7xx
/// 		http://www.nxp.com/
///
///
/// @brief	I2C_Serial release number
///
#define I2C_SERIAL_RELEASE 308
#ifndef I2C_Serial_h
#define I2C_Serial_h
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
#error Platform not defined
#endif
//#include "Stream.h"
#include "Wire.h"
#include "Print.h"
#define ALL  0xff
class I2C_Serial : public Stream {
public:
  I2C_Serial(uint8_t number=0);
  String WhoAmI();
  void begin(long baud=9600);
  // write and read functions
#if ( (defined(ARDUINO) && (ARDUINO >= 100)) || defined(ENERGIA) )
  virtual size_t write(uint8_t byte);
#else
  virtual void write(uint8_t byte);
#endif
  virtual int read();
  virtual int available();
  virtual void flush();
  virtual int peek(); // !
  uint8_t free();
  boolean test();
  void loopback(boolean flag=true);
  void pinMode(uint8_t pin, uint8_t mode);
  void digitalWrite(uint8_t pin, uint8_t value);
  int digitalRead(uint8_t pin);
  void reset(uint8_t pin, uint8_t value=LOW, uint8_t ms=10);
private:
  int8_t _address;
};
#endif
