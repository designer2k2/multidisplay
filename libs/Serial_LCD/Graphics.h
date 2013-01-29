///
/// @file 	Graphics.h
/// @brief	Graphics with clock, histogram (one or up to 4 values), yaw-pitch-roll, gauge
/// @details 	Graphics provides high-level dedicated graphics
/// @n
/// @n
/// @n @a	Member of Serial_LCD Library Suite for 4D Systems uLCD-uLED-uVGA screens
/// @n @a	on Arduino 0023 and 1.0, chipKIT MPIDE 0023, Wiring 1.0
///
/// @a 		Developed with [embedXcode](http://embedXcode.weebly.com)
/// @n
/// @author	Rei VILO
/// @author	http://embeddedcomputing.weebly.com
/// @date 	Sep 12, 2012
/// @version 	release 317
/// @n
/// @copyright 	© Rei VILO, 2010-2013
/// @copyright 	CC = BY NC SA [Attribution-NonCommercial-ShareAlike](http://creativecommons.org/licenses/by-nc-sa/3.0/)
/// @n		http://embeddedcomputing.weebly.com/serial-lcd.html
/// @n		http://github.com/rei-vilo/Serial_LCD
///
/// @see	4D Systems Goldelox and Picaso SGC Command Set
/// @n		http://www.4dsystems.com.au/
///
#define GRAPHICS_RELEASE 317
#ifndef Graphics_h
#define Graphics_h
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
class mtgDial {
public:
  mtgDial();
protected:
  void drawDial(Serial_LCD * lcd,
                uint16_t x0, uint16_t y0, uint16_t radius,
                uint16_t backColour, uint16_t frontColour,
                uint16_t gridColour, uint16_t valueColour);
  Serial_LCD * _pscreen;
  uint16_t _x0, _y0, _radius;
  uint16_t _backColour, _frontColour;
  uint16_t _gridColour, _valueColour;
};
class gClock: public mtgDial {
public:
  gClock();
  void dDefine(Serial_LCD * lcd,
               uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
               uint16_t backColour=blackColour, uint16_t frontColour=greenColour,
               uint16_t hourColour=redColour, uint16_t minuteColour=blueColour, uint16_t secondColour=yellowColour);
  void define(Serial_LCD * lcd,
              uint16_t x0, uint16_t y0, uint16_t radius,
              uint16_t backColour=blackColour, uint16_t frontColour=greenColour,
              uint16_t hourColour=redColour, uint16_t minuteColour=blueColour, uint16_t secondColour=yellowColour);
  void draw(uint8_t hour, uint8_t minute, uint8_t second);
private:
  uint16_t _hourColour, _minuteColour, _secondColour;
  uint8_t _oldHour, _oldMinute, _oldSecond;
};
class gYaw: public mtgDial {
public:
  gYaw();
  void dDefine(Serial_LCD * lcd,
               uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
               uint16_t backColour=blackColour, uint16_t frontColour=whiteColour,
               uint16_t gridColour=yellowColour, uint16_t valueColour=blueColour);
  void define(Serial_LCD * lcd,
              uint16_t x0, uint16_t y0, uint16_t radius,
              uint16_t backColour=blackColour, uint16_t frontColour=whiteColour,
              uint16_t gridColour=yellowColour, uint16_t valueColour=blueColour);
  void draw(float degrees);
private:
  float _oldDegrees;
};
class gPitch: public mtgDial {
public:
  gPitch();
  void dDefine(Serial_LCD * lcd,
               uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
               uint16_t backColour=blackColour, uint16_t frontColour=whiteColour,
               uint16_t gridColour=yellowColour, uint16_t valueColour=blueColour);
  void define(Serial_LCD * lcd,
              uint16_t x0, uint16_t y0, uint16_t radius,
              uint16_t backColour=blackColour, uint16_t frontColour=whiteColour,
              uint16_t gridColour=yellowColour, uint16_t valueColour=blueColour);
  void draw(float degrees);
private:
  float _oldDegrees;
};
class gRoll: public mtgDial {
public:
  gRoll();
  void dDefine(Serial_LCD * lcd,
               uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
               uint16_t backColour=blackColour, uint16_t frontColour=whiteColour,
               uint16_t gridColour=yellowColour, uint16_t valueColour=blueColour);
  void define(Serial_LCD * lcd,
              uint16_t x0, uint16_t y0, uint16_t radius,
              uint16_t backColour=blackColour, uint16_t frontColour=whiteColour,
              uint16_t gridColour=yellowColour, uint16_t valueColour=blueColour);
  void draw(float degrees);
private:
  float _oldDegrees;
};
class gGauge: public mtgDial {
public:
  gGauge();
  void define(Serial_LCD * lcd,
              uint16_t x0, uint16_t y0, uint16_t radius,
              float valueMin, float valueMax, uint16_t memory=8, uint16_t grid=8,
              uint16_t backColour=blackColour, uint16_t frontColour=whiteColour, uint16_t gridColour=yellowColour,
              uint16_t valueColour=blueColour, uint16_t minColour=greenColour, uint16_t maxColour=redColour);
  void dDefine(Serial_LCD * lcd,
               uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
               float valueMin, float valueMax, uint16_t memory=8, uint16_t grid=8,
               uint16_t backColour=blackColour, uint16_t frontColour=whiteColour, uint16_t gridColour=yellowColour,
               uint16_t valueColour=blueColour, uint16_t minColour=greenColour, uint16_t maxColour=redColour);
  void draw(float value, String sValue="");
private:
  //  Serial_LCD * _pscreen;
  //  uint16_t _x0, _y0, _radius;
  //  uint16_t _backColour, _frontColour;
  //  uint16_t _gridColour, _valueColour;
  uint16_t _dx, _dy;
  float _valueMin, _valueMax, _oldZ;
  float _min, _max;
  uint16_t _n, _amnesiaMin, _amnesiaMax;
  uint32_t _lapse;
  uint16_t _memory, _grid;
  uint16_t _minColour, _maxColour;
};
class mtgPanel {
public:
  mtgPanel();
protected:
  void drawPanel(Serial_LCD * lcd,
                 uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 uint16_t lapse, uint16_t gridX, uint16_t gridY, boolean continous,
                 uint16_t backColour, uint16_t frontColour, uint16_t gridColour);
  void drawContinue();
  void drawLapse(uint32_t ms);
  Serial_LCD * _pscreen;
  uint16_t _x0, _y0, _dx, _dy;
  uint16_t _n;
  uint16_t _lapse;
  boolean _continous;
  uint16_t _gridX, _gridY, _gridC;
  uint16_t _backColour, _frontColour, _gridColour;
  uint32_t _chrono;
};
class gHistogram: public mtgPanel {
public:
  gHistogram();
  void dDefine(Serial_LCD * lcd,
               uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
               float valueMin, float valueMax,
               uint32_t lapse=0, uint16_t memory=8,
               uint16_t gridX=4, uint16_t gridY=0, boolean continous=false,
               uint16_t backColour=blackColour, uint16_t frontColour=whiteColour, uint16_t gridColour=yellowColour,
               uint16_t valueColour=blueColour, uint16_t minColour=greenColour, uint16_t maxColour=redColour);
  void define(Serial_LCD * lcd,
              uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
              float valueMin, float valueMax,
              uint32_t lapse=0, uint16_t memory=8, uint16_t gridX=4, uint16_t gridY=0, boolean continous=false,
              uint16_t backColour=blackColour, uint16_t frontColour=whiteColour, uint16_t gridColour=yellowColour,
              uint16_t valueColour=blueColour, uint16_t minColour=greenColour, uint16_t maxColour=redColour);
  void draw(float value);
private:
  float _valueMin, _valueMax;
  uint16_t _min, _max;
  uint16_t _amnesiaMin, _amnesiaMax;
  uint16_t _memory;
  uint16_t _valueColour, _minColour, _maxColour;
};
class gMultiple: public mtgPanel {
public:
  gMultiple();
  void dDefine(Serial_LCD * lcd,
               uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
               String serie1, float valueMin1, float valueMax1, uint16_t valueColour1=blueColour,
               String serie2="", float valueMin2=0, float valueMax2=0, uint16_t valueColour2=greenColour,
               String serie3="", float valueMin3=0, float valueMax3=0, uint16_t valueColour3=redColour,
               String serie4="", float valueMin4=0, float valueMax4=0, uint16_t valueColour4=whiteColour,
               uint32_t lapse=0, uint16_t gridX=4, uint16_t gridY=0, boolean continous=false,
               uint16_t backColour=blackColour, uint16_t frontColour=whiteColour, uint16_t gridColour=yellowColour);
  void define(Serial_LCD * lcd,
              uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
              String serie1, float valueMin1, float valueMax1, uint16_t valueColour1=blueColour,
              String serie2="", float valueMin2=0, float valueMax2=0, uint16_t valueColour2=greenColour,
              String serie3="", float valueMin3=0, float valueMax3=0, uint16_t valueColour3=redColour,
              String serie4="", float valueMin4=0, float valueMax4=0, uint16_t valueColour4=whiteColour,
              uint32_t lapse=0, uint16_t gridX=4, uint16_t gridY=0, boolean continous=false,
              uint16_t backColour=blackColour, uint16_t frontColour=whiteColour, uint16_t gridColour=yellowColour);
  void draw(float value1, float value2=0, float value3=0, float value4=0);
private:
  String _name1, _name2, _name3, _name4;
  float _valueMin1, _valueMin2, _valueMin3,_valueMin4, _valueMax1, _valueMax2, _valueMax3, _valueMax4;
  uint16_t _valueColour1, _valueColour2, _valueColour3, _valueColour4;
};
#endif
