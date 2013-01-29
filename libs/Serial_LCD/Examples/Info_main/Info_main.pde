///
/// @file 	Info_main.pde
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

#elif defined(__32MX320F128H__) || defined(__32MX795F512L__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega644P__) // hardware serial Serial1
  #define mySerial Serial1

#else // error
  #error Platform not defined
#endif
// === End of Serial port choice ===

ProxySerial myPort(&mySerial); // hardware abstraction layer
Serial_LCD myLCD(&myPort); // LCD

button myButton, myRotate;
uint16_t x, y;
uint32_t l;


void show() {
  Serial.print("\n");
  Serial.print(myLCD.WhoAmI());
  Serial.print("\n");


  Serial.print("\n checkScreenType \t");
  switch (myLCD.checkScreenType()) {
  case 0:
    dLabel(&myLCD, 0, 0, myLCD.maxX(), 40, "uOLED", 0xffff, 0x0000, 0, 0, 9);
    Serial.print("uOLED");
    break;
  case 1:
    dLabel(&myLCD, 0, 0, myLCD.maxX()/2, 40, "uLCD", 0xffff, 0x0000, 0, 0, 9);
    Serial.print("uLCD");
    break;
  case 2:
    dLabel(&myLCD, 0, 0, myLCD.maxX()/2, 40, "uVGA", 0xffff, 0x0000, 0, 0, 9);
    Serial.print("uVGA");
    break;
  default:
    dLabel(&myLCD, 0, 0, myLCD.maxX()/2, 40, "Unknown", 0xffff, 0x0000, 0, 0, 9);
  }


  Serial.print("\n checkControllerType \t");
  switch (myLCD.checkControllerType()) {
  case 0:
    dLabel(&myLCD, myLCD.maxX()/2, 0, myLCD.maxX()/2, 40, "8-bits Goldelox", 0xffff, 0x0000, 0, 0, 9);
    Serial.print("8-bits Goldelox");
    break;
  case 1:
    dLabel(&myLCD, myLCD.maxX()/2, 0, myLCD.maxX()/2, 40, "16-bits Picaso", 0xffff, 0x0000, 0, 0, 9);
    Serial.print("16-bits Picaso");
    break;
  default:
    dLabel(&myLCD, myLCD.maxX()/2, 0, myLCD.maxX()/2, 40, "Unknown", 0xffff, 0x0000, 0, 0, 9);
  }

  dLabel(&myLCD, 0, 40, myLCD.maxX()/2, 40, "Hardware: " + htoa(myLCD.checkHardwareVersion(), 2), 0xffff, myLCD.setColour(0x7f, 0x7f, 0x00), 0, 0, 9);
  dLabel(&myLCD, myLCD.maxX()/2, 40, myLCD.maxX()/2, 40, "Software: " + htoa(myLCD.checkSoftwareVersion(), 2), 0xffff, myLCD.setColour(0x7f, 0x7f, 0x00), 0, 0, 9);
  Serial.print("\n checkHardwareVersion \t");
  Serial.print(htoa(myLCD.checkHardwareVersion(), 2));
  Serial.print("\n checkSoftwareVersion \t");
  Serial.print(htoa(myLCD.checkSoftwareVersion(), 2));

  dLabel(&myLCD, 0, 80, myLCD.maxX()/2, 40, "max X:" + ftoa(myLCD.maxX(), 0, 5), 0xffff, myLCD.setColour(0x7f, 0x7f, 0x7f), 0, 0, 9);
  dLabel(&myLCD, myLCD.maxX()/2, 80, myLCD.maxX()/2, 40, "max Y:" + ftoa(myLCD.maxY(), 0, 5), 0xffff, myLCD.setColour(0x7f, 0x7f, 0x7f), 0, 0, 9);
  Serial.print("\n maxX \t");
  Serial.print(ftoa(myLCD.maxX(), 0, 5));
  Serial.print("\n maxY \t");
  Serial.print(ftoa(myLCD.maxY(), 0, 5));

  myLCD.initSD();
  Serial.print("\n checkSD \t");

  if (myLCD.checkSD()) {
    dLabel(&myLCD, 0, 120, myLCD.maxX()/2, 40, "SD card", 0xffff, myLCD.setColour(0x00, 0x7f, 0x00), 0, 0, 9);
    Serial.print("\n checkRAW \t");

    if (myLCD.checkRAW()) {
      dLabel(&myLCD, myLCD.maxX()/2, 120, myLCD.maxX()/2, 40, "FAT + RAW",   0xffff, myLCD.setColour(0x00, 0x00, 0x7f), 0, 0, 9);
      Serial.print("FAT + RAW");
    } 
    else {
      dLabel(&myLCD, myLCD.maxX()/2, 120, myLCD.maxX()/2, 40, "FAT only",  0xffff, myLCD.setColour(0x00, 0x00, 0x7f), 0, 0, 9);
      Serial.print("FAT only");
    } 
  }
  else {
    dLabel(&myLCD, 0, 120, myLCD.maxX()/2, 40, "no SD card",   0xffff, myLCD.setColour(0x7f, 0x00, 0x00), 0, 0, 9);
    Serial.print("no SD card");
  }

  Serial.print("\n getOrientation \t");
  Serial.print(myLCD.getOrientation(), DEC);
  Serial.print("\n");


  Serial.print("\n checkScreenType==1 \t");
  Serial.print((myLCD.checkScreenType()==1), DEC);
  Serial.print("\n");

  if (myLCD.checkScreenType()==1) {
    myRotate.dDefine(&myLCD, 0, myLCD.maxY()-50, 80, 50, setItem(0, "Rotate"), myLCD.setColour(0xff, 0xff, 0xff), myLCD.setColour(0x00, 0xff, 0x00));
    myRotate.enable(true);
    myRotate.draw();

    myButton.dDefine(&myLCD, myLCD.maxX()-80, myLCD.maxY()-50, 80, 50, setItem(0, "Stop"), myLCD.setColour(0xff, 0xff, 0xff), myLCD.setColour(0xff, 0x00, 0x00));
    myButton.enable(true);
    myButton.draw();
  }
}


void setup() {
  Serial.begin(9600);
  Serial.print("\n\n\n***\n");

  // === Serial port initialisation ===
#if defined(__I2C_Serial__)
  Serial.print("i2c Serial\n");
  Wire.begin();
  mySerial.begin(9600);

#elif defined(__AVR_ATmega328P__) | defined(__AVR_ATmega328P__)
  Serial.print("Software Serial \n");
  mySerial.begin(9600);

#elif defined(__PIC32MX__) || defined(__AVR_ATmega2560__)
  Serial.print("Hardware Serial\n");
  Serial1.begin(9600);

#endif 
  // === End of Serial port initialisation ===

  myLCD.begin(4);  // 9600 at start-up
  myLCD.setOrientation(0x03);
  if (myLCD.checkScreenType()==1) {
    myLCD.setTouch(true);
  }
  show();
}

uint8_t c;
uint16_t result;
boolean b=true;

void loop() {
  if (myLCD.checkScreenType()==1) {
    if (myLCD.getTouchActivity()>0) {
      if (myRotate.check()) {
        myLCD.clear();
        myLCD.setOrientation(myLCD.getOrientation() %4 +1);
        show();
      } 
      else if (myButton.check()) {
        myLCD.off();
          Serial.print("\n END");

        while(true);
      }
    }
  } 

  delay(250);
}


















