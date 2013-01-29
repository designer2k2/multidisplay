//
// SC16IS750 I2C slave bridge to serial
// Arduino + chipKIT Library
//
// Sep 01, 2012 release 308
// see README.txt
//
// © Rei VILO, 2010-2013
//   CC = BY NC SA
//   http://embeddedcomputing.weebly.com/serial-lcd.html
//   http://github.com/rei-vilo/Serial_LCD
//
//

#include "Wire.h"
#include "I2C_Serial.h"
#include "Print.h"

// ---------------- Functions

// Write or read according to framework
static void _send(uint8_t ui) {
#if ( (defined(ARDUINO) && (ARDUINO >= 100)) || defined(ENERGIA) || defined(WIRING) )

//  Wire.write(ui);
  Wire.send(ui);
#else
  Wire.send(ui);
#endif
}

static uint8_t _receive() {
#if ( (defined(ARDUINO) && (ARDUINO >= 100)) || defined(ENERGIA) || defined(WIRING) )
	//changed
  //return Wire.read();
	return Wire.receive();
#else
  return Wire.receive();
#endif
}

// Writes value to address register on device
static void _writeTo(uint8_t device, uint8_t address, uint8_t value) {
    Wire.beginTransmission(device); // start transmission to device 
    _send(address);             // send register address
    _send(value);               // send value to write
    Wire.endTransmission();         // end transmission
}


// Reads number uint8_ts starting from address register on device uint8_to buffer array
static void _readFrom(uint8_t device, uint8_t address, uint8_t number, uint8_t buffer[]) {
    Wire.beginTransmission(device); // start transmission to device 
    _send(address);             // sends address to read from
    Wire.endTransmission();         // end transmission
    
    Wire.beginTransmission(device);    // start transmission to device
    Wire.requestFrom(device, number);  // request number uint8_ts from device
    
    uint8_t i = 0;
    while(Wire.available())       // device may send less than requested (abnormal)
    { 
        buffer[i] = _receive(); // receive a uint8_t
        i++;
    }
    Wire.endTransmission();       //end transmission
}



static uint8_t _readByteFrom(int8_t device, uint8_t address) {
    Wire.beginTransmission(device); // start transmission to device  
    _send(address);             // send address to read from
    Wire.endTransmission();         // end transmission
    
    Wire.beginTransmission(device); // start transmission to device
    Wire.requestFrom(device, 1);    // request 1 uint8_t from device
    
    while(!Wire.available());      // device may send less than requested (abnormal)
    uint8_t b = _receive();    // receive a uint8_t
    Wire.endTransmission();        // end transmission
    return b;
}

// ---------------- Class

I2C_Serial::I2C_Serial(uint8_t number) // constructor
{
    _address=0x48;                  // 0x90 a0/a1=+/+ : default I2C serial port
    if (number==1) _address=0x49;        // 0x92 a0/a1=+/- : secondary I2C serial port
    else if (number==2) _address=0x4c;   // 0x98 a0/a1=-/+ : RFID ID-2 sensor board
    else if (number==12) _address=0x4d;  // 0x9a a0/a1=-/- : RFID ID-12 sensor board
}

String I2C_Serial::WhoAmI() 
{
    String s="";
    if (_address < 0x10) s="0";
    s = s + String(_address, 0x10) +"h ";
    s = s + "SC16IS750 I2C Serial (actuator/sensor)";
    return s;
}

void I2C_Serial::begin(long baud) 
{  
    _writeTo(_address, 0x0e << 3, 0x01 << 3); // software reset
    
    // See datasheet section 7.8 for configuring the
    // "Programmable baud rate generator"
    uint16_t divisor = (uint16_t)(14745600/baud/16);
    
    _writeTo(_address, 0x03 << 3, 0b10000000); // Line Control Register - divisor latch enable
    _writeTo(_address, 0x00 << 3, lowByte(divisor));  // Divisor Latch LSB
    _writeTo(_address, 0x01 << 3, highByte(divisor)); // Divisor Latch MSB

    //  _writeTo(_address, 0x03 << 3, 0xbf); // Line Control Register - access EFR register
    //  _writeTo(_address, 0x02 << 3, 0x00); // enable enhanced registers
    _writeTo(_address, 0x03 << 3, 0b00000011); // Line Control Register - 8 data bit, 1 stop bits, no parity
    _writeTo(_address, 0x02 << 3, 0x06); // reset TXFIFO, reset RXFIFO, non FIFO mode
    _writeTo(_address, 0x02 << 3, 0x01); // enable FIFO mode   
}


// write and read functions
#if ( (defined(ARDUINO) && (ARDUINO >= 100)) || defined(ENERGIA) )
size_t I2C_Serial::write(uint8_t byte)
{
    _writeTo(_address, 0x00 << 3, byte);
	return 1;    
}
#else
void I2C_Serial::write(uint8_t byte) 
{
    //  //                                   start I2C    
    //  Wire.beginTransmission(_address); // talk to device at _address
    //  Wire.send(0x00);                   // command 
    //  Wire.send(i);                     // value  
    //  Wire.endTransmission();           // end I2C  
    _writeTo(_address, 0x00 << 3, byte);
}
#endif


int I2C_Serial::read() 
{
    //  Wire.beginTransmission(_address); //start transmission to ACC 
    //  Wire.send(0x00);                   // command 
    //  Wire.endTransmission(); //end transmission
    //
    //    Wire.beginTransmission(_address); //start transmission to ACC
    //  Wire.requestFrom(_address, 1);    // request 6 uint8_ts from ACC
    //
    //  while (!Wire.available())    //ACC may send less than requested (abnormal)
    //    return  Wire.receive(); // receive a uint8_t
    //  Wire.endTransmission(); //end transmission
    
    return _readByteFrom(_address, 0x00 << 3);
}


// tests functions
boolean I2C_Serial::test() 
{
    char a = (char)random(0x00, 0xff);
    _writeTo(_address, 0x07 << 3, a); // Scratch Pad Register
    delay(3);
    char b=_readByteFrom(_address, 0x07 << 3); // Scratch Pad Register
    return (a==b);
}

void I2C_Serial::loopback(boolean flag) 
{
    uint8_t b =_readByteFrom(_address, 0x04 << 3); // Modem Control Register
    bitWrite(b, 4, flag); 
    _writeTo(_address, 0x04 << 3, b); // Modem Control Register
}

// I/O functions
// if pin=0..7, mode or val or result with 0=INPUT or 1=OUPUT
// if pin=ALL, mode or val or result with bit map
void I2C_Serial::pinMode(uint8_t pin, uint8_t mode) 
{
    if ( pin==ALL ) {
        _writeTo(_address, 0x0a << 3, mode);  
    } else {
        uint8_t b=_readByteFrom(_address, 0x0a << 3);
        bitWrite(b, pin, mode);
        _writeTo(_address, 0x0a << 3, b);
    }

//  uint8_t b=_readByteFrom(_address, 0x0a << 3);
//  Serial.print("in-out ");	  
//  Serial.println(b, BIN);	  
}

void I2C_Serial::digitalWrite(uint8_t pin, uint8_t value) 
{
    if ( pin==ALL ) {
        _writeTo(_address, 0x0b << 3, value);
    } else {
        uint8_t b=_readByteFrom(_address, 0x0b << 3);
        bitWrite(b, pin, value);
        _writeTo(_address, 0x0b << 3, b);
    }
}  

int I2C_Serial::digitalRead(uint8_t pin) 
{
    if ( pin==ALL ) {
        return _readByteFrom(_address, 0x0b << 3);
    } else {
        return bitRead(_readByteFrom(_address, 0x0b << 3), pin);        
    }    
}

void I2C_Serial::reset(uint8_t pin, uint8_t value, uint8_t ms) 
{
    if ( pin==ALL ) {
        _writeTo(_address, 0x0a << 3, 0xff*OUTPUT);  
        _writeTo(_address, 0x0b << 3, 0xff*value);
        delay(ms);
        _writeTo(_address, 0x0b << 3, 0xff*(~value));        
    } else {
        uint8_t b=_readByteFrom(_address, 0x0a << 3);
        bitWrite(b, pin, OUTPUT);
        _writeTo(_address, 0x0a << 3, b);
        
        b=_readByteFrom(_address, 0x0b << 3);
        bitWrite(b, pin, value);
        _writeTo(_address, 0x0b << 3, b);
        
//        Serial.print(" BIN ");
//        Serial.print(b, BIN);
        
        delay(ms);
        bitWrite(b, pin, ~value);
        _writeTo(_address, 0x0b << 3, b);
        
//        Serial.print(" BIN ");
//        Serial.print(b, BIN);
        
    }
}


// management functions
int I2C_Serial::available() 
{
    return _readByteFrom(_address, 0x09 << 3); // Receiver FIFO Level register
}

int I2C_Serial::peek() 
{
    if (available()==0) return -1;
    else return 1; // ?
}

uint8_t I2C_Serial::free() 
{
    return _readByteFrom(_address, 0x08 << 3); // Transmitter FIFO Level register
}

void I2C_Serial::flush() 
{
    _writeTo(_address, 0x02 << 3, 0x06); // reset TXFIFO, reset RXFIFO, non FIFO mode
    _writeTo(_address, 0x02 << 3, 0x01); // enable FIFO mode   
}
