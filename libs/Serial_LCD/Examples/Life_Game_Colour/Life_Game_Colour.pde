///
/// @file 	Life_Game_Colour.pde
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

#include "proxySerial.h"
#include "Serial_LCD.h"

// test release
#if SERIAL_LCD_RELEASE < 333
#error required SERIAL_LCD_RELEASE 333
#endif

// Beware of RAM limitations
#define ROWS 20 // max 80
#define COLS 15 // max 60
#define PERCENT 70 // % dead

// bits 7 . 6 . 5 . 4 . 3 . 2 . 1 . 0
// [3] = dead 0, alive 1
// [2..0] = age 0-7 

#define ALIVE 0x08
#define DEAD  0x00

#define NMASK 0x08
#define AGEMASK 0x07

//#define mySerial Serial1
ProxySerial myPort(&mySerial); 
Serial_LCD myLCD(&myPort); 

uint8_t _screen[2][ROWS][COLS]; 
uint32_t t; 
uint8_t scanline; 
uint8_t df; 
uint8_t rf; 
uint16_t clut[16]; 
uint16_t generation; 

void new_game() {
  uint8_t i, j; 
  if(rf == df) {
    for(i=0; i<ROWS; i++) {
      for(j=0; j<COLS; j++) {
        if(random(100)>PERCENT) {
          _screen[rf^1][i][j] = ALIVE; 
        } 
        else {
          _screen[rf^1][i][j] = DEAD; 
        }
        myLCD.rectangle(4*i, 4*j, 4*i+3, 4*j+3, clut[_screen[rf^1][i][j]]);
      }
    }
  }
  rf ^= 1; 
  generation=0; 
  t=millis(); 
}




void setup() {
  Serial.begin(9600); 
  Serial.print("mySerial.begin...");

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
  Serial.println("done");


  Serial.print("myLCD.begin...");
  myLCD.begin(4); 
  Serial.println("done");

  //  myLCD.setSpeed(57600); 
  //  mySerial.begin(57600); 
  //  myLCD.setSpeed(115200); 
  //  mySerial.begin(115200); 

  myLCD.setOrientation(0x03); 
  myLCD.setPenSolid(true); 
  myLCD.setTouch(true); 


  // 0-7 dying: cold colours
  // from green to blue to black
  clut[ 0] = myLCD.setColour(  0, 255,   0); 
  clut[ 1] = myLCD.setColour(  0, 204,  51); 
  clut[ 2] = myLCD.setColour(  0, 153, 102); 
  clut[ 3] = myLCD.setColour(  0, 102, 153); 
  clut[ 4] = myLCD.setColour(  0,  51, 204); 
  clut[ 5] = myLCD.setColour(  0,   0, 255); 
  clut[ 6] = myLCD.setColour(  0,   0, 127); 
  clut[ 7] = myLCD.setColour(  0,   0,   0); 


  // 8-15 living: hot colours
  // from red to orange to yellow
  clut[ 8] = myLCD.setColour(255,   0,   0); 
  clut[ 9] = myLCD.setColour(255,  43,   0); 
  clut[10] = myLCD.setColour(255,  85,   0); 
  clut[11] = myLCD.setColour(255, 127,   0); 
  clut[12] = myLCD.setColour(255, 171,   0); 
  clut[13] = myLCD.setColour(255, 213,   0); 
  clut[14] = myLCD.setColour(255, 255,   0); 
  clut[15] = myLCD.setColour(255, 255, 255); 

  myLCD.setFontSolid(true); 
  myLCD.setFont(3); 
  myLCD.gText(40, 20, "Life Game"); 

  myLCD.setFont(2); 
  myLCD.gText(40+16*8, 80, "Death"); 
  for (uint8_t i=0; i<8; i++) 
    myLCD.rectangle(128+40+16*i, 60, 128+40+16*i+15, 60+16, clut[i]); 

  myLCD.gText(40+16*0, 80, "Life"); 
  for (uint8_t i=8; i<16; i++) 
    myLCD.rectangle(40+16*i-128, 60, 40+16*i+15-128, 60+16, clut[i]); 

  myLCD.setFont(0); 
  myLCD.gText(40, 120, "Touch to stop the game."); 

  delay(4000); 

  myLCD.clear(); 

  randomSeed(analogRead(0)); 
  df = 0; 
  rf = 0; 
  new_game(); 

}


void update_pixel(uint8_t i, uint8_t j, uint8_t neighbours) {
  if( bitRead(_screen[rf][i][j], 3) ) {
    // alive
    if((neighbours < 2) || (neighbours > 3)) { 
      _screen[rf^1][i][j] = DEAD;   // zero age dead pixel      // 0 pass
    }  
    else {
      _screen[rf^1][i][j] = _screen[rf][i][j] + 1;             // 8-15 living
      if (_screen[rf^1][i][j] > 0x0f) { 
        _screen[rf^1][i][j] = 0x0f; 
      } 
    }
  }   
  else {
    // dead
    if((neighbours == 3)) {
      _screen[rf^1][i][j] = ALIVE; // zero age alive pixel     // 8 born
    }     
    else { 
      _screen[rf^1][i][j] = _screen[rf][i][j] + 1;             // 0-7 dying
      if (_screen[rf^1][i][j] > 0x07) { 
        _screen[rf^1][i][j] = 0x07; 
      } 
    }
  }

  //  Serial.print(i, DEC); 
  //  Serial.print("\t");  
  //  Serial.print(j, DEC); 
  //  Serial.print("\t"); 
  //  Serial.print(_screen[rf][i][j], HEX); 
  //  Serial.print("\t"); 
  //  Serial.print(clut[_screen[rf][i][j]]); 
  //  Serial.print("\n"); 
  //  myLCD.point(i, j, clut[_screen[rf][i][j]]); 

  if(_screen[rf][i][j]!=_screen[rf^1][i][j]) 
    myLCD.rectangle(4*i, 4*j, 4*i+3, 4*j+3, clut[_screen[rf^1][i][j]]); 
}

void loop() {
  uint8_t i, j; 
  uint8_t neighbours; 

  generation++; 

  for(i=1; i<ROWS-1; i++) {
    for(j=1; j<COLS-1; j++) {
      neighbours  = bitRead(_screen[rf][i-1][j-1], 3); 
      neighbours += bitRead(_screen[rf][i-1][j  ], 3); 
      neighbours += bitRead(_screen[rf][i-1][j+1], 3); 
      neighbours += bitRead(_screen[rf][i  ][j-1], 3); 
      neighbours += bitRead(_screen[rf][i  ][j+1], 3); 
      neighbours += bitRead(_screen[rf][i+1][j-1], 3); 
      neighbours += bitRead(_screen[rf][i+1][j  ], 3); 
      neighbours += bitRead(_screen[rf][i+1][j+1], 3); 
      update_pixel(i,j, neighbours); 
    }
  }
  for(i=1; i<COLS-1; i++) {
    neighbours  = bitRead(_screen[rf][0     ][i-1], 3); 
    neighbours += bitRead(_screen[rf][0     ][i+1], 3); 
    neighbours += bitRead(_screen[rf][1     ][i-1], 3); 
    neighbours += bitRead(_screen[rf][1     ][i  ], 3); 
    neighbours += bitRead(_screen[rf][1     ][i+1], 3); 
    neighbours += bitRead(_screen[rf][ROWS-1][i-1], 3); 
    neighbours += bitRead(_screen[rf][ROWS-1][i  ], 3); 
    neighbours += bitRead(_screen[rf][ROWS-1][i+1], 3); 
    update_pixel(0, i, neighbours); 

    neighbours  = bitRead(_screen[rf][ROWS-2][i-1], 3); 
    neighbours += bitRead(_screen[rf][ROWS-2][i  ], 3); 
    neighbours += bitRead(_screen[rf][ROWS-2][i+1], 3); 
    neighbours += bitRead(_screen[rf][ROWS-1][i-1], 3); 
    neighbours += bitRead(_screen[rf][ROWS-1][i+1], 3); 
    neighbours += bitRead(_screen[rf][0     ][i-1], 3); 
    neighbours += bitRead(_screen[rf][0     ][i  ], 3); 
    neighbours += bitRead(_screen[rf][0     ][i+1], 3); 
    update_pixel(ROWS-1,i, neighbours); 
  }
  for(i=1; i<ROWS-1; i++) {
    neighbours  = bitRead(_screen[rf][i-1][0     ], 3); 
    neighbours += bitRead(_screen[rf][i+1][0     ], 3); 
    neighbours += bitRead(_screen[rf][i-1][1     ], 3); 
    neighbours += bitRead(_screen[rf][i  ][1     ], 3); 
    neighbours += bitRead(_screen[rf][i+1][1     ], 3); 
    neighbours += bitRead(_screen[rf][i-1][COLS-1], 3); 
    neighbours += bitRead(_screen[rf][i  ][COLS-1], 3); 
    neighbours += bitRead(_screen[rf][i+1][COLS-1], 3); 
    update_pixel(i, 0, neighbours); 

    neighbours  = bitRead(_screen[rf][i-1][COLS-1], 3); 
    neighbours += bitRead(_screen[rf][i+1][COLS-1], 3); 
    neighbours += bitRead(_screen[rf][i-1][COLS-2], 3); 
    neighbours += bitRead(_screen[rf][i  ][COLS-2], 3); 
    neighbours += bitRead(_screen[rf][i+1][COLS-2], 3); 
    neighbours += bitRead(_screen[rf][i-1][0     ], 3); 
    neighbours += bitRead(_screen[rf][i  ][0     ], 3); 
    neighbours += bitRead(_screen[rf][i+1][0     ], 3); 
    update_pixel(i,COLS-1, neighbours); 
  }

  neighbours  = bitRead(_screen[rf][1     ][0     ], 3); 
  neighbours += bitRead(_screen[rf][1     ][1     ], 3); 
  neighbours += bitRead(_screen[rf][0     ][1     ], 3); 
  neighbours += bitRead(_screen[rf][ROWS-1][0     ], 3); 
  neighbours += bitRead(_screen[rf][ROWS-1][1     ], 3); 
  neighbours += bitRead(_screen[rf][ROWS-1][COLS-1], 3); 
  neighbours += bitRead(_screen[rf][0     ][COLS-1], 3); 
  neighbours += bitRead(_screen[rf][1     ][COLS-1], 3); 
  update_pixel(0,0, neighbours); 

  neighbours  = bitRead(_screen[rf][1     ][0     ], 3); 
  neighbours += bitRead(_screen[rf][0     ][0     ], 3); 
  neighbours += bitRead(_screen[rf][0     ][COLS-2], 3); 
  neighbours += bitRead(_screen[rf][1     ][COLS-2], 3); 
  neighbours += bitRead(_screen[rf][1     ][COLS-1], 3); 
  neighbours += bitRead(_screen[rf][ROWS-1][COLS-1], 3); 
  neighbours += bitRead(_screen[rf][ROWS-1][COLS-2], 3); 
  neighbours += bitRead(_screen[rf][ROWS-1][0     ], 3); 
  update_pixel(0,COLS-1, neighbours); 

  neighbours  = bitRead(_screen[rf][ROWS-1][1     ], 3); 
  neighbours += bitRead(_screen[rf][ROWS-2][0     ], 3); 
  neighbours += bitRead(_screen[rf][ROWS-2][1     ], 3); 
  neighbours += bitRead(_screen[rf][ROWS-1][COLS-1], 3); 
  neighbours += bitRead(_screen[rf][ROWS-2][COLS-1], 3); 
  neighbours += bitRead(_screen[rf][0     ][COLS-1], 3); 
  neighbours += bitRead(_screen[rf][0     ][0     ], 3); 
  neighbours += bitRead(_screen[rf][0     ][1     ], 3); 
  update_pixel(ROWS-1,0, neighbours); 

  neighbours  = bitRead(_screen[rf][ROWS-1][COLS-2], 3); 
  neighbours += bitRead(_screen[rf][ROWS-2][COLS-2], 3); 
  neighbours += bitRead(_screen[rf][ROWS-2][COLS-1], 3); 
  neighbours += bitRead(_screen[rf][ROWS-1][0     ], 3); 
  neighbours += bitRead(_screen[rf][ROWS-2][0     ], 3); 
  neighbours += bitRead(_screen[rf][0     ][0     ], 3); 
  neighbours += bitRead(_screen[rf][0     ][COLS-1], 3); 
  neighbours += bitRead(_screen[rf][1     ][COLS-2], 3); 
  update_pixel(ROWS-1,COLS-1, neighbours); 

  rf ^= 1; 

  // generation
  myLCD.setFont(3); 
  myLCD.gText(210, 235-myLCD.fontY(), ftoa(generation, 0, 5)); 

  // calculation time in seconds
  myLCD.setFont(1); 
  myLCD.gText(280, 235-myLCD.fontY(), ftoa((millis()-t)/1000.0, 1, 4)); 
  t=millis(); 

  // touch to stop
  if (myLCD.getTouchActivity()>0) {
    myLCD.off(); 
    while(1); 
  }
}  













