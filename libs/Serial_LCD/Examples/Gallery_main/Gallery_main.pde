///
/// @file 	Gallery_main.pde
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
//

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
#include "Gallery.h"

// test release
#if GALLERY_RELEASE < 309
#error required GALLERY_RELEASE 309
#endif

// === Serial port choice ===

// uncomment for I2C serial interface
//#define __I2C_Serial__

// --- I2C Case -
#if defined(__I2C_Serial__)
#include "Wire.h"
#include "I2C_Serial.h"
I2C_Serial mySerial(0);

// --- Arduino SoftwareSerial Case - Arduino only
#elif defined(__AVR_ATmega328P__) 
#include "NewSoftSerial.h"
NewSoftSerial mySerial(2, 3); // RX, TX

// --- chipKIT HardwareSerial Case - chipKIT
#elif defined(__PIC32MX__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define mySerial Serial1

#else
#error Non defined board
#endif 

// === End of Serial port choice ===

ProxySerial myPort(&mySerial);
Serial_LCD myLCD( &myPort); 

uint16_t x, y;
uint32_t l;
uint8_t a;

Gallery myGallery;


void setup() {
  Serial.begin(19200);
  Serial.print("\n\n\n***\n");

  // === Serial port initialisation ===
#if defined(__I2C_Serial__)
  Serial.print("i2c\n");
  Wire.begin();
  mySerial.begin(9600);

#elif defined(__AVR_ATmega328P__)
  Serial.print("Arduino: NewSoftSerial\n");
  mySerial.begin(9600);

#elif defined(__PIC32MX__) || defined(__AVR_ATmega2560__)
  Serial.print("chipKIT or mega2560: Serial1\n");
  Serial1.begin(9600);

#endif 
  // === End of Serial port initialisation ===

  Serial.print("\n begin 9600");
  myLCD.begin(4);
  Serial.print("\n LCD begin");

  // === Serial port speed change ===
  myLCD.setSpeed(38400);
#if defined(__I2C_Serial__)
  mySerial.begin(38400);

#elif defined(__AVR_ATmega328P__)
  mySerial.begin(38400);

#elif defined(__PIC32MX__) || defined(__AVR_ATmega2560__)
  Serial1.begin(38400);

#endif 
  // === End of Serial port speed change ===
  Serial.print("\n LCD begin 38400");

  Serial.print("\n readScreenGCI initSD");
  Serial.print("\t");
  a =   myLCD.initSD();
  Serial.print(a, HEX);

  Serial.print("\n readScreenGCI checkSD");
  Serial.print("\t");
  a =   myLCD.checkSD();
  Serial.print(a, HEX);

  myLCD.setOrientation(0x03);

  myLCD.setPenSolid(true);
  myLCD.setFontSolid(true);
  myLCD.setFont(0);

  Serial.print("\n readScreenGCI checkScreenType");
  Serial.print("\t");
  a =   myLCD.checkScreenType();
  Serial.print(a, HEX);  

  Serial.print("\n readScreenGCI WhoAmI");
  myLCD.gText( 0, 210, myLCD.WhoAmI());
  delay(500);

  Serial.print("\n readScreenGCI images.Gci");
  Serial.print("\t");
  a = myLCD.findFile("images.gci");
  Serial.print(a, HEX);

  Serial.print("\n readScreenGCI images.Dat");
  Serial.print("\t");
  a = myLCD.findFile("images.dat");
  Serial.print(a, HEX);

  Serial.print("\n myGallery.begin(images)");
  Serial.print("\t");
  a = myGallery.begin(&myLCD, "images");
  Serial.print(a, DEC);
  Serial.print(" images");

  Serial.print("\n > ");
  Serial.print(mySerial.available(), DEC);

  if ( a==0 ) {
    Serial.print("\n images not available");
    Serial.print("\n *** end");
    myLCD.off();
    while( true );
  }

  Serial.print("\n readScreenGCI setTouch");
  Serial.print("\t");
  a = myLCD.setTouch(true);
  Serial.print(a, HEX);  

  //  Serial.print("* readTextFile");
  //  Serial.print("\n");

  //  a = myLCD.readTextFile("images.dat", 7, callbackReadFile);
  //  Serial.print("* ");
  //  Serial.print(a, HEX);
  //  Serial.print("\n");

  //  a = myLCD.openTextFileDelimiter("images.dat", '\n', callbackReadFile);
  //  Serial.print("* ");
  //  Serial.print(a, HEX);
  //  Serial.print("\n");


  //  a = myLCD.readScreenFAT("160.gci");
  //  Serial.print(a, HEX);
  //  Serial.print("\n");
  //  delay(10000);

  Serial.print("\n > ");
  Serial.print(mySerial.available(), DEC);

}

uint8_t c;

void loop() {

  Serial.print("\n ... ");
  c = myLCD.getTouchActivity();
  Serial.print(c, HEX);
  if (c>0) {
    myLCD.off();
    while(true);
  }


  Serial.print("\n showNext (");
  a = myGallery.showNext();
  Serial.print(myGallery.index(), DEC);
  Serial.print(")\t");
  Serial.print(a, HEX);

  //
  //
  //
  //  myLCD.setFont(0);
  //  myLCD.setFontSolid(true);
  //  myLCD.gText( 250, 225, 0xffff, ftoa(millis()-l, 0, 6));
  //  l=millis();
  delay(3000);
}




