// 
// 4D Systems uLCD-uLED-uVGA Serial_LCD Library Suite
// Arduino 0023 and 1.0, chipKIT MPIDE 0023, Wiring 1.0
// ----------------------------------
//
// Sep 01, 2012 release 313
// See README.txt
//
// © Rei VILO, 2010-2013
//   CC = BY NC SA
//   http://embeddedcomputing.weebly.com/serial-lcd.html
//   http://github.com/rei-vilo/Serial_LCD
//
// For 
//   4D Systems Goldelox and Picaso SGC Command Set
//   http://www.4dsystems.com.au/
//

// Library header
#include "proxySerial.h"

// Other libraries
#include "Stream.h"

// Utilities

String ftoa(float number, uint8_t precision, uint8_t size) {
  /// @see mem, 16.07.2008 
  /// http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1207226548/6#6
  
  // prints val with number of decimal places determine by precision
  // precision is a number from 0 to 6 indicating the desired decimial places
  // example: printDouble(3.1415, 2); // prints 3.14 (two decimal places)
  
  // Added rounding, size and overflow #
  // ftoa(343.1453, 2, 10) -> "    343.15"
  // ftoa(343.1453, 4,  7) -> "#      "
  // avenue33, April 10th, 2010
  
  String s = "";
  
  // Negative 
  if (number < 0.0)  {
    s = "-";
    number = -number;
  }
  
  float rounding = 0.5;
  for (uint8_t i = 0; i < precision; ++i)    rounding /= 10.0;
  
  number += rounding;
  s += String(int(number));  // prints the integer part
  
  if(precision > 0) {
    s += ".";                // prints the decimal point
    uint32_t frac;
    uint32_t mult = 1;
    uint8_t padding = precision -1;
    while(precision--)     mult *= 10;
    
    frac = (number - (uint64_t)(number)) * mult;
    
    uint32_t frac1 = frac;
    while(frac1 /= 10)    padding--;
    while(padding--)      s += "0";
    
    s += String(frac, DEC) ;  // prints the fractional part
  }
  
  if ( (size>0) & (s.length()>size) )  return("#");
  while( s.length()<size ) s = " "+s;
  
  return s;
}


String u64toa(uint64_t number, uint8_t precision, uint8_t size) {
  String s = "";
  
  // s.length()<precision+2 ensure a leading 0 for 0.123
  // precision>0            but only if there's a decimal part
  while ( (number>0) || ((s.length()<precision+2) && (precision>0)) ) {
    s = String("0123456789")[number%10] + s;
    if (s.length()==precision) s = "."+s;
    number /= 10;
  }
  if ( (size>0) && (s.length()>size) )  return("#");
  
  while ( s.length()<size )  s = " "+s;
  return s;
}


String ttoa(uint64_t number, uint8_t precision, uint8_t size) {
  String s = "";
  // Automatic selection of the time unit: ms, s, mn, h
  
  if (number<(uint64_t)1000) {			// ms
    s = "ms";
    number *= 1000;
    precision = 0; // no digital place for ms
  } else if (number<(uint64_t)60000) { 		// s = ms/1000
    s="s";    
  } else if (number<(uint64_t)3600000.0) {      // mn = ms/1000/60
    number /= 60;
    s = "mn";
  } else {      				// h = ms/1000/60/60
    number /= 3600;
    s = "h";
  }
  
  if ( precision<3 ) for (uint8_t i=precision; i<3; i++) number /= 10;
  else precision = 3;
  
  return u64toa(number, precision, max(0, size-s.length())) + s;
}


String htoa(uint32_t number, uint8_t size) {
  String s = "";
  while ( number>0 ) {
    s = String("0123456789abcdef")[number%16] + s;
    number = number>>4;
  }
  if ( (size>0) && (s.length()>size) )  return("#");
  
  while ( s.length()<size )  s = "0"+s;
  return s;
}

// Object

ProxySerial::ProxySerial(Stream * proxyPort) {
  _proxyPort = proxyPort; 
#if (SECURITYDELAY>0)
  _millis = millis() + securityDelay;
#endif
}

#if (SECURITYDELAY>0)
void ProxySerial::_checkSpeed() {
  //    while(!(millis()-_millis > 3));    // time in ms
  while( millis() < _millis );    // time in ms, the same !> = <
  _millis = millis() + securityDelay;
}
#endif

void ProxySerial::begin(uint16_t dummy) {  // speed to be managed at serial port level
#if (SECURITYDELAY>0)
  _checkSpeed();
#endif
  //  _proxyPort->begin(dummy);
}

void ProxySerial::print(int8_t i) { 
#if (SECURITYDELAY>0)
  _checkSpeed();
#endif
  _proxyPort->print((char)i); 
}

void ProxySerial::print(uint8_t ui) { 
#if (SECURITYDELAY>0)
  _checkSpeed();
#endif  
  _proxyPort->print((char)ui); 
};

void ProxySerial::print(int16_t i) { 
#if (SECURITYDELAY>0)
  _checkSpeed();
#endif  
  _proxyPort->print((char)highByte(i)); 
  _proxyPort->print((char)lowByte(i)); 
};

void ProxySerial::print(uint16_t ui) { 
#if (SECURITYDELAY>0)
  _checkSpeed();
#endif  
  _proxyPort->print((char)highByte(ui)); 
  _proxyPort->print((char)lowByte(ui)); 
};

void ProxySerial::print(char c) { 
  _proxyPort->print((char)c); 
};

void ProxySerial::print(String s) { 
  for (uint8_t i=0; i<s.length(); i++)     
    _proxyPort->print(s.charAt(i));
}

uint8_t ProxySerial::read() { 
#if (SECURITYDELAY>0)
  _checkSpeed();
#endif  
  return _proxyPort->read(); 
}

uint16_t ProxySerial::read16() { 
#if (SECURITYDELAY>0)
  _checkSpeed();
#endif  
  while (_proxyPort->available()<2);
  return (uint16_t)(_proxyPort->read() << 8) + _proxyPort->read(); 
}

int8_t ProxySerial::available() { 
#if (SECURITYDELAY>0)
  _checkSpeed();
#endif  
  return _proxyPort->available(); 
}

void ProxySerial::flush() {  
#if (SECURITYDELAY>0)
  _checkSpeed();
#endif  
  _proxyPort->flush(); 
}















