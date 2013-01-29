///
/// @file 	histogram_gauge_main.pde
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

#if defined(__AVR_ATmega328P__)
#error This sketch requires about 35 KB, too big for Arduino Uno.
#endif

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
gHistogram myHistogram;


void setup() {
  Serial.begin(19200);
  Serial.print("\n\n\n***\n");

  // === Serial port initialisation ===
  // --- Arduino Uno - software serial
#if defined(__I2C__)
  Wire.begin();
  Serial.print("I2C\n");
#elif defined(__AVR_ATmega328P__) 
  Serial.print("software\n");
  // --- Arduino mega2560 - hardware serial
#elif defined(__AVR_ATmega2560__)
  Serial.print("hardware\n");
#elif defined(__PIC32MX__)
  Serial.print("hardware\n");
// --- I2C serial
#endif
  // === End of Serial port initialisation ===
  mySerial.begin(9600);

  myLCD.begin(4);

  Serial.print("begin\n");

  // === Serial port speed change ===
  myLCD.setSpeed(38400);
  mySerial.begin(38400);
  // === End of Serial port speed change ===

  myLCD.setOrientation(0x03);
  myLCD.setPenSolid(true);
  myLCD.setFontSolid(true);

  myLCD.setFont(0);
  myLCD.gText( 0, myLCD.maxY()-20, myLCD.WhoAmI());
  myLCD.setTouch(true);

  l=millis();

  b7.dDefine(&myLCD, myLCD.maxX()-70, myLCD.maxY()-40, 70, 40, setItem(0, "Stop"), myLCD.setColour(0xff, 0xff, 0xff), myLCD.setColour(0xff, 0x00, 0x00), myLCD.setColour(0x88, 0x00, 0x00), 9);
  b7.enable(true);
  b7.draw();

// void dDefine(Serial_LCD * lcd0, 
//    uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, 
//    float valueMin, float valueMax, 
//    uint16_t lapse=0, uint16_t memory=8, uint16_t gridX=4, uint16_t gridY=0, 
//    boolean continous=false, 
//    uint16_t backColour=blackColour, uint16_t frontColour=whiteColour, 
//    uint16_t gridColour=yellowColour, uint16_t valueColour=blueColour, 
//    uint16_t minColour=greenColour, uint16_t maxColour=redColour);

  myHistogram.dDefine(&myLCD, 20, 40, myLCD.maxX()/2-40, myLCD.maxY()-40-80, -2.0, 2.0, 250, 8, 4, 10, false);
  myGauge.dDefine(&myLCD, myLCD.maxX()/2+20, 20, myLCD.maxX()/2-40, myLCD.maxY()-40-40, -2.0, 2.0, 8, 8);
}

uint32_t ll = 0;
uint16_t i = 0;
float v;
uint8_t c;


void loop() {
  Serial.print(".");
  
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
  while (millis()-ll<250);

  ll=millis();
  myHistogram.draw( v );
  myGauge.draw( v, ftoa(v, 2, 7) );

//  myLCD.setFont(3);
//  myLCD.setFontSolid(true);
//  myLCD.gText( 160, 180, ftoa(v, 2, 10 ));

  c = myLCD.getTouchActivity();
  
  if ( c>0 ) {
    
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
  myLCD.gText( 250, myLCD.maxY()-20, ttoa(millis()-l, 0, 6));
  l=millis();

  //  delay(100);
}







