///
/// @file 	energy_main.pde
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

// Include application, user and local libraries
#include "proxySerial.h"
#include "Serial_LCD.h"
#include "Energy.h"
#include "GUI.h"

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

// Define variables and constants
ProxySerial myPort(&mySerial);
Serial_LCD myLCD(&myPort);
Energy myEnergy;
uint32_t ui;
button b3;

// Add setup code 
void setup() {                
  Serial.begin(9600);
  Serial.print("\n\n\n ***\n\n");
  
    // === Serial port initialisation ===
#if defined(__I2C_Serial__)
  Wire.begin();
  mySerial.begin(9600);

#elif defined(__AVR__)  || defined(__AVR_ATmega328P__) | defined(__AVR_ATmega328P__)
  mySerial.begin(9600);

#elif defined(__PIC32MX__) 
  Serial1.begin(9600);

#endif 
  // === End of Serial port initialisation ===

  Serial.begin(9600);
  Serial.print("\n\n\n***\n");

  myLCD.begin(4);
  myLCD.setTouch(true);

  myLCD.setFontSolid(true);    
  myLCD.setFont(0);
  myLCD.gText( 0, 210, myLCD.WhoAmI());

  b3.dStringDefine(&myLCD, 240, 160, 60, 40, "Stop",   whiteColour, redColour);
  b3.enable(true);
  b3.draw();

  myEnergy.begin(&myLCD, 3); // 3 second
  
  Serial.println("Energy set to 3 s");
  ui = millis();
}

uint8_t c;
uint16_t x, y;

// Add loop code 
void loop() {
  myLCD.setFontSolid();
  myLCD.setFont(1);
  myLCD.gText(0, 0, ftoa(millis()-ui, 0, 6));

  c = myLCD.getTouchActivity();
  myEnergy.check( c );

  Serial.print(ftoa(millis()-ui, 0, 6));
  Serial.print("\t");
  Serial.println(c, DEC);



  if ( c ) {
    myLCD.getTouchXY(x, y);
    myLCD.setFont(0);
    myLCD.gText(200, 0,  ftoa(x, 0, 5), greenColour); 
    myLCD.gText(200, 15, ftoa(y, 0, 5), redColour); 

    // quit
    if (b3.check()) {
      myLCD.off();
      Serial.print("\n END");
      while(true);
    }

    ui = millis();
  }

}

