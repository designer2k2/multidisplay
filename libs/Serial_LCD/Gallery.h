///
/// @file 	Gallery.h
/// @brief	Gallery of pictures
/// @details	Display a gallery of pictures
/// @n
/// @n @a	Member of Serial_LCD Library Suite for 4D Systems uLCD-uLED-uVGA screens
/// @n @a	on Arduino 0023 and 1.0, chipKIT MPIDE 0023, Wiring 1.0
///
/// @a 		Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author	Rei VILO
/// @author	http://embeddedcomputing.weebly.com
/// @date	Nov 29, 2012
/// @version 	release 311
/// @n
/// @copyright	© Rei VILO, 2010-2013
/// @copyright	CC = BY NC SA [Attribution-NonCommercial-ShareAlike](http://creativecommons.org/licenses/by-nc-sa/3.0/)
/// @n		http://embeddedcomputing.weebly.com/serial-lcd.html
/// @n		http://github.com/rei-vilo/Serial_LCD
///
/// @see	4D Systems Goldelox and Picaso SGC Command Set
/// @n		http://www.4dsystems.com.au/
///
#define GALLERY_RELEASE 311
#ifndef Gallery_h
#define Gallery_h
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
//#if defined(MPIDE) || defined(__LM4F120H5QR__) // chipKIT and StellarPad specific
#define MAXPICTURE 64
//#else
//#include "vector_t.h"
//#endif
#if SERIAL_LCD_RELEASE < 336
#error required SERIAL_LCD_RELEASE 336
#endif
class Gallery {
public:
    Gallery();
    ~Gallery();
    uint8_t begin(Serial_LCD * lcd, String name);
    uint8_t number();
    uint8_t index();
    uint8_t showNext();
    uint8_t showPrevious();
    uint8_t showImage(uint8_t index=0);
private:
    struct image_t {
        //    String name;
        uint16_t msb, lsb, x, y;
    };
    Serial_LCD * _pscreen;
    String _name;
    uint8_t _index;
    //#if defined(MPIDE) || defined(__LM4F120H5QR__) // chipKIT and StellarPad specific
    image_t _gallery[MAXPICTURE];
    uint16_t _size;
    //#else
    //  Vector_t <image_t> _gallery;
    //#endif
};
#endif
