//
/// @file 	memu_main.pde
/// @brief	Example
/// @details 	
/// @n @a 	Example for Serial_LCD Library Suite
/// @n @a	for 4D Systems uLCD-uLED-uVGA Serial_LCD Library Suite
/// @n 		on Arduino 0023 and 1.0, chipKIT MPIDE 0023, Wiring 1.0
///
/// @a 		Developed with [embedXcode](http://embedXcode.weebly.com)
/// 
/// @author 	Rei VILO
/// @author 	http://embeddedcomputing.weebly.com
/// @date	Jul 12, 2012
/// @version	release 132
/// @n
/// @copyright 	© Rei VILO, 2010-2012
/// @copyright 	CC = BY NC SA
/// @n		http://embeddedcomputing.weebly.com/serial-lcd.html
/// @n		http://github.com/rei-vilo/Serial_LCD
///
/// @see 	4D Systems Goldelox and Picaso SGC Command Set
/// @n		http://www.4dsystems.com.au/
///

// Core library - MCU-based
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__) // Arduino specific
  #if defined(ARDUINO) && (ARDUINO >= 100)
  #include "Arduino.h" // for Arduino 1.0
  #else
  #include "WProgram.h" // for Arduino 23
  #endif
#elif defined(__32MX320F128H__) || defined(__32MX795F512L__) // chipKIT specific 
#include "WProgram.h"
#elif defined(__AVR_ATmega644P__) // Wiring specific
#include "Wiring.h"
#else // error
#error Platform not supported
#endif

// Include application, user and local libraries
#include "Wire.h"
#include "proxySerial.h"
#include "Serial_LCD.h"
#include "GUI.h"

// test release
#if GUI_RELEASE < 315
#error required GUI_RELEASE 315
#endif

// uncomment for I2C serial interface
//#define __I2C_Serial__

// === Serial port choice ===
#if defined(__I2C_Serial__) // I2C serial
  #include "Wire.h"
  #include "I2C_Serial.h"
  I2C_Serial mySerial(0);

#elif defined(__AVR_ATmega328P__) // software serial
  #if defined(ARDUINO) && (ARDUINO>=100) // for Arduino 1.0
    #include "SoftwareSerial.h"
    SoftwareSerial mySerial(2, 3);
  #else
    #include "NewSoftSerial.h" // for Arduino 23
    NewSoftSerial mySerial(2, 3);
  #endif

#elif defined(__32MX320F128H__) || defined(__32MX795F512L__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega644P__) // hardware serial Serial1
  #define mySerial Serial1

#else // error
  #error Platform not defined
#endif
// === End of Serial port choice ===

// Define variables and constants
ProxySerial myPort(&mySerial); // hardware abstraction layer
Serial_LCD myLCD(&myPort); // LCD

uint16_t x, y;
uint32_t l;

button b7;



void setup() {
  Serial.begin(9600);
  Serial.print("\n\n\n***\n");

  // === Serial port initialisation ===
#if defined(__I2C_Serial__)
  Serial.print("i2c Serial\n");
  Wire.begin();

#elif defined(__AVR_ATmega328P__) 
  Serial.print("Software Serial\n");

#elif defined(__32MX320F128H__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega644P__)
  Serial.print("Hardware Serial\n");

#endif 
  // === End of Serial port initialisation ===
  mySerial.begin(9600);

  myLCD.begin(4);  // 9600 at start-up

  // === Serial port speed change ===
  if (false) {
    myLCD.setSpeed(38400);
    mySerial.begin(38400);
  }
  // === End of Serial port speed change ===

  myLCD.setOrientation(0x03);
  myLCD.setPenSolid(true);
  myLCD.setFontSolid(true);

  myLCD.setFont(0);
  myLCD.gText( 0, 210, myLCD.WhoAmI());

  myLCD.setTouch(true);

  l=millis();

  myLCD.ellipse(100, 100, 50, 20, myLCD.setColour(0xff,0x00,0x00));

  b7.dDefine(&myLCD, 0, 0, 80, 60, setItem(0, "Stop"), myLCD.setColour(0xff, 0xff, 0xff), myLCD.setColour(0xff, 0x00, 0x00), myLCD.setColour(0x88, 0x00, 0x00), 9);
  b7.enable(true);
  b7.draw();

  myLCD.setPenSolid(false);
  myLCD.rectangle(40, 40, 200, 200, myLCD.setColour(0x00, 0x00, 0xff));

  myLCD.detectTouchRegion(40, 40, 200, 200);
}

uint8_t c;

void loop() {
  c=myLCD.getTouchActivity();
  myLCD.setFont(3);
  myLCD.gText(140, 10, ftoa(c, 0, 5)); 

  if ((c==1) || (c==3)) {
    myLCD.getTouchXY(x, y);

    myLCD.setFont(1);
    myLCD.gText(200, 0, ftoa(x, 0, 5)); 
    myLCD.gText(200, 20, ftoa(y, 0, 5)); 

    myLCD.point(x, y, 0xffff);

    // quit
    if (b7.check()) {
      myLCD.off();
      while(true);
    }
  }

  myLCD.setFont(0);
  myLCD.setFontSolid(true);
  myLCD.gText( 250, 225, String(millis()-l));
  l=millis();
}



