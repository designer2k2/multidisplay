///
/// @file 	Graphics_main.pde
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

#include "Wire.h"
#include "Serial_LCD.h"
#include "proxySerial.h"
#include "Graphics.h"

// test release
#if GRAPHICS_RELEASE < 313
#error required GRAPHICS_RELEASE 313
#endif

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
gClock myClock;
gYaw myDirection;

uint16_t x, y;
uint32_t l;


void setup() {
  Serial.begin(19200);
  Serial.print("\n\n\n***\n");

  // === Serial port initialisation ===
#if defined(__I2C_Serial__)
  Serial.print("i2c\n");
  Wire.begin();

#elif defined(__AVR__)  || defined(__AVR_ATmega328P__) | defined(__AVR_ATmega328P__)
  Serial.print("avr\n");

#elif defined(__PIC32MX__) 
  Serial.print("chipKIT\n");

#endif 
  // === End of Serial port initialisation ===

  mySerial.begin(9600);
  myLCD.begin(4);  // 9600 at start-up

  // === Serial port speed change ===
  myLCD.setSpeed(38400);
  mySerial.begin(38400);
  // === End of Serial port speed change ===

  myLCD.setOrientation(0x03);
  myLCD.setTouch(true);

  myClock.define(&myLCD, myLCD.maxX()/4, myLCD.maxY()/2, 60);
  myDirection.define(&myLCD, myLCD.maxX()*3/4, myLCD.maxY()/2, 60);
}

uint8_t g;
uint8_t h=0, m=0, s=0;

void loop() {


  if (myLCD.getTouchActivity()) {
    myLCD.off();
    while(true);
  } 

  l=millis();
  myClock.draw(h, m, s);
  myLCD.setFont(0);
  myLCD.setFontSolid(true);
  myLCD.gText( myLCD.maxX()/4-20, 225, ftoa(millis()-l, 0, 5));

  l=millis();
  myDirection.draw(g*2.0);
  myLCD.setFont(0);
  myLCD.setFontSolid(true);
  myLCD.gText( myLCD.maxX()*3/4-20, 225, ftoa(millis()-l, 0, 5));

  g++;
  g %= 60;

  delay(333);
  s++;
  if (s>59) {
    s = 0;
    m++;
  }
  if (m>59) {
    m = 0;
    h++;
  }
  if (h>23) {
    h = 0;
  }
}






