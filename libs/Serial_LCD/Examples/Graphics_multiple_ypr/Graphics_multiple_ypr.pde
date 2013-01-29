/// 
/// @mainpage	Graphics Multiple Example
/// @details	Example with yaw-pitch-roll angles
/// @n 
/// @n 
/// @n @a	Developed with [embedXcode](http://embedXcode.weebly.com)
/// 
/// @author	Rei VILO
/// @author	http://embeddedcomputing.weebly.com
/// @date	Jul 12, 2012
/// @version	201
/// 
/// @copyright	© Rei VILO, 2012
/// @copyright	CC = BY NC SA
///
/// @see	ReadMe.txt for references
///


///
/// @file	Graphics_multiple_ypr.pde 
/// @brief	Main sketch
/// @details	<#details#>
/// @n @a	Developed with [embedXcode](http://embedXcode.weebly.com)
/// 
/// @author	Rei VILO
/// @author	http://embeddedcomputing.weebly.com
/// @date	Jul 12, 2012
/// @version	201
/// 
/// @copyright	© Rei VILO, 2012
/// @copyright	CC = BY NC SA
///
/// @see	ReadMe.txt for references
/// @n
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


#include "Wire.h"
#include "Serial_LCD.h"
#include "proxySerial.h"
#include "Graphics.h"

// IMU - Place here your own libraries
#include "I2C_IMU.h"
#include "I2C_Gyroscope.h"
#include "I2C_Magnetometer.h"
#include "I2C_Accelerometer.h"

// test release
#if GRAPHICS_RELEASE < 313
#error required GRAPHICS_RELEASE 313
#endif

// Uncomment for Arduino UNO
//#include "SoftwareSerial.h"
//SoftwareSerial mySerial(2, 3);

// Uncomment for Serial1
//#define mySerial Serial1

// Uncomment for I2C_Serial
#include "I2C_Serial.h"
I2C_Serial mySerial;

ProxySerial myPort(&mySerial);
Serial_LCD myLCD( &myPort); 

// IMU - Initialisation
I2C_IMU myIMU;
float ypr[3]; // yaw pitch roll angles

uint16_t x, y;
int t=0;
uint16_t l;
gMultiple myMultiple;


void setup() 
{
  // Wire.begin only once
  Wire.begin();
  
  myIMU.begin(200);
  
  mySerial.begin(9600);
  myLCD.begin(4);
  
  // Arduino: max 38400
  myLCD.setSpeed(38400);
  mySerial.begin(38400);
  
  // chipKIT and I2C: 57600
  //  myLCD.setSpeed(57600);
  //  mySerial.begin(57600);
  
  myLCD.setOrientation(0x03);
  
  myLCD.setPenSolid(true);
  myLCD.setFontSolid(true);
  
  myLCD.circle(myLCD.maxX()-20, myLCD.maxY()-20, 10, redColour);
  
  myLCD.setFont(0);
  myLCD.gText( 0, myLCD.maxY()-30,  myLCD.WhoAmI());
  myLCD.gText( 0, myLCD.maxY()-15,  myIMU.WhoAmI());
  
  myLCD.setTouch(true);
  
  myMultiple.dDefine(&myLCD, 
                     20, 15, myLCD.maxX()-40, myLCD.maxY()-20-45, 
                     "yaw", -180, 180, blueColour,
                     "pitch", -90, 90, greenColour,
                     "roll", -90, 90, redColour,
                     ".", 0, 0, 0x0000,
                     0, 4, 0, false, 
                     blackColour, whiteColour, yellowColour);
  
  l = millis();
}


void loop() 
{
  // IMU - Acquire and read angles
  myIMU.get();
  myIMU.YawPitchRoll(ypr[0], ypr[1], ypr[2]);
  
  // Display angles
  myMultiple.draw(ypr[0], ypr[1], ypr[2]);
  
  if (myLCD.getTouchActivity()) {
    myLCD.getTouchXY(x, y);
    
    if ((x>myLCD.maxX()-40) && (y>myLCD.maxY()-40)) {
      myLCD.off();
      while(true);
    }
  }
  
  myLCD.setFont(0);
  myLCD.gText( 250, myLCD.maxY()-30,  String((uint16_t)(millis()-l)));
  l = millis();
}
