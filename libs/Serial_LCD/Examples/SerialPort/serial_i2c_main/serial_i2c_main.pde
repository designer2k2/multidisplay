///
/// @file 	serial_i2c_main.pde
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

// --- i2cSerial Case - Arduino + chipKIT
#include "Wire.h"
#include "I2C_Serial.h"
#include "Serial_LCD.h"
#include "proxySerial.h"
#include "GUI.h"

// Test release
#if I2C_SERIAL_RELEASE < 306
#error required I2C_SERIAL_RELEASE 306
#endif
// test release
#if GUI_RELEASE < 315
#error required GUI_RELEASE 315
#endif



I2C_Serial myI2C(0);
ProxySerial mySerial(&myI2C);
Serial_LCD myLCD( &mySerial); 
button b7;

uint16_t x, y;
uint32_t l;

void setup() {
  Serial.begin(19200);
  Serial.print("\n\n\n***\n");


  Serial.print("I2C_SERIAL_RELEASE: ");
  Serial.print(I2C_SERIAL_RELEASE, DEC);
  Serial.print("\n");


  // --- i2cSerial Case - Arduino + chipKIT
  Wire.begin();
  myI2C.begin(9600);
  Serial.print("I2C 9600 \t");

  Serial.print("test \t");
  Serial.print(myI2C.test(), DEC);
  Serial.print("*\n");
  // ===

  Serial.print("\n LCD reset \t");
  myI2C.reset(0);
  Serial.print("done \n");


  myLCD.begin(4);

  delay(10);

  // === Serial port speed up
  myLCD.setSpeed(38400);
  myI2C.begin(38400);
  Serial.print("I2C 38400 \t");
  // ===

  Serial.print("test \t");
  Serial.print(myI2C.test(), DEC);
  Serial.print("*\n");

  myLCD.setOrientation(0x03);
  myLCD.setPenSolid(true);
  myLCD.setFontSolid(true);
  myLCD.setFont(0);
  myLCD.gText( 0, 210, myLCD.WhoAmI());
  myLCD.setTouch(true);

  l=millis();
  b7.dDefine(&myLCD, 0, 0, 80, 60, setItem(0, "Stop"), myLCD.setColour(0xff, 0xff, 0xff), myLCD.setColour(0xff, 0x00, 0x00), myLCD.setColour(0x88, 0x00, 0x00), 9);
  b7.enable(true);
  b7.draw();

  uint16_t chrono0, chrono1;
  chrono0=millis();

  myLCD.setPenSolid(false);
  for (int i=0; i<10; i++) {
    for (int j=0; j<10; j++) {
      myLCD.circle(120+j*10, 30+i*10, 30, random(0, 0xffff));
    }
  }

  chrono1=millis();
  myLCD.gText( 0, 180, ftoa((chrono1-chrono0), 0, 10 ));

  chrono0=millis();

  myLCD.setPenSolid(true);
  for (int i=0; i<10; i++) {
    for (int j=0; j<10; j++) {
      myLCD.circle(120+j*10, 30+i*10, 30, random(0, 0xffff));
    }
  }

  chrono1=millis();
  myLCD.gText( 160, 180, ftoa((chrono1-chrono0), 0, 10 ));

}

uint8_t c;

void loop() {

  c=myLCD.getTouchActivity();

  if (c>0) {
    myLCD.getTouchXY(x, y);
    myLCD.setFont(0);
    myLCD.gText(200, 0, ftoa(x, 0, 5)); 
    myLCD.gText(200, 15, ftoa(y, 0, 5)); 

    // quit
    if (b7.check()) {
      myLCD.off();
      while(true);
    }
  }
  myLCD.setFont(0);
  myLCD.setFontSolid(true);
  myLCD.gText( 250, 225, ftoa(millis()-l, 0, 6));
  l=millis();
}


