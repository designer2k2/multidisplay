///
/// @file 	Serial_LCD_main.pde
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
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__) // Arduino specific
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

uint8_t aa;

void setup() {
  Serial.begin(19200);
  Serial.print("\n\n\n***\n");

  // === Serial port initialisation ===
#if defined(__I2C_Serial__)
  Serial.print("i2c\n");
  Wire.begin();
  mySerial.begin(9600);

#elif defined(__AVR__)  || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328P__)
  Serial.print("avr\n");
  mySerial.begin(9600);

#elif defined(__PIC32MX__) 
  Serial.print("chipKIT\n");
  mySerial.begin(9600);

#endif 
  // === End of Serial port initialisation ===


  myLCD.begin();

delay(100);
  aa=myLCD.setOrientation(0x03);

delay(100);
  Serial.print("\n line \t");
  aa=myLCD.line(0,0,100,100, myLCD.setColour(0,255,0));
  Serial.print(aa, DEC);

delay(100);
  Serial.print("\n rectangle \t");
  aa=myLCD.rectangle(239,0,319,256, myLCD.setColour(255,0,0));
  Serial.print(aa, DEC);

delay(100);
  Serial.print("\n circle \t");
  aa=myLCD.circle(200,100, 50, myLCD.setColour(0,0,255));
  Serial.print(aa, DEC);

delay(200);
  Serial.print("\n setFont \t");
  aa=myLCD.setFont(2);
  Serial.print(aa, DEC);

delay(100);
  Serial.print("\n gText \t");
  aa=myLCD.gText(25, 25, "String");
  Serial.print(aa, DEC);

delay(100);
//  Serial.print("\n tText \t");
//  aa=myLCD.tText(0, 19, 0xffff, "WhoAmI? "+ myLCD.WhoAmI());
//  Serial.print(aa, DEC);

//  for (uint8_t i=0; i<myLCD.WhoAmI().length(); i++)   
//    Serial.print(myLCD.WhoAmI().charAt(i));
//  Serial.print("\n");

  delay(100);

delay(1000);
  Serial.print("\n triangle \t");
  aa=myLCD.triangle(160, 200, 80, 160, 60, 100, 0xffff);
  Serial.print(aa, DEC);

delay(100);
  Serial.print("\n setTouch \t");
  aa=myLCD.setTouch(true);
  Serial.print(aa, DEC);

  Serial.print("\n ");
}

uint16_t x, y;
uint8_t b;

void loop() {


  Serial.print("\n getTouchActivity \t");
  aa=myLCD.getTouchActivity();
  Serial.print(aa, DEC);


  if (aa) {
aa=0;
Serial.print(" * " );
    Serial.print("\t getTouchXY \t");
    aa=myLCD.getTouchXY(x, y);
delay(10);
    Serial.print(aa, DEC);

    Serial.print("\t");
    Serial.print(x, DEC );      
    Serial.print("\t");
    Serial.print(y, DEC );

    if (x>256) {
      myLCD.off();
      while(1);
    }
    Serial.println();
  }


  delay(200);

}












