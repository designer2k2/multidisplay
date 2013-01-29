///
/// @file 	colour_main.pde
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

  myLCD.begin();

  //  // === Serial port speed change ===
  //  myLCD.setSpeed(38400);
  //#if defined(__I2C_Serial__)
  //  mySerial.begin(38400);
  //
  //#elif defined(__AVR__)  || defined(__AVR_ATmega328P__) | defined(__AVR_ATmega328P__)
  //  mySerial.begin(38400);
  //
  //#elif defined(__PIC32MX__) 
  //  Serial1.begin(38400);
  //
  //#endif 
  //  // === End of Serial port speed change ===



  int8_t i;


  myLCD.setPenSolid(true);

  i=0;  
  myLCD.dRectangle(20*i, 0, 17, 100, blackColour);
  i++;  
  myLCD.dRectangle(20*i, 0, 17, 100, whiteColour);
  i++;  
  myLCD.dRectangle(20*i, 0, 17, 100, redColour);
  i++;  
  myLCD.dRectangle(20*i, 0, 17, 100, greenColour);
  i++;  
  myLCD.dRectangle(20*i, 0, 17, 100, blueColour);
  i++;  
  myLCD.dRectangle(20*i, 0, 17, 100, yellowColour);
  i++;  
  myLCD.dRectangle(20*i, 0, 17, 100, cyanColour);
  i++;  
  myLCD.dRectangle(20*i, 0, 17, 100, orangeColour);
  i++;  
  myLCD.dRectangle(20*i, 0, 17, 100, magentaColour);
  i++;
  myLCD.dRectangle(20*i, 0, 17, 100, violetColour);
  i++;
  myLCD.dRectangle(20*i, 0, 17, 100, grayColour);


  i=0;  
  myLCD.dRectangle(20*i, 100, 17, 100, myLCD.reverseColour(blackColour));
  i++;  
  myLCD.dRectangle(20*i, 100, 17, 100, myLCD.reverseColour(whiteColour));
  i++;  
  myLCD.dRectangle(20*i, 100, 17, 100, myLCD.reverseColour(redColour));
  i++;  
  myLCD.dRectangle(20*i, 100, 17, 100, myLCD.reverseColour(greenColour));
  i++;  
  myLCD.dRectangle(20*i, 100, 17, 100, myLCD.reverseColour(blueColour));
  i++;  
  myLCD.dRectangle(20*i, 100, 17, 100, myLCD.reverseColour(yellowColour));
  i++;  
  myLCD.dRectangle(20*i, 100, 17, 100, myLCD.reverseColour(cyanColour));
  i++;  
  myLCD.dRectangle(20*i, 100, 17, 100, myLCD.reverseColour(orangeColour));
  i++;  
  myLCD.dRectangle(20*i, 100, 17, 100, myLCD.reverseColour(magentaColour));
  i++;
  myLCD.dRectangle(20*i, 100, 17, 100, myLCD.reverseColour(violetColour));
  i++;
  myLCD.dRectangle(20*i, 100, 17, 100, myLCD.reverseColour(grayColour));


}


void loop() {
}























