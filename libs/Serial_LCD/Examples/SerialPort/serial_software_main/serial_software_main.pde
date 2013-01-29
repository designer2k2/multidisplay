///
/// @file 	serial_software_main.pde
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

// test release
#if SERIAL_LCD_RELEASE < 333
#error required SERIAL_LCD_RELEASE 333
#endif

// === Serial port choice
// --- SoftwareSerial Case - Arduino only
#if defined(ARDUINO) && (ARDUINO>=100) // for Arduino 1.0
#include "SoftwareSerial.h"
SoftwareSerial mySerial(2, 3);
#else
#include "NewSoftSerial.h" // for Arduino 23
NewSoftSerial mySerial(2, 3);
#endif

// pin 4 for RESET

ProxySerial myPort(&mySerial); // hardware abstraction layer
Serial_LCD myLCD(&myPort); // LCD

uint16_t x, y;
uint32_t l;

button b7;



void setup() {
  Serial.begin(9600);
  Serial.print("\n\n\n***\n");

  //  // === Serial port initialisation
  //  // --- SoftwareSerial Case - Arduino only
  mySerial.begin(9600);
  Serial.print("SoftwareSerial\t");
  Serial.print("\n");
  //  //
  //  // --- HardwareSerial Case - Arduino + chipKIT
  //  Serial1.begin(9600);
  //  Serial.print("HardwareSerial\t");
  //  Serial.print("\n");
  //  // 
  //  // --- i2cSerial Case - Arduino + chipKIT
  //  Wire.begin();
  //  myI2C.begin(9600);
  //  Serial.print("HardwareSerial\t");
  //  Serial.print("\n");
  //  //
  //  // ===

  myLCD.begin(4);

  delay(10);
  myLCD.setSpeed(38400);

  //  // === Serail port speed up
  //  // --- SoftwareSerial Case - Arduino only
  mySerial.begin(38400);
  //  //
  //  // --- HardwareSerial Case - Arduino + chipKIT
  //  Serial1.begin(38400);
  //  // 
  //  // --- i2cSerial Case - Arduino + chipKIT
  //  myI2C.begin(38400);
  //  //
  //  // ===  

  myLCD.setOrientation(0x03);

  myLCD.setPenSolid(true);
  myLCD.setFontSolid(true);

  myLCD.setFont(0);
  myLCD.gText( 0, 210, myLCD.WhoAmI());

  myLCD.setTouch(true);

  l=millis();

  b7.define(&myLCD, 0, 0, 79, 59, setItem(2, "Stop"), myLCD.setColour(0xff, 0xff, 0xff), myLCD.setColour(0xff, 0x00, 0x00), myLCD.setColour(0x88, 0x00, 0x00), 9);

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






















