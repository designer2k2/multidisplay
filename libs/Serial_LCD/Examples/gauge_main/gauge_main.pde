///
/// @file 	gauge_main.pde
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
#include "proxySerial.h"
#include "GUI.h"
#include "Graphics.h"

// test release
#if GRAPHICS_RELEASE < 313
#error required GRAPHICS_RELEASE 313
#endif

// test release
#if SERIAL_LCD_RELEASE < 333
#error required SERIAL_LCD_RELEASE 333
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

button b7;
uint8_t a;
gGauge myGauge;


void setup() {
  Serial.begin(19200);
  Serial.print("\n\n\n***\n");

  // === Serial port initialisation ===
#if defined(__I2C_Serial__)
  Serial.print("i2c\n");
  Wire.begin();
  mySerial.begin(9600);

#elif defined(__AVR__)  || defined(__AVR_ATmega328P__) | defined(__AVR_ATmega328P__)
  Serial.print("avr\n");
  mySerial.begin(9600);

#elif defined(__PIC32MX__) 
  Serial.print("chipKIT\n");
  Serial1.begin(9600);

#endif 
  // === End of Serial port initialisation ===

  myLCD.begin(4);

  Serial.print("begin\n");

  // === Serial port speed change ===
  myLCD.setSpeed(38400);
#if defined(__I2C_Serial__)
  mySerial.begin(38400);

#elif defined(__AVR__)  || defined(__AVR_ATmega328P__) | defined(__AVR_ATmega328P__)
  mySerial.begin(38400);

#elif defined(__PIC32MX__) 
  Serial1.begin(38400);

#endif 
  // === End of Serial port speed change ===

  myLCD.setOrientation(0x03);
  myLCD.setPenSolid(true);
  myLCD.setFontSolid(true);

  myLCD.setFont(0);
  myLCD.gText( 0, 225, myLCD.WhoAmI());
  myLCD.setTouch(true);

  l=millis();

  b7.dDefine(&myLCD, 0, 0, 70, 40, setItem(0, "Stop"), myLCD.setColour(0xff, 0xff, 0xff), myLCD.setColour(0xff, 0x00, 0x00), myLCD.setColour(0x88, 0x00, 0x00), 9);
  b7.enable(true);
  b7.draw();

  myGauge.define(&myLCD, 169, 120, 60, -2.0, 2.0, 8, 8);
}

uint32_t ll = 0;
uint16_t i = 0;
float v;

void loop() {

  //  // Solution 1
  //  while (millis()-l<100) {
  //    v = cos(i*PI/64.0) + 0.3*cos(i*PI/16.0+PI/8.0);
  //    i++;
  //    i %= 256;
  //  }

  // Solution 2
  v = cos(i*PI/64.0) + 0.3*cos(i*PI/16.0+PI/8.0);
  i++;
  i %= 256;
//  while (millis()-ll<100);

  ll=millis();
  myGauge.draw( v, ftoa(v, 2, 7) );

//  myLCD.setFont(3);
//  myLCD.setFontSolid(true);
//  myLCD.gText( 160, 180, ftoa(v, 2, 10 ));

  if (myLCD.getTouchActivity()>0) {
    myLCD.getTouchXY(x, y);
    myLCD.setFont(0);
    myLCD.gText(200, 0, ftoa(x, 0, 5)); 
    myLCD.gText(200, 15, ftoa(y, 0, 5)); 

    // quit
    if (b7.check()) {
      myLCD.off();
      while(true);
    } // quit
  } // getTouchActivity

  myLCD.setFont(0);
  myLCD.setFontSolid(true);
  myLCD.gText( 250, 225, ttoa(millis()-l, 0, 6));
  l=millis();

  //  delay(100);
}
























