///
/// @file	Energy.h
/// @brief	Power saving based on idle time
/// @details	Energy switches backlight off when inactivity period is reach to save energy
/// @n
/// @n @a	Member of Serial_LCD Library Suite for 4D Systems uLCD-uLED-uVGA screens
/// @n @a	on Arduino 0023 and 1.0, chipKIT MPIDE 0023, Wiring 1.0
///
/// @a		Developed with [embedXcode](http://embedXcode.weebly.com)
/// @n
/// @author	Rei VILO
/// @author	http://embeddedcomputing.weebly.com
/// @date 	Sep 01, 2012
/// @version	release 307
/// @n
/// @copyright	© Rei VILO, 2010-2013
/// @copyright	CC = BY NC SA [Attribution-NonCommercial-ShareAlike](http://creativecommons.org/licenses/by-nc-sa/3.0/)
/// @n		http://embeddedcomputing.weebly.com/serial-lcd.html
/// @n		http://github.com/rei-vilo/Serial_LCD
///
/// @see	4D Systems Goldelox and Picaso SGC Command Set
/// @n		http://www.4dsystems.com.au/
///
#define ENERGY_RELEASE 307
#ifndef Energy_h
#define Energy_h
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
#include "Serial_LCD.h"
#if SERIAL_LCD_RELEASE < 334
#error required SERIAL_LCD_RELEASE 334
#endif
class Energy {
public:
  Energy();
  void begin(Serial_LCD * lcd, uint32_t seconds=3);
  void check(boolean flag);
private:
  Serial_LCD * _pscreen;
  uint32_t _ms;
  uint32_t _chrono;
  boolean _state;
};
#endif
