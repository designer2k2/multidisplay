///
/// @file 	play_wav_main.pde
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
/// @date	Dec 11, 2012
/// @version	release 337
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

#include <Wire.h>

#include "Serial_LCD.h"
#include "GUI.h"

#include "GUI.h"

// test release
#if GUI_RELEASE < 318
#error required GUI_RELEASE 318
#endif

// test release
#if SERIAL_LCD_RELEASE < 337
#error required SERIAL_LCD_RELEASE 337
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
button b8;
uint8_t a;

void setup() {
    Serial.begin(9600);
    Serial.println("\n\n\n***\n");
    
    mySerial.begin(9600);
    myLCD.begin(4);
    myLCD.setOrientation(0x03);
    
    Serial.println(myLCD.WhoAmI());
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);
  
#ifdef __I2C_Serial__
    Wire.begin();
#endif
    
    myLCD.setPenSolid(true);
    myLCD.setFontSolid(true);
    
    myLCD.setFont(0);
    myLCD.gText( 0, 210, myLCD.WhoAmI());
    
    myLCD.setTouch(true);
    
    l=millis();
    
    uint16_t i=9;
    b7.dStringDefine(&myLCD, 160, 120, 79, 59, "End", whiteColour, redColour, myLCD.halfColour(redColour));
    
    b7.enable(true);
    b7.draw();
    
    b8.dStringDefine(&myLCD, 80, 120, 79, 59, "Sound", whiteColour, yellowColour, myLCD.halfColour(yellowColour));

    Serial.print("initSD... ");
    a = myLCD.initSD();
    Serial.println(a, HEX);
    
    b8.enable((a==0x06));
    b8.draw();
    
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
        else if (b8.check()) {
            Serial.print("findFile... ");
            a=myLCD.findFile("Sound1.wav");
            Serial.println(a, HEX);
            
            Serial.print("setVolume... ");
            a=myLCD.setVolume(66);
            Serial.println(a, HEX);
            
            digitalWrite(13, HIGH);
            Serial.print("playSoundSD... ");
            a=myLCD.playSoundSD("Sound1.wav");
            digitalWrite(13, LOW);

            while (!mySerial.available());
            Serial.println(mySerial.read(), HEX);
            delay(200);
        }
    }
    myLCD.setFont(0);
    myLCD.setFontSolid(true);
    myLCD.gText( 250, 225, ftoa((millis()-l)/1000, 3, 8));
    l=millis();
}







