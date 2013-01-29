///
/// @file 	GUI.h
/// @brief	GUI with label, button, dialog box, menu, slider
/// @details	GUI provides high-level elements for user graphic interface
/// @n
/// @n
/// @n @a	Member of Serial_LCD Library Suite for 4D Systems uLCD-uLED-uVGA screens
/// @n @a	on Arduino 0023 and 1.0, chipKIT MPIDE 0023, Wiring 1.0
///
/// @deprecated	Rectangle coordinates
/// @n		Rectangle coordinates are no longer supported. Use vector coordinates instead.
/// @ref Coordinates
/// @n		button::define, button::stringDefine and Label are deprecated;
/// @n		Consider button::dDefine, button::dStringDefine and dLabel instead.
///
/// @a 		Developed with [embedXcode](http://embedXcode.weebly.com)
/// @n
/// @author	Rei VILO
/// @author	http://embeddedcomputing.weebly.com
/// @date 	Dec 01, 2012
/// @version 	release 318
/// @n
/// @copyright 	© Rei VILO, 2010-2013
/// @copyright 	CC = BY NC SA [Attribution-NonCommercial-ShareAlike](http://creativecommons.org/licenses/by-nc-sa/3.0/)
#define GUI_RELEASE 318
#ifndef GUI_h
#define GUI_h
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
struct item {
  uint16_t index;
  char label[16];
};
item setItem(uint8_t index0, String text0);
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
class area {
public:
    area();
    void dDefine(Serial_LCD * lcd,
                 uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 uint16_t index=0x0000);
    boolean state();
    void enable(boolean flag=true);
    boolean check(boolean instant=false);
    uint16_t getIndex();
protected:
    uint16_t _x0, _y0, _dx, _dy;
    boolean _enable;
    uint16_t _index;
    Serial_LCD * _pscreen;
};
class button: public area {
public:
  button();
  void dDefine(Serial_LCD * lcd,
               uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
               item item0,
               uint16_t textColour1, uint16_t highColour1);
  void dDefine(Serial_LCD * lcd,
               uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
               item item0,
               uint16_t textColour1, uint16_t highColour1, uint16_t lowColour1,
               uint8_t size0=9);
  void dStringDefine(Serial_LCD * lcd,
                     uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                     String text0,
                     uint16_t textColour1, uint16_t highColour1);
  void dStringDefine(Serial_LCD * lcd,
                     uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                     String text0,
                     uint16_t textColour1, uint16_t highColour1, uint16_t lowColour1,
                     uint8_t size0=9);
//
//
//
//
//  boolean state();
    void draw(boolean flag=false);
  boolean check(boolean instant=false);
//
//
//
//
//    uint16_t getIndex();
private:
  uint16_t _textColour, _highColour, _lowColour;
  String _text;
  uint8_t _size;
};
uint8_t dialog(Serial_LCD * lcd,
               String text0, uint8_t kind0, uint16_t textColour0, uint16_t highColour0, uint16_t lowColour0,
               String text1="Press OK", String button1="OK", uint16_t textColour1=whiteColour, uint16_t highColour1=blueColour, uint16_t lowColour1=blackColour,
               String text2="\0", String button2="\0", uint16_t textColour2=0, uint16_t highColour2=0, uint16_t lowColour2=0,
               String text3="\0", String button3="\0", uint16_t textColour3=0, uint16_t highColour3=0, uint16_t lowColour3=0);
uint16_t menu(Serial_LCD * lcd,
              item menuItem0[], uint8_t nItems0,
              uint16_t textColour0, uint16_t highColourMain0, uint16_t highColourSub0);
void dLabel(Serial_LCD * lcd,
            uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
            String text0, uint16_t textColour0=whiteColour, uint16_t backColour0=blackColour,
            uint8_t horizontal0=0, uint8_t vertical0=0,
            uint8_t size0=9);
boolean slider(Serial_LCD * lcd,
               uint16_t &value, uint16_t min=0, uint16_t max=511, uint16_t step=8,
               uint16_t minColour0=greenColour, uint16_t maxColour0=redColour,
               String okText0="OK", uint16_t okTextColour0=whiteColour, uint16_t okColour0=blueColour,
               String cancelText0="Cancel", uint16_t cancelTextColour0=whiteColour, uint16_t cancelColour0=yellowColour);
#endif
