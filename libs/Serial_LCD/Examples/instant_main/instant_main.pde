///
/// @file 	instant_main.pde
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

#include "Serial_LCD.h"
#include "GUI.h"

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

#elif defined(__32MX320F128H__) || defined(__32MX795F512L__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega644P__) // hardware serial Serial1
  #define mySerial Serial1

#else // error
  #error Platform not defined
#endif
// === End of Serial port choice ===

ProxySerial myPort(&mySerial); // hardware abstraction layer
Serial_LCD myLCD(&myPort); // LCD
uint16_t x, y;
uint32_t l;

button b1, b2, b7;



void setup() {
  Serial.begin(19200);
  Serial.print("\n\n\n***\n");

#if defined(__I2C_Serial__)
  Wire.begin();
#elif defined(__AVR__)  || defined(__AVR_ATmega328P__) | defined(__AVR_ATmega328P__)
  Serial.print("avr\t");
  Serial.print(__AVR__);
  Serial.print("\n");
#elif defined(__PIC32MX__) 
  Serial.print("chipKIT\t");
  Serial.print(__PIC32MX__);
  Serial.print("\n");
#endif 
  mySerial.begin(9600);
  myLCD.begin();
  myLCD.setOrientation(0x03);

  myLCD.setPenSolid(true);
  myLCD.setFontSolid(true);

  myLCD.setFont(0);
  myLCD.gText( 0, 210, myLCD.WhoAmI());

  myLCD.setTouch(true);

  l=millis();

  uint16_t i=9;
  b1.dDefine(&myLCD,  0, myLCD.maxY()-60, 79, 59, setItem(1, "Normal"), myLCD.setColour(0xff, 0xff, 0xff), myLCD.setColour(0x00, 0xff, 0x00), myLCD.setColour(0x00, 0x88, 0x00), i);
  b2.dDefine(&myLCD,  80, myLCD.maxY()-60, 79, 59, setItem(2, "Instant"), myLCD.setColour(0xff, 0xff, 0xff), myLCD.setColour(0x00, 0x00, 0xff), myLCD.setColour(0x00, 0x00, 0x88), i);
  b7.dDefine(&myLCD,  myLCD.maxX()-80, myLCD.maxY()-60, 79, 59, setItem(0, "Stop"), myLCD.setColour(0xff, 0xff, 0xff), myLCD.setColour(0xff, 0x00, 0x00), myLCD.setColour(0x88, 0x00, 0x00), i);

  b1.enable(true);
  b2.enable(true);
  b7.enable(true);
  b1.draw();
  b2.draw();
  b7.draw();

  //    myLCD.setFont(3);
  //    myLCD.gText(0,  0, "         1         2    ");
  //    myLCD.gText(0, 20, "12345678901234567890123456"); 
  //    myLCD.gText(0, 60, ftoa(myLCD.fontX(), 0, 8)); 
  //
  //    myLCD.setFont(2);
  //    myLCD.gText(0,  80, "         1         2         3         4");
  //    myLCD.gText(0, 100, "1234567890123456789012345678901234567890"); 
  //    myLCD.gText(0, 120, ftoa(myLCD.fontX(), 0, 8)); 

  myLCD.setFont(1);
  myLCD.gText(0,  0, "         1         2         3         4");
  myLCD.gText(0, 20, "1234567890123456789012345678901234567890"); 
  myLCD.gText(0, 60, ftoa(myLCD.fontX(), 0, 8)); 

  myLCD.setFont(0);
  myLCD.gText(0,  80, "         1         2         3         4         5");
  myLCD.gText(0, 100, "12345678901234567890123456789012345678901234567890123"); 
  myLCD.gText(0, 120, ftoa(myLCD.fontX(), 0, 8)); 
}

uint8_t c;

void loop() {

  if (myLCD.getTouchActivity()) {

    myLCD.getTouchXY(x, y);
    myLCD.setFont(0);
    myLCD.gText(0,  0, ftoa(x, 0, 5)); 
    myLCD.gText(0, 15, ftoa(y, 0, 5)); 

    // quit
    if (b1.check()) {
  myLCD.setPenSolid(true);
  myLCD.dRectangle(40, 40, 100, 100, myLCD.setColour(random(0, 0xff), random(0, 0xff), random(0, 0xff)));       
    } 
    if (b2.check(true)) {
  myLCD.setPenSolid(true);
  myLCD.dRectangle(180, 40, 100, 100, myLCD.setColour(random(0, 0xff), random(0, 0xff), random(0, 0xff)));       
      
    }
    
    if (b7.check()) {
      myLCD.off();
      while(true);
    }



  }
  myLCD.setFont(0);
  myLCD.setFontSolid(true);
  myLCD.gText( 0, 225, String(millis()-l));
  l=millis();
}



















