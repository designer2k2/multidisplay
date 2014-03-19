/*
    Copyright 2009-13 Stephan Martin, Dominik Gummel

    This file is part of Multidisplay.

    Multidisplay is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Multidisplay is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Multidisplay.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "MultidisplayController.h"
#include "LCDController.h"
#include "RPMBoostController.h"
#include "Map16x1.h"

#include <stdlib.h>
#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <Arduino.h>
#include <EEPROM.h>

#include <digitalWriteFast.h>


//Lookup Table for the TypK:
//from 0-1350Â°C in steps of 50Â°C, the list is in microV according to that Temp.
const unsigned int MultidisplayController::tempTypK[] =
{
		0,
		1922,
		3891,
		5831,
		7731,
		9645,
		11599,
		13578,
		15577,
		17590,
		19612,
		21637,
		23660,
		25674,
		27673,
		29652,
		31611,
		33547,
		35460,
		37348,
		39212,
		41050,
		42863,
		44645,
		46396,
		48112,
		49790,
		51431
};


//Lookup Table for the Oilpressure: (12 Values)
//from 0-10Bar in steps of 1Bar, the list is in 12Bit Digital Reading when supplied with 5V and a 220Ohm Resistor in series
//(measuring the Voltage on the Sensor)
//it has a increasing Resistance with the Pressure.
const unsigned int MultidisplayController::tempVDOPressure[] =
{
		0,
		178,
		505,
		783,
		999,
		1189,
		1340,
		1476,
		1592,
		1699,
		1785,
		1865
};

//Lookup Table for the VDOtemperature (22 Values) Its Calibration curve 92-027-006
//from -30C-180C in steps of 10C, the list is in 12Bit Digital Reading when supplied with 5V and a 220Ohm Resistor in series
//(measuring the Voltage on the Sensor)
//it has a decreasing Resistance with the Temperature
const unsigned int MultidisplayController::tempVDOTemp[] =
{
		4049,
		4012,
		3944,
		3835,
		3672,
		3447,
		3157,
		2811,
		2433,
		2053,
		1694,
		1382,
		1121,
		899,
		724,
		582,
		474,
		387,
		319,
		263,
		218,
		182,
};

MultidisplayController::MultidisplayController() {
}

void  MultidisplayController::myconstructor() {

	IOport2 = 0b11111111;
	wire = TwoWire();

	FlashETimeU = 0;
	FlashTimeU = 0;
	ScreenSave = 0;
	time = 0;

	data.maxLd = 0;
	data.maxLdt=0;             //max LD for the screen

	DoCheck = 1;
	SerOut = SERIALOUT_BINARY;
#ifdef KWP1281_KLINE_DEBUG
	SerOut = SERIALOUT_DISABLED;
#endif

	buttonTime = 0;

	pinMode (LCDBRIGHTPIN, OUTPUT);
	//test
	pinMode (NORDSCHLEIFENPIN, INPUT);
	digitalWrite( NORDSCHLEIFENPIN, HIGH); // turn on pullup resistors
#ifdef LAMBDASTANDALONE
	pinMode (LAMBDASTANDALONE, INPUT);
	digitalWrite( LAMBDASTANDALONE, LOW);
#endif

	pinMode (SPEEDPIN, INPUT);
	digitalWrite (SPEEDPIN, LOW);

	pinMode (N75PIN, OUTPUT);
#if defined(MULTIDISPLAY_V2)
	//http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1235060559/15
	//Audi S2 N75
	//30Hz
	TCCR1B = (TCCR1B & 0b11111000) | 0x5;
#else

	//http://www.arduino.cc/playground/Main/TimerPWMCheatsheet
	//http://www.arcfn.com/2009/07/secrets-of-arduino-pwm.html
	//set pwm freq tp 30Hz
	//11
	//Audi S2 N75
	TCCR2B = (TCCR2B & 0b11111000) | 0x7;
	//	pinMode (FREEPWM2, OUTPUT);
	//	TCCR1B = (TCCR1B & 0b11111000) | 0x5;
#endif


	//set pin modes
	pinMode(DATAOUT, OUTPUT);
	pinMode(DATAIN, INPUT);
	pinMode(SPICLOCK, OUTPUT);
	digitalWrite(DATAOUT,LOW);
	digitalWrite(SPICLOCK,LOW);

//	Serial.begin(57600);
	Serial.begin(115200);

#if defined(MULTIDISPLAY_V2) && defined(DIGIFANT_KLINE)
//	Serial1.begin(9600);
//	Serial1.begin(6666);
	Serial1.begin(6667);

	DF_KlineSerialTime = 0;
	df_kline_status = DF_KLINE_STATUS_FRAME_COMPLETE;
	df_kline_active_frame = 0;
	df_kline_index = 0;
	df_kline_discarded_frames = 0;
	df_kline_last_frame_completely_received = 255;

	df_kline_freq_milliseconds = 0;
	df_kline_millis_last_frame_received = millis();
#endif

#if defined(MULTIDISPLAY_V2) && defined(KWP1281_KLINE)
	//arduino mega pinout diagram
	//http://www.pighixxx.com/downloads/arduino-mega-v2/
	//Serial1 RX: PD2
	//Serial1 TX: PD3
	//L: PA7
	//set L and K/TX to output
//	Serial1.end();
	UCSR1B = bitClear(UCSR1B, 4);
	UCSR1B = bitClear(UCSR1B, 3);
	DDRD |= 8;
	DDRD &= ~4;

	UCSR2B = bitClear(UCSR2B, 4);
	UCSR2B = bitClear(UCSR2B, 3);
	DDRH |= 2;
	DDRH &= ~1;

	UCSR3B = bitClear(UCSR3B, 4);
	UCSR3B = bitClear(UCSR3B, 3);
	DDRJ |= 2;
	DDRJ &= ~1;
//
//	//Test 2=PE4 3=PE5
//	//DDRE &= ~8;
//	DDRE |= 16;
//	pinMode (3,OUTPUT);

//	DDRD = bitSet(DDRD, 3);
//	pinMode (18, OUTPUT);
//	pinMode (19, INPUT);
//	digitalWrite (18,LOW);
//	pinMode (16, OUTPUT);
//	digitalWrite (16,LOW);
//	pinMode (2, OUTPUT);
//	DDRA |= 128;
//	DDRA = bitSet (DDRA,7);
	kwp1281_connect_failures=0;
	kwp1281_block_counter=0;
	kwp1281_state=KWP1281_STATE_NO_CONNECTION;
	kwp1281_kline_active_frame = 0;
	kwp1281_kline_index = 0;

#ifdef KWP1281_KLINE_DEBUG
	kwp1281_debug_printtime=0;
#endif

#endif

	//bluetooth module
#if defined(MULTIDISPLAY_V2) && defined(BLUETOOTH_ON_SERIAL2)
	Serial2.begin(115200);
#endif

	//Oled
#ifdef OLED
	oled = OledController();
	oled.myconstructor();
//	oled_flags = 0;
//	oled.reset();
#endif
/* ************** TES T***************** */
//	// Serial port choice
//	#define mySerial Serial3
//	ProxySerial myPort(&mySerial);
//	Serial_LCD myLCD( &myPort);
//
//	// Serial port initialisation
//	mySerial.begin(9600);
//	myLCD.begin();
//
//	// Serial port speed up
//	myLCD.setSpeed(38400);
//	mySerial.begin(38400);
	/* ************** TES T***************** */

	wire.begin();                  //Start the Wire Libary for the PCF8574

	expanderWrite2(IOport2); //Switch off all Devices

	//Print the Info:
#ifdef MULTIDISPLAY_V2
	Serial.println("MultiDisplay 2.0!");
#else
	Serial.println("MultiDisplay 1.1!");
#endif

#ifdef LCD
	lcdController.myconstructor();
//	lcdController.lcdShowIntro(INITTIME);                      //Shows the Into
	lcdController.init();
#endif

	//Init the Buttons:
	expanderWrite(0b10000011);        //This may needs to be modified when a third button is attached.

	serialFreq = SERIALFREQ;
	serialTime = millis();
	typK_state = TYPK_STATE_NEXT_SELECT_CHANNEL;
	typK_state_cur_channel = 0;

#ifdef READFROMEEPROM
	readSettingsFromEeprom();
#endif

	#ifdef V2DEVDEBUG
	v2devdebugflag = 0;
#endif

#if defined(MULTIDISPLAY_V2) && defined(GEAR_RECOGNITION)
	gear_state = GEAR_STATE_NEED_RECOGNITION;
	last_gear = 0;
#endif

	drawTime = 0;

	//LiquidCrystal lib doesnt works with the displaytech 204A :(
//	lcd.begin(20,4, LCD_5x10DOTS);
//	lcd.clear();

#ifdef BW_EFR_SPEEDSENSOR
	pinMode (BW_EFR_SPEEDSENSOR_PIN, INPUT);
	//enable pull-up
//	digitalWrite (BW_EFR_SPEEDSENSOR_PIN, HIGH);
	//normal Port Mode. OC4A, OC4B, OC4C disconnected
	TCCR4A = 0;
	//set prescalter to 8
	//1 timer tick earch 0.5usecs (prescaler 8)
	TCCR4B = 2;
	//enable noise canceller
	TCCR4B |= _BV (ICNC4);
	//enable input capture 4
	// 1 = trigger on rising edge!
	TCCR4B |= _BV (ICES4);
	//enable input capture IRQ
	bitSet (TIMSK4,ICIE4);
	// Enable overflow interrupt
	bitSet (TIMSK4,TOIE4);
	sei();
#endif

	pinMode (V2_SHIFTLED1PIN, OUTPUT);
	pinMode (V2_SHIFTLED2PIN, OUTPUT);
	pinMode (V2_SHIFTLED3PIN, OUTPUT);
}

#ifdef BW_EFR_SPEEDSENSOR
ISR(TIMER4_CAPT_vect) {
	if ( bitRead(TCCR4B,ICES4) == 0 ) {
		//falling edge
		data.efr_speed_reading = ICR4;
		//reset
		TCNT4 = 0;
	} else {
		//rising edgke
		TCNT4 = 0;
	}
	//toggle capture
	TCCR4B ^= _BV(ICES4);
}
ISR(TIMER4_OVF_vect) {
	//we have a timer overflow
	//set reading to max
	data.efr_speed_reading = 0xFFFF;
}
#endif

/*
 * only for V2 pcb!
 */
int MultidisplayController::read_adc_fast_mega (uint8_t channel){

	int adcvalue = 0;
	byte commandbits = B11000000; //command bits - start, mode, chn (3), dont care (3)
	uint8_t savedChannel = channel;  //Save the channel, to make it possible to deselect the MCP later.

	//choose the right MCP3208, depending on number to read.
	if(channel>8) 	{
		//Channels 9-16 on second MCP3208
		IOport2 = IOport2 & B01111101;     //Clears the Selection Pin for MCP 2 (Bitwise AND) (right is the MCP, left the LED)
		//to adapt the right Channel selection
		channel = channel - 8;
	} else 	{
		//Channels 1-8 on first MCP3208
		IOport2 = IOport2 & B01111110;     //Clears the Selection Pin for MCP 1 (Bitwise AND) http://www.programmers-corner.com/article/27
	}

	//allow channel selection
	commandbits|=((channel-1)<<3);

	expanderWrite2(IOport2); //Select ADC

	// setup bits to be written
	for (int i=7 ; i>=3 ; i--){

		//TODO testme
		uint8_t val = commandbits&1<<i;
		//DATAOUT
		if ( val ) {
			//asm(code : output operand list : input operand list [: clobber list]);
			asm("sbi %0,3" : : "I" (_SFR_IO_ADDR(PORTF)) );
		} else {
			asm("cbi %0,3" : : "I" (_SFR_IO_ADDR(PORTF)) );
		}

		//cycle clock
		//SPICLOCK
		asm("sbi %0,4" : : "I" (_SFR_IO_ADDR(PORTA)) );
		asm("cbi %0,4" : : "I" (_SFR_IO_ADDR(PORTA)) );
	}

	//SPICLOCK
	asm("sbi %0,4" : : "I" (_SFR_IO_ADDR(PORTA)) );
	asm("cbi %0,4" : : "I" (_SFR_IO_ADDR(PORTA)) );
	asm("sbi %0,4" : : "I" (_SFR_IO_ADDR(PORTA)) );
	asm("cbi %0,4" : : "I" (_SFR_IO_ADDR(PORTA)) );

	//read bits from adc
	for (int i=11 ; i>=0 ; i--){
		adcvalue += digitalReadFast(DATAIN) << i;
		//cycle clock
		//SPICLOCK
		asm("sbi %0,4" : : "I" (_SFR_IO_ADDR(PORTA)) );
		asm("cbi %0,4" : : "I" (_SFR_IO_ADDR(PORTA)) );
	}

	//deselct the MCP
	if(savedChannel > 8) 	{
		//Channels 9-16 on second MCP3208
		IOport2 = IOport2 | B10000010;     //Sets the Selection Pin for MCP 2
	} 	else 	{
		//Channels 1-8 on first MCP3208
		IOport2 = IOport2 | B10000001;     //Sets the Selection Pin for MCP 1
	}

	expanderWrite2(IOport2); //Deselect ADC

	return adcvalue;
}

int MultidisplayController::read_adc_fast (uint8_t channel){

	int adcvalue = 0;
	byte commandbits = B11000000; //command bits - start, mode, chn (3), dont care (3)
	uint8_t savedChannel = channel;  //Save the channel, to make it possible to deselect the MCP later.

	//choose the right MCP3208, depending on number to read.
	if(channel>8) 	{
		//Channels 9-16 on second MCP3208
		IOport2 = IOport2 & B01111101;     //Clears the Selection Pin for MCP 2 (Bitwise AND) (right is the MCP, left the LED)
		//to adapt the right Channel selection
		channel = channel - 8;
	} else 	{
		//Channels 1-8 on first MCP3208
		IOport2 = IOport2 & B01111110;     //Clears the Selection Pin for MCP 1 (Bitwise AND) http://www.programmers-corner.com/article/27
	}

	//allow channel selection
	commandbits|=((channel-1)<<3);

	expanderWrite2(IOport2); //Select ADC

	// setup bits to be written
	for (int i=7 ; i>=3 ; i--){

		//TODO uses still digitalwrite
//		digitalWriteFast(DATAOUT,commandbits&1<<i);

		//TODO testme
		uint8_t val = commandbits&1<<i;
		if ( val ) {
			//asm(code : output operand list : input operand list [: clobber list]);
			asm("sbi %0,3" : : "I" (_SFR_IO_ADDR(PORTF)) );
		} else {
			asm("cbi %0,3" : : "I" (_SFR_IO_ADDR(PORTF)) );
		}

		//cycle clock
		digitalWriteFast(SPICLOCK,HIGH);
		digitalWriteFast(SPICLOCK,LOW);
	}

	digitalWriteFast(SPICLOCK,HIGH);    //ignores 2 null bits
	digitalWriteFast(SPICLOCK,LOW);
	digitalWriteFast(SPICLOCK,HIGH);
	digitalWriteFast(SPICLOCK,LOW);

	//read bits from adc
	for (int i=11 ; i>=0 ; i--){
		adcvalue+=digitalReadFast(DATAIN)<<i;
		//cycle clock
		digitalWriteFast(SPICLOCK,HIGH);
		digitalWriteFast(SPICLOCK,LOW);
	}

	//deselct the MCP
	if(savedChannel > 8) 	{
		//Channels 9-16 on second MCP3208
		IOport2 = IOport2 | B10000010;     //Sets the Selection Pin for MCP 2
	} 	else 	{
		//Channels 1-8 on first MCP3208
		IOport2 = IOport2 | B10000001;     //Sets the Selection Pin for MCP 1
	}

	expanderWrite2(IOport2); //Deselect ADC

	return adcvalue;
}


/**
 * reads the 12bit adc -> analog values between 0 and 2^12
 * \see http://www.arduino.cc/playground/Code/MCP3208
 */
int MultidisplayController::read_adc(uint8_t channel){
	//TODO replace digital write http://code.google.com/p/multidisplay/issues/detail?id=3

	int adcvalue = 0;
	byte commandbits = B11000000; //command bits - start, mode, chn (3), dont care (3)
	uint8_t savedChannel = channel;  //Save the channel, to make it possible to deselect the MCP later.

	//choose the right MCP3208, depending on number to read.
	if(channel>8) 	{
		//Channels 9-16 on second MCP3208
		IOport2 = IOport2 & B01111101;     //Clears the Selection Pin for MCP 2 (Bitwise AND) (right is the MCP, left the LED)
		//to adapt the right Channel selection
		channel = channel - 8;
	} else 	{
		//Channels 1-8 on first MCP3208
		IOport2 = IOport2 & B01111110;     //Clears the Selection Pin for MCP 1 (Bitwise AND) http://www.programmers-corner.com/article/27
	}

	//allow channel selection
	commandbits|=((channel-1)<<3);

	expanderWrite2(IOport2); //Select ADC

	// setup bits to be written
	for (int i=7 ; i>=3 ; i--){
		digitalWrite(DATAOUT,commandbits&1<<i);
		//cycle clock
		digitalWrite(SPICLOCK,HIGH);
		digitalWrite(SPICLOCK,LOW);
	}

	digitalWrite(SPICLOCK,HIGH);    //ignores 2 null bits
	digitalWrite(SPICLOCK,LOW);
	digitalWrite(SPICLOCK,HIGH);
	digitalWrite(SPICLOCK,LOW);

	//read bits from adc
	for (int i=11 ; i>=0 ; i--){
		adcvalue+=digitalRead(DATAIN)<<i;
		//cycle clock
		digitalWrite(SPICLOCK,HIGH);
		digitalWrite(SPICLOCK,LOW);
	}

	//Deselct the MCP
	if(savedChannel > 8) 	{
		//Channels 9-16 on second MCP3208
		IOport2 = IOport2 | B10000010;     //Sets the Selection Pin for MCP 2
	} 	else 	{
		//Channels 1-8 on first MCP3208
		IOport2 = IOport2 | B10000001;     //Sets the Selection Pin for MCP 1
	}

	expanderWrite2(IOport2); //Deselect ADC

	return adcvalue;
}


//http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1173047782/5
void MultidisplayController::expanderWrite(byte _data) {
	wire.beginTransmission(EXPANDER);
	wire.send(_data);
	wire.endTransmission();
}

void MultidisplayController::expanderWrite2(byte _data) {
	wire.beginTransmission(EXPANDER2);
	wire.send(_data);
	wire.endTransmission();
}

byte MultidisplayController::expanderRead2() {
	byte _data = 0;
	wire.requestFrom(EXPANDER2, 1);
	if(wire.available()) {
		_data = wire.receive();
	}
	return _data;
}

byte MultidisplayController::expanderRead() {
	byte _data = 0;
	wire.requestFrom(EXPANDER, 1);
	if(wire.available()) {
		_data = wire.receive();
	}
	return _data;
}

void MultidisplayController::AnaConversion() {
	//This converts all 16 Analog Values into the real deal :)

	//Boost:
#ifdef BOOST_PLX_SMVACBOOST
	//see http://plxdevices.com/images/SM-VacBoostVolts.jpg
	// voltage = boost(psi)/9 + 1,66
	// boost (psi) = voltage * 9 - 15
	data.calAbsoluteBoost = 5.0* ( (float) data.anaIn[BOOSTPIN])/4095.0;
	data.calAbsoluteBoost = data.calAbsoluteBoost * 9.0 - 15; //psi
	data.calAbsoluteBoost = data.calAbsoluteBoost / BAR2PSI; //bar
	data.calBoost = data.calAbsoluteBoost; //formula above gets relative boost
	data.calAbsoluteBoost += data.boostAmbientPressureBar;
#endif
#ifdef BOOST_MOTOROLA_MPX4250
	//or Motorola MPX 4250 datasheet
	data.calAbsoluteBoost = ( ((5.0 * (data.anaIn[BOOSTPIN]/4095.0)) + 0.2) / 0.02 ) / 100; //Bar
	data.calBoost = data.calAbsoluteBoost - data.boostAmbientPressureBar;			//apply the offset (ambient pressure)
#endif
#ifdef BOOST_FREESCALE_MPXA6400A
	data.calAbsoluteBoost = 5.0* ((float) data.anaIn[BOOSTPIN])/4095.0;             //only gets 0-5V
	// Freescale MPXA6400A
	// transfer function P = Vout/0.012105 + 3.477902
	data.calAbsoluteBoost = (data.calAbsoluteBoost/0.012105 + 3.477902) / 100;     	//makes 0-400kPa out of it
	data.calBoost = data.calAbsoluteBoost - data.boostAmbientPressureBar;			//apply the offset (ambient pressure)
#endif
#ifdef BOOST_BOSCH_200KPA
	data.calAbsoluteBoost = 5.0* ((float) data.anaIn[BOOST2PIN])/4095.0;             //only gets 0-5V
	data.calAbsoluteBoost = (data.calAbsoluteBoost + 0.25)/0.0252778;     	//makes 0-200kPa out of it
	data.calAbsoluteBoost = data.calAbsoluteBoost / 100.0; //bar
//	data.calBoostPSI = data.calBoostBar * BAR2PSI;
	data.calBoost = data.calAbsoluteBoost - data.boostAmbientPressureBar;			//apply the offset (ambient pressure)
#endif


    //Calibration for RPM (its 2.34!)
	//Check if the Boost is a new Max Boost Event
	if( data.calBoost >= data.maxValues[MAXVAL_BOOST].boost ) 	{
		data.saveMax(MAXVAL_BOOST);
	}

	//Lambda:
	/*
	 * http://www.nxtgenmustang.com/forum/showthread.php/111-Wideband-Transfer-Functions
	 * Wideband Transfer Functions:
AEM: (v*2)+10
AFM1000: (v*2)+8
DynoJet Wideband Commander: (v*1.6)+10
EGT PLX: (v*368.2)+32
G100: (v*1.4)+9
Innovative LM-1: (v*2)+10
LC-1 (Innovative): (v*3)+7.35
LM-1 (Innovative): (v*2)+10
PLX: (v*2)+10
SCT EGT: (v*368.2)+32
Tech Edge 2AO: (v*2)+9
Zeitronix: (v*2)+9.6
	 */
#ifdef LAMBDA_WIDEBAND
#ifdef LAMBDA_PLX_SMAFR
	//http://www.plxdevices.com/InstallationInstructions/SM-AFRUsersGuide.pdf
	// air fuel ratio = 2*Voltage + 10
	data.calLambdaF = ( (5.0*( ((float) data.anaIn[LAMBDAPIN]) /4095)) * 2 + 10 ) / 14.7;
#endif
#ifdef LAMBDA_INNOVATE_LC1
	data.calLambdaF = ( (5.0*( ((float) data.anaIn[LAMBDAPIN]) /4095)) * 3 + 7.35 ) / 14.7;
#endif
	data.calLambda = map(data.anaIn[LAMBDAPIN], 0, 4095, 0, 200);
#else
	data.calLambda = map(data.anaIn[LAMBDAPIN], LAMBDAMIN, LAMBDAMAX, 0, 200);    //gets about the 0-1V into 0-200 values
	data.calLambdaF = (float) data.calLambda;
#endif
	data.calLambda = constrain(data.calLambda, 0, 200);

#ifdef LAMBDASTANDALONE
	//Lambda Standalone Patch -> hängt am Arduino direkt!	data.calLambdaF = ( (5.0*( ((float) data.anaIn[LAMBDAPIN]) /4095)) * 2 + 10 ) / 14.7;
	//nur 10Bit ADC!
	data.calLambdaF = ( (5.0*( ((float) data.anaIn[LAMBDAPIN]) /1023)) * 2 + 10 ) / 14.7;
#endif

	//CaseTemp: (damped)
	data.calCaseTemp = data.calCaseTemp*0.9 + (500.0*data.anaIn[CASETEMPPIN]/4095.0)*0.1;  //thats how to get Â°C out from a LM35 with 12Bit ADW

#ifdef VR6_MOTRONIC
	//Throttle:
	data.calThrottle = map(data.anaIn[THROTTLEPIN], THROTTLE_VR6_MIN, THROTTLE_VR6_MAX, 0, 100);
	data.calThrottle = constrain(data.calThrottle, 0, 100);
	data.calThrottle = 100 - data.calThrottle;                 //VR6 has 5V for closed and 0V for open throttle...
#endif

#if defined(DIGIFANT) && defined(DIGIFANT_DK_POTI)
//	data.calThrottle = data.anaIn[LMMPIN] ;
	data.calThrottle = map(data.anaIn[LMMPIN], THROTTLE_S2_MIN, THROTTLE_S2_MAX, 0, 100);
	data.calThrottle = constrain(data.calThrottle, 0, 100);
#endif

#if defined(DIGIFANT) && not defined(DIGIFANT_DK_POTI)
    //digifant idle switch (LL-Schalter) gets 0V for closed throttle valve; ~5V if open
    // assumption: if > 3 V: open
    if ( data.anaIn[LMMPIN] < 2457 )
            data.calThrottle = 0;
    else {
            //full throttle ?
            if ( data.anaIn[THROTTLEPIN] > 2457 )
                    data.calThrottle = 50;
            else
                    data.calThrottle = 100;
    }
#endif
    if ( data.calThrottle >= 90 ) {
    	//only on wot
    	if ( data.calLambdaF <= data.maxValues[MAXVAL_LAMBDA].lambda ) {
    		data.saveMax(MAXVAL_LAMBDA);
    	}
	}



#ifdef VR6_MOTRONIC
	//LMM
	data.calLMM = 5.0*data.anaIn[LMMPIN]/4095.0;		   //makes 0.0 to 5.0 Volt out of it, with VagCom this could be maped to gr Air i think
	if ( data.calLMM >= data.maxValues[MAXVAL_LMM].lmm ) {
		data.saveMax(MAXVAL_LMM);
	}
#endif

#ifdef V2DEVDEBUG
	if ( (v2devdebugflag & 1) == 0 ) {
#endif
	//RPM
#ifdef USE_RPM_ONBOARD
	//(from the smoothing example)
	//TODO more smoothing below 1000 rpm
	data.rpmTotal -= data.rpmReadings[data.rpmIndex];               // subtract the last reading
	data.rpmReadings[data.rpmIndex] = data.anaIn[RPMPIN];           // read from the sensor
	data.rpmTotal += data.rpmReadings[data.rpmIndex];               // add the reading to the total
	data.rpmIndex = (data.rpmIndex + 1);                       // advance to the next index

	if (data.rpmIndex >= RPMSMOOTH)                       // if we're at the end of the array...
	{
		data.rpmIndex = 0;                                  // ...wrap around to the beginning
	}

	data.rpmAverage = data.rpmTotal / RPMSMOOTH;               // calculate the average
	//digifant and motronic have the same rpmFactor
	data.calRPM = data.rpmAverage*RPMFACTOR;                   // apply the factor for calibration

	//Check if the RPM is a new Max RPM Event
	if ( data.calRPM >= data.maxValues[MAXVAL_RPM].rpm ) {
		data.saveMax(MAXVAL_RPM);
	}

	data.rpm_map_idx = (uint8_t) map (
			constrain (data.calRPM, RPM_MIN_FOR_BOOST_CONTROL, RPM_MAX_FOR_BOOST_CONTROL),
			RPM_MIN_FOR_BOOST_CONTROL, RPM_MAX_FOR_BOOST_CONTROL, 0, 255);
#endif
#ifdef V2DEVDEBUG
	}
#endif

	//Speed
	data.speedTotal -= data.speedReadings[data.speedIndex];
	data.speedReadings[data.speedIndex] = analogRead(SPEEDPIN);
	data.speedTotal += data.speedReadings[data.speedIndex];
	data.speedIndex = data.speedIndex++;

	if (data.speedIndex >= SPEEDSMOOTH)
		data.speedIndex = 0;

	data.speedAverage = data.speedTotal / SPEEDSMOOTH;               // calculate the average
	data.speedF = (float) data.speedAverage * SPEEDFACTOR * SPEEDCORRECTIONFACTOR;                   // apply the factor for calibration
	data.speed = (uint16_t) data.speedF;

	//Check if the speed is a new Max speed Event
	if ( data.speed >= data.maxValues[MAXVAL_SPEED].speed ) {
		data.saveMax(MAXVAL_SPEED);
	}

#ifdef BW_EFR_SPEEDSENSOR
	//we measure the time from rising to falling edge
	if ( data.efr_speed_reading == 0xFFFF )
		data.efr_speed = 0;
	else
		data.efr_speed = (uint32_t) BW_EFR_TIME_2_RPM / data.efr_speed_reading;
	if ( data.efr_speed >= data.maxValues[MAXVAL_EFR_SPEED].efr_speed )
			data.saveMax(MAXVAL_EFR_SPEED);
#endif


	//Battery Voltage: (Directly from the Arduino!, so only 1023, not 4095.)
	//measured Voltage * 4,09 + 0,7V should give the supply voltage
	data.batVolt = ((5.0*analogRead(BATTERYPIN)/1023.0)*4.09)+0.7;

#ifdef TYPE_K
	//Lets do the Typ K Conversion:
	fetchTypK3_fast();
#endif

	//VDO Stuff:
	if ( data.anaIn[VDOT1PIN] > 0 )
		data.VDOTemp1 = data.anaIn[VDOT1PIN];
	if ( data.anaIn[VDOT2PIN] > 0 )
		data.VDOTemp2 = GetVDOTemp(data.anaIn[VDOT2PIN]);
	if ( data.anaIn[VDOT3PIN] > 0 )
		data.VDOTemp3 = GetVDOTemp(data.anaIn[VDOT3PIN]);
	if (data.VDOTemp1 == 65506)
			data.VDOTemp1 = 0;
	if (data.VDOTemp2 == 65506)
			data.VDOTemp2 = 0;
	if (data.VDOTemp3 == 65506)
			data.VDOTemp3 = 0;

	if ( data.anaIn[VDOP1PIN] > 4090 )
		data.VDOPres1 = 0;
	else
		data.VDOPres1 = mapVdo5Bar.map32 ( data.anaIn[VDOP1PIN] );

	if ( data.anaIn[VDOP2PIN] > 4090 )
		data.VDOPres2 = 0;
	else
		data.VDOPres2 = mapVdo10Bar.map32 ( data.anaIn[VDOP2PIN] );
//		data.VDOPres2 = data.anaIn[VDOP2PIN] >> 4;

	if ( data.anaIn[VDOP3PIN] > 4090 )
		data.VDOPres3 = 0;
	else
		data.VDOPres3 = mapVdo10Bar.map32 ( data.anaIn[VDOP3PIN] );

	//data.VDOPres3 = GetVDOPressure(data.anaIn[VDOP3PIN]);

	//TODO max val for oil and gas pressure!

}

void MultidisplayController::V2_WarnLED() {
	#define BlueGreen 450      // Temperature (°C) when the Led changes from Blue to Green
	#define GreenYellow 900    // Temperature (°C) when the Led changes from Green to Yellow
	#define YellowRed 945      // Temperature (°C) when the Led changes from Yellow to Red

	if ( data.efr_speed > V2_RGB_WARNLED_EFR_SPEED_REDLINE ) {
		//red
		analogWrite (V2_SHIFTLED1PIN, 255);
		analogWrite (V2_SHIFTLED2PIN, 0);
		analogWrite (V2_SHIFTLED3PIN, 0);
		return; //dont matter what egt says -> we're over the redline and want red light!
	}

	if ( data.getMaxEgt() < BlueGreen ) {
		//blue
		analogWrite (V2_SHIFTLED1PIN, 0);
		analogWrite (V2_SHIFTLED2PIN, 0);
		analogWrite (V2_SHIFTLED3PIN, 255);
	} else {
		if ( data.getMaxEgt() < GreenYellow ) {
			//green
			analogWrite (V2_SHIFTLED1PIN, 0);
			analogWrite (V2_SHIFTLED2PIN, 255);
			analogWrite (V2_SHIFTLED3PIN, 0);
		} else {
			if ( data.getMaxEgt() < YellowRed ) {
				//yellow
				analogWrite (V2_SHIFTLED1PIN, 255);
				analogWrite (V2_SHIFTLED2PIN, 100);
				analogWrite (V2_SHIFTLED3PIN, 0);
			} else {
				//red
				analogWrite (V2_SHIFTLED1PIN, 255);
				analogWrite (V2_SHIFTLED2PIN, 0);
				analogWrite (V2_SHIFTLED3PIN, 0);
			}

		}
	}
}

void MultidisplayController::V1_Shiftlight()
{
	//This sets the brightness according to the RPM
	if(data.calRPM >= V1_RPM_SHIFT_LOW && data.calRPM <= V1_RPM_SHIFT_HIGH) 	{
		analogWrite(V1_RPMSHIFTLIGHTPIN,V1_RPM_LOW_BRIGHT);
	} else if(data.calRPM>= V1_RPM_SHIFT_HIGH) 	{
		analogWrite(V1_RPMSHIFTLIGHTPIN,V1_RPM_HIGH_BRIGHT);
	} else if(data.calRPM< V1_RPM_SHIFT_LOW) {
		analogWrite(V1_RPMSHIFTLIGHTPIN,V1_RPM_NO_BRIGHT);
	}
}





void MultidisplayController::serialReceive() {

	int bytes_read=0;
	byte command = -1;
	while(Serial.available()&&bytes_read<49)
	{
		if(bytes_read==0) {
			command = Serial.read();
		} else
			srData.asBytes[bytes_read-1] = Serial.read();
		bytes_read++;
	}

	// if the information we got was in the correct format,
	// read it into the system
	// case 1: command for pid lib
	if(bytes_read==25  && (command==0 || command==1))
	{
#ifdef BOOSTN75
		/*
		 * TODO review for V2
		 * Kp,Ki,Kd
		 * SampleTime
		 * On / Of
		 * ActivationThreshold factor
		 */

		/* we get fixed point values (base 1000) from the pc ! */

		boostController.pidBoostSetPoint = double(srData.asFixedInt32[0] / 1000.0);
		if(command==0) {
			// * only change the output if we are in manual mode
			boostController.boostOutput = double(srData.asFixedInt32[2] / 1000.0);
		}
#endif
		;


	} else 	{
		switch (command) {
			case 2:
				if (bytes_read >= 2) {
				//command for multidisplay

					switch ( srData.asBytes[0] ) {
					case 1:
						buttonAPressed();
						break;
					case 2:
						buttonAHold();
						break;
					case 3:
						buttonBPressed();
						break;
					case 4:
						buttonBHold();
						break;
					}
				}
				break;

			case 3:
				if (bytes_read >= 2) {
					switch ( srData.asBytes[0] ) {
						case 3:
							SerOut = SERIALOUT_TUNERPRO_ADX;
							break;
						case 2:
							SerOut = SERIALOUT_ENABLED;
							break;
						case 0:
							SerOut = SERIALOUT_DISABLED;
							break;
						case 4:
							SerOut = SERIALOUT_BINARY;
							break;
					}
				}
				break;
			case 4:
				if (bytes_read >= 2){
					switch ( srData.asBytes[0] ) {
#ifdef MULTIDISPLAY_V2
					case 1:
						saveSettings2Eeprom();
						break;
					case 2:
						calibrateLD();
						break;
					case 3:
						readSettingsFromEeprom();
						break;
					case 5:
#ifdef LCD
						lcdController.setBrightness(srData.asBytes[1]);
#endif
						break;
#endif
					case 4:
						//set new manual n75 boost dutycycles
#ifdef BOOSTN75
						if (bytes_read >= 4) {
							boostController.n75_manual_normal = srData.asBytes[1];
							boostController.n75_manual_race = srData.asBytes[2];
						}
#endif
						break;
					}
				}
				break;
#ifdef V2DEVDEBUG
			case 5:
				//V2 debugging stuff
				if (bytes_read >= 2){
					int rpm = 0;
					switch ( srData.asBytes[0] ) {
					case 0: //disable RPM computation if 1
						if ( srData.asBytes[1] == 1 )
							v2devdebugflag = v2devdebugflag  | 1;
						else
							v2devdebugflag = v2devdebugflag  & 254;
						break;
					case 1: // set RPM
						rpm = srData.asBytes[1] << 8 | srData.asBytes[2];
						data.calRPM = rpm;
						data.rpm_map_idx = (uint8_t) map ( constrain (data.calRPM, 0, RPM_MAX_FOR_BOOST_CONTROL), 0, RPM_MAX_FOR_BOOST_CONTROL, 0, 255);
						break;
					case 2: //disable N75 computation if 1
						if ( srData.asBytes[1] == 1 )
							v2devdebugflag = v2devdebugflag  | 2;
						else
							v2devdebugflag = v2devdebugflag  & 253;
						break;
					case 3: //set manual n75 duty cycle
#ifdef BOOSTN75
						boostController.boostOutput = (double) srData.asBytes[1];
#endif
						break;
					case 4: //bluetooth test / config
#if defined(MULTIDISPLAY_V2) && defined(BLUETOOTH_SETUP_ON_SERIAL2)
						switch ( srData.asBytes[1] ) {
						case 0 : Serial2.print("AT");
								break;
						case 1 : Serial2.print("AT+BAUD8"); //set 115200
								break;
						case 2 : Serial2.print ("AT+NAMEmdv2");
								break;
						case 3 : Serial2.print ("AT+PIN1234");
								break;
						}
#endif
						break;
#ifdef OLED
					case 5: //OLED debug
						switch ( srData.asBytes[1] ) {
						case 0: oled.sendDebugData();
								break;

						}
						break;
#endif
					}
				}
				break;
#endif
			case 6:
				//V2 config
				if ( bytes_read >= 2 ) {
					switch ( srData.asBytes[0] ) {
#ifdef BOOSTN75
					case 1: // gearX mode(low=0 high=1) serial : request gearX high/low duty cycle map
						if ( bytes_read >= 5 ) {
							boostController.serialSendDutyMap ( srData.asBytes[1], srData.asBytes[2], srData.asBytes[3]);
						}
						break;
					case 2:
						if ( bytes_read >= 5 ) {
							boostController.serialSendSetpointMap ( srData.asBytes[1], srData.asBytes[2], srData.asBytes[3]);
						}
						break;
					case 3:
						//2 gearX mode(low=0 high=1) serial 16 bytes map values: set gearX high/low duty cycle map [size 21]
						//  		reply: STX tag=22 gearX mode serial 16 bytes map ETX neu geschriebene map zurücksenden!
						if ( bytes_read >= 21 ) {
							boostController.setDutyMap ( srData.asBytes[1], srData.asBytes[2], &(srData.asBytes[4]) );
//							boostController.serialSendDutyMap ( srData.asBytes[1], srData.asBytes[2], srData.asBytes[3]);
							serialSendAck (srData.asBytes[3]);
						}
						break;
					case 4:
						if ( bytes_read >= 37 ) {
							boostController.setSetpointMap ( srData.asBytes[1], srData.asBytes[2], (uint16_t*) &(srData.asBytes[4]) );
//							boostController.serialSendSetpointMap ( srData.asBytes[1], srData.asBytes[2], srData.asBytes[3]);
							serialSendAck (srData.asBytes[3]);
						}
						break;
					case 5:
						//load maps
						if ( bytes_read >= 3 ) {
							boostController.loadMapsFromEEprom();
							serialSendAck (srData.asBytes[1]);
						}
						break;
					case 6:
						//Save maps
						if ( bytes_read >= 3 ) {
							boostController.writeMapsToEEprom();
							serialSendAck (srData.asBytes[1]);
						}
						break;
					case 7:
						//load parameters
						if ( bytes_read >= 3 ) {
							boostController.loadParamsFromEEprom();
							serialSendAck (srData.asBytes[1]);
						}
						break;
					case 8:
						//save parameters
						if ( bytes_read >= 3 ) {
							boostController.writeParamsToEEprom();
							serialSendAck (srData.asBytes[1]);
						}
						break;
					case 9:
						//set n75 new params
						// 9 set N75 params: 9 serial aKp aKi aKd cKp cKi cKd aAT cAT (16bit fixed uint16 base 100)
						// flags (uint8 bit0=pid enable) bst_limit (16bit base 100)
						if ( bytes_read >= 22 ) {
							boostController.setN75Params( (uint16_t*) &(srData.asBytes[2]));
							serialSendAck (srData.asBytes[1]);
						}
						break;
					case 10:
						//send n75 params
						if ( bytes_read >= 3 ) {
							boostController.serialSendN75Params(srData.asBytes[1]);
						}
						break;
#endif
					case 11:
						//set serial frequency
						if ( bytes_read >= 5 ) {
							uint16_t f = srData.asBytes[2];
							f += srData.asBytes[3] << 8;
							serialFreq = f;
							serialSendAck (srData.asBytes[1]);
						}
						break;
					case 12:
						//reserved for get serial frequency
						break;
					case 13:
#ifdef GEAR_RECOGNITION
						//get gear ratio map
						if ( bytes_read >= 3 )
							serialSendGearRatioMap (srData.asBytes[1]);
#endif
						break;
					case 14:
#ifdef GEAR_RECOGNITION
						//set gear ratio map
						if ( bytes_read >= 16 ) {
							//14 serial gears gear*uint16(fixed_int_base1000)
							if ( srData.asBytes[2] == 6 ) {
								//only implemented for 6 gears!
								uint16_t *p = (uint16_t*) &(srData.asBytes[3]);
								for ( uint8_t i = 0 ; i < srData.asBytes[2] ; i++ ) {
									gear_ratio[i] = fixedintb10002float( *p );
									p++;
								}
								serialSendGearRatioMap (srData.asBytes[1]);
							}
						}
#endif
						break;

					} //switch

				}
				break;
		}
	}
//	Serial.flush();
}


void MultidisplayController::serialSendAck (uint8_t serial) {
	Serial.print("\2");
	uint8_t outbuf = SERIALOUT_BINARY_TAG_ACK;
	Serial.write ( (uint8_t*) &(outbuf), sizeof(uint8_t) );
	Serial.write ( (uint8_t*) &(serial), sizeof(uint8_t) );
	Serial.print("\3");
}

#ifdef GEAR_RECOGNITION
void MultidisplayController::serialSendGearRatioMap (uint8_t serial) {
	if ( GEARS != 6 )
		return; //unimplemented!
	Serial.print("\2");
	uint8_t outbuf = SERIALOUT_BINARY_TAG_GEAR_RATIO_6G;
	Serial.write ( (uint8_t*) &(outbuf), sizeof(uint8_t) );
	Serial.write ( (uint8_t*) &(serial), sizeof(uint8_t) );
	outbuf = GEARS;
	Serial.write ( (uint8_t*) &(outbuf), sizeof(uint8_t) );
	for ( uint8_t i = 0 ; i < GEARS ; i++ ) {
		uint16_t out = float2fixedintb1000( gear_ratio[i] );
		Serial.write ( (uint8_t*) &(out), sizeof(uint16_t) );
	}
	Serial.print("\3");
}
#endif

void MultidisplayController::saveSettings2Eeprom() {
#ifdef LCD
	EEPROM.write(EEPROM_ACTIVESCREEN, lcdController.activeScreen );
#endif

#ifdef MULTIDISPLAY_V2
	uint8_t ldp = data.ldCalPoint;
	if ( ldp >= 0 && ldp <= 20 )
		EEPROM.write(EEPROM_LDCALPOINT, ldp);
	float ldt = data.boostAmbientPressureBar;
//	if ( ldt > 0.0 && ldt < 1.2 )
		EEPROMWriteLong (EEPROM_AMBIENTPRESSURE, ldt*1000);

#ifdef LCD
	EEPROM.write ( EEPROM_BRIGHTNESS, lcdController.brightness );
#endif

	EEPROMWriteuint16( EEPROM_SERIALFREQ, serialFreq );

#ifdef GEAR_RECOGNITION
	// gear ratio
	for ( uint8_t i = 0 ; i < GEARS ; i++ ) {
		EEPROMWriteuint16( EEPROM_GEAR_RATIO_MAP_START + i*2,
				float2fixedintb1000( gear_ratio[i] ) );
	}
#endif

#endif

#ifdef BOOSTN75
	EEPROM.write (EEPROM_N75_MANUALDUTY_NORMAL, boostController.n75_manual_normal);
	EEPROM.write (EEPROM_N75_MANUALDUTY_RACE, boostController.n75_manual_race);
#endif
}

void MultidisplayController::readSettingsFromEeprom() {
#ifdef LCD
	//what screen was last shown?
	uint8_t t = EEPROM.read(EEPROM_ACTIVESCREEN);
//	if ( t < SCREENCOUNT )
//		lcdController.setActiveScreen (t);
//	else
//		lcdController.setActiveScreen (0);
	//FIXME hack
	lcdController.setActiveScreen (0);

	lcdController.setBrightness (EEPROM.read(EEPROM_BRIGHTNESS));
#endif

	uint8_t ldp = EEPROM.read(EEPROM_LDCALPOINT);
	if ( ldp >= 0 && ldp <= 20 )
		data.ldCalPoint = ldp;
	float ldt = EEPROMReadLong(EEPROM_AMBIENTPRESSURE)/1000.0;      //gets the float back (thats accurate enough)
//	if ( ldt > 0.0 && ldt < 1.2 )
		data.boostAmbientPressureBar = ldt;

	uint16_t freq = EEPROMReaduint16(EEPROM_SERIALFREQ);
	if ( freq < 0xFFFF )
		serialFreq = freq;

	// gear ratio
#ifdef GEAR_RECOGNITION
	bool load_default_g = false;
	for ( uint8_t i = 0 ; i < GEARS ; i++ ) {
		uint16_t v = EEPROMReaduint16( EEPROM_GEAR_RATIO_MAP_START + 2*i );
		if ( v < 0xFFFF )
			gear_ratio[i] = fixedintb10002float( v );
		else
			load_default_g = true;
	}
	if ( load_default_g ) {
		//Corrado 02A ATB
	//	gear_ratio[0] = 13.0341;
	//	gear_ratio[1] = 7.26225;
	//	gear_ratio[2] = 4.64025;
	//	gear_ratio[3] = 3.34995;
	//	gear_ratio[4] = 2.74275;
	//	gear_ratio[5] = 2.74275;

		//Corrado AYN
		gear_ratio[0] = 13.918152;
		gear_ratio[1] = 7.802712;
		gear_ratio[2] = 4.95498;
		gear_ratio[3] = 3.577164;
		gear_ratio[4] = 2.785;
		gear_ratio[5] = 2.785;
	}
#endif

}


void MultidisplayController::serialSend() {

	if ( SerOut != SERIALOUT_DISABLED ) {
		Serial.print("\2");
#if defined(MULTIDISPLAY_V2) && defined(BLUETOOTH_ON_SERIAL2)
		Serial2.print("\2");
#endif

	}

	switch(SerOut){
	case SERIALOUT_DISABLED:
		break;
	case SERIALOUT_ENABLED:
		//Convertet Output:
		//TODO more output (ie VDO)
		Serial.print(SERIALOUT_ENABLED);
		Serial.print(":");
		Serial.print(time);
		Serial.print(";");
		Serial.print(data.calRPM);
		Serial.print(";");
		Serial.print(data.calBoost);
		Serial.print(";");
		Serial.print(data.calThrottle);
		Serial.print(";");
		Serial.print(data.calLambdaF);
		Serial.print(";");
		Serial.print(data.calLMM);
		Serial.print(";");
		Serial.print(data.calCaseTemp);
		Serial.print(";");
		Serial.print(data.calEgt[0]);
		Serial.print(";");
		Serial.print(data.calEgt[1]);
		Serial.print(";");
		Serial.print(data.batVolt);
		Serial.print(";");
		Serial.print(data.VDOPres1);
		Serial.print(";");
		Serial.print(data.VDOPres2);
		Serial.print(";");
		Serial.print(data.VDOPres3);
		Serial.print(";");
		Serial.print(data.VDOTemp1);
		Serial.print(";");
		Serial.print(data.VDOTemp2);
		Serial.print(";");
		Serial.print(data.VDOTemp3);
		break;
//	case SERIALOUT_TUNERPRO_ADX:
//		uint16_t outbuf;
//
//		Serial.write ( (uint8_t*) &(data.calRPM), sizeof(int) );
//		//hack for DF data frequency
////		Serial.write ( (uint8_t*) &( df_kline_freq_milliseconds ), sizeof(int) );
//
//		Serial.write ( (uint8_t*) &(data.calThrottle), sizeof(int) );
//
//		outbuf = float2fixedintb100(data.calLambdaF);
//		Serial.write ( (uint8_t*) &outbuf, sizeof(int) );
//
//		outbuf = float2fixedintb100(data.calAbsoluteBoost);
//		Serial.write ( (uint8_t*) &(outbuf), sizeof(int) );
//
//		Serial.write ( (uint8_t*) &(data.calEgt[0]), sizeof(int) );
//		Serial.write ( (uint8_t*) &(data.calEgt[1]), sizeof(int) );
	case SERIALOUT_TUNERPRO_ADX:
		int outbuf;
		uint32_t outbuf32;
		float f;

		//1. Lambda Raw
#ifdef LAMBDASTANDALONE
		outbuf = analogRead (LAMBDASTANDALONE);
#else
		outbuf = 0;
#endif
		Serial.write ( (uint8_t*) &(outbuf), sizeof(int) );
		f = ( (5.0*( ((float) outbuf) /1023)) * 2 + 10 ) / 14.7;
		outbuf = float2fixedintb100(f);
		//2. Lambda float
		Serial.write ( (uint8_t*) &outbuf, sizeof(int) );
		//3. Digifant total retard
		outbuf = float2fixedintb100(data.df_total_retard);
		Serial.write ( (uint8_t*) &outbuf, sizeof(int) );

		//4. Boost
		outbuf = float2fixedintb100(data.calBoost);
//		outbuf = float2fixedintb100(data.calAbsoluteBoost);
		Serial.write ( (uint8_t*) &(outbuf), sizeof(int) );

		//5. AGT
		Serial.write ( (uint8_t*) &(data.calEgt[0]), sizeof(int) );

		//6. Flags
		//bit 0: WOT
		//bit 1: idle

		//7. ECT
		//8. IAT
#if defined(MULTIDISPLAY_V2) && defined(DIGIFANT_KLINE)

		if ( df_kline_last_frame_completely_received < 255 ) {
			for ( uint8_t i = 1 ; i < (DF_KLINEFRAMESIZE-1) ; i++ )
				Serial.write ( (uint8_t*) &(df_klineData[df_kline_last_frame_completely_received].asBytes[i]), sizeof(uint8_t) );
		} else {
			uint8_t tmp = 0;
			for ( uint8_t i = 0 ; i < (DF_KLINEFRAMESIZE-2) ; i++ )
				Serial.write ( (uint8_t*) &(tmp), sizeof(uint8_t) );
		}
#endif

		break;

	case SERIALOUT_BINARY:

		/*
		 * STX: 1 byte
		 * SERIALOUT_BINARY_TAG: 1 byte
		 * MD2 data: 52 bytes
		 * digifant data: 32 bytes
		 * ETX: 1 byte
		 *
		 * overall 87 bytes per frame!
		 */

		// 1 byte
		outbuf = SERIALOUT_BINARY_TAG;
		Serial.write ( (uint8_t*) &(outbuf), sizeof(uint8_t) );
		//32bit / 4 byte
		Serial.write ( (uint8_t*) &(time), sizeof(unsigned long) );
		// 2 bytes
		Serial.write ( (uint8_t*) &(data.calRPM), sizeof(int) );

		// 2 bytes
		outbuf = float2fixedintb100(data.calAbsoluteBoost);
		Serial.write ( (uint8_t*) &(outbuf), sizeof(int) );

		//8bit are enough / 1 byte
		Serial.write ( (uint8_t*) &(data.calThrottle), sizeof(uint8_t) );

		// 2 bytes
		outbuf = float2fixedintb100(data.calLambdaF);
		Serial.write ( (uint8_t*) &outbuf, sizeof(int) );

		// 2 bytes
		outbuf = float2fixedintb100(data.calLMM);
		Serial.write ( (uint8_t*) &outbuf, sizeof(int) );

		// 2 bytes
		outbuf = float2fixedintb100(data.calCaseTemp);
		Serial.write ( (uint8_t*) &outbuf, sizeof(int) );

		//8 x 16 bit -> 16 bytes
		for ( uint8_t i = 0 ; i < MAX_ATTACHED_TYPE_K ; i++) {
			Serial.write ( (uint8_t*) &(data.calEgt[i]), sizeof(int) );
		}

		// 2 bytes
		outbuf = float2fixedintb100(data.batVolt);
		Serial.write ( (uint8_t*) &outbuf, sizeof(int) );

		//FIXME send as float ??
		//pressure is fixed int base 10
		// 12 bytes
		Serial.write ( (uint8_t*) &(data.VDOPres1), sizeof(int) );
		Serial.write ( (uint8_t*) &(data.VDOPres2), sizeof(int) );
		Serial.write ( (uint8_t*) &(data.VDOPres3), sizeof(int) );
		Serial.write ( (uint8_t*) &(data.VDOTemp1), sizeof(int) );
		Serial.write ( (uint8_t*) &(data.VDOTemp2), sizeof(int) );
		Serial.write ( (uint8_t*) &(data.VDOTemp3), sizeof(int) );

		// 2 bytes
		//Serial.write ( (uint8_t*) &(data.speed), sizeof(uint16_t) );
		outbuf = float2fixedintb100(data.speedF);
		Serial.write ( (uint8_t*) &outbuf, sizeof(int) );

		// 1 byte
		Serial.write ( (uint8_t*) &(data.gear), sizeof(uint8_t) );
		// 1 byte N75 dutycycle
#ifdef BOOSTN75
		outbuf = (uint8_t) boostController.boostOutput;
		Serial.write ( (uint8_t*) &(outbuf), sizeof(uint8_t) );
#else
		outbuf = 0;
		Serial.write ( (uint8_t*) &(outbuf), sizeof(uint8_t) );
#endif
		// 2 bytes
#ifdef BOOSTN75
		outbuf = float2fixedintb100( (float) boostController.req_Boost);
		Serial.write ( (uint8_t*) &(outbuf), sizeof(int) );
		// 1 byte N75 map pwm
		outbuf = (uint8_t) boostController.req_Boost_PWM;
		Serial.write ( (uint8_t*) &(outbuf), sizeof(uint8_t) );
#else
		outbuf = 0;
		Serial.write ( (uint8_t*) &(outbuf), sizeof(int) );
		Serial.write ( (uint8_t*) &(outbuf), sizeof(uint8_t) );
#endif

		/*
		 * reserved for flags:
		 * bit 0 : n75 pid enabled
		 * bit 1 : n75 pid aggressive settings
		 */
		outbuf = 0;
#ifdef BOOSTN75
		if ( boostController.usePID )
			outbuf |= 1;
		if ( boostController.aggressiveSettings )
			outbuf |= 2;
#endif
		Serial.write ( (uint8_t*) &(outbuf), sizeof(uint8_t) );
//		outbuf32 = float2fixedint32b100(data.efr_speed);
		outbuf = data.efr_speed_reading;
		Serial.write ( (uint8_t*) &(outbuf), sizeof(uint16_t) );
		//Knock
		outbuf = data.knock;
		Serial.write ( (uint8_t*) &(outbuf), sizeof(uint16_t) );


#if defined(MULTIDISPLAY_V2) && defined(DIGIFANT_KLINE)
		// 32 bytes
		if ( df_kline_last_frame_completely_received < 255 ) {
			for ( uint8_t i = 1 ; i < (DF_KLINEFRAMESIZE-1) ; i++ ) {
				Serial.write ( (uint8_t*) &(df_klineData[df_kline_last_frame_completely_received].asBytes[i]), sizeof(uint8_t) );
			}
		} else {
			uint8_t tmp = 0;
			for ( uint8_t i = 0 ; i < (DF_KLINEFRAMESIZE-2) ; i++ ) {
				Serial.write ( (uint8_t*) &(tmp), sizeof(uint8_t) );
			}
		}
		// k-line debug
		// changed from frequency to timestamp of last received df frame
		outbuf = df_kline_millis_last_frame_received;
		Serial.write ( (uint8_t*) &(outbuf), sizeof(uint16_t) );
		// frame number 1 byte
		Serial.write ( (uint8_t*) &(df_kline_last_frame_completely_received), sizeof(uint8_t) );
#endif

		break;

	default:
		SerOut = SERIALOUT_DISABLED;
		break;
	}

	if ( SerOut != SERIALOUT_DISABLED ) {
		Serial.print("\3");
#if defined(MULTIDISPLAY_V2) && defined(BLUETOOTH_ON_SERIAL2)
		Serial2.print("\3");
#endif
	}
}

void MultidisplayController::HeaderPrint() {

	switch(SerOut){
	case SERIALOUT_DISABLED:
		//This prints Debugging stuff:
		/*
   Serial.println(" ");
   Serial.print("CalBoost: ");
   Serial.println(CalLD);
		 */
		break;
	case SERIALOUT_ENABLED:
		Serial.println(" ");
		Serial.print(SERIALOUT_ENABLED);
		Serial.print(":");
//		Serial.println_P(PSTR("Time;RPM;Boost;Throttle;Lambda;LMM;CaseTemp;TypK1;TypK2;Battery;VDOP1;2;3;VDOT1;2;3"));
		break;
	default:
		break;
	}
}

void MultidisplayController::ChangeSerOut()
{
	//Switch from RAW to Cal and vise versa.
	switch(SerOut){
	case SERIALOUT_DISABLED:
		SerOut = SERIALOUT_BINARY;
		break;
	case SERIALOUT_BINARY:
		SerOut = SERIALOUT_ENABLED;
		break;
	case SERIALOUT_ENABLED:
		SerOut = SERIALOUT_DISABLED;
		break;
	default:
		SerOut = SERIALOUT_DISABLED;
		break;
	}
	HeaderPrint();
}
//-------------------------------------------------------------------------------------------------------

void MultidisplayController::calibrateLD()
{
	data.boostAmbientPressureBar = data.calAbsoluteBoost;
	// changed from global val3 to caluclation of mapped boost
	data.ldCalPoint = map(data.anaIn[BOOSTPIN], 0, 4095, 0, 200) / 10;
	//and saved:
	EEPROM.write(EEPROM_LDCALPOINT,data.ldCalPoint);
	EEPROMWriteLong(EEPROM_AMBIENTPRESSURE,data.boostAmbientPressureBar*1000);    //writes the float as long, will do it.

	//The MaxLD will be reset!
	data.maxLd = 0.0;
	data.maxLdt = data.ldCalPoint;

	Serial.println(" ");
	Serial.print("CalBoost: ");
	Serial.println(data.boostAmbientPressureBar);
}

//-------------------------------------------------------------------------------------------------------

//This converts the thermocouple ÂµV reading into some usable Â°C
int MultidisplayController::GetTypKTemp(unsigned int microVolts)
{
	//TODO dont use linear search!

	int LookedupValue = 0;
	//This searches the 2 surrounding values, and then linear interpolates between them.
	for(int i = 0; i<TEMPTYPEKREADINGS;i++) 	{
		if(microVolts >= pgm_read_word(&tempTypK[i]) && microVolts <= pgm_read_word(&tempTypK[i+1]))
		{
			LookedupValue = ((i)*50) + ((50L *(microVolts - pgm_read_word(&tempTypK[i]))) / ((pgm_read_word(&tempTypK[i+1]) - pgm_read_word(&tempTypK[i]))));
			break;
		}
	}
	return LookedupValue;
}

//Converts the ADW Reading into ï¿œC
int MultidisplayController::GetVDOTemp(unsigned int ADWreading) {
	//TODO dont use linear search!

	int LookedupValue = 0;
	//This searches the 2 surrounding values, and then linear interpolates between them.
	for(int i = 0; i<22;i++) {
		if(ADWreading <= pgm_read_word(&tempVDOTemp[i]) && ADWreading >= pgm_read_word(&tempVDOTemp[i+1]))
		{
			LookedupValue = (i*10) + 10 - ((10L *(ADWreading - pgm_read_word(&tempVDOTemp[i+1]))) / ((pgm_read_word(&tempVDOTemp[i]) - pgm_read_word(&tempVDOTemp[i+1]))));
			break;
		}
	}

	LookedupValue -= 30;   //must be reduced as the lookup table starts at -30Â°C.
	LookedupValue = constrain(LookedupValue,-40,999);    //Limits the Output to 999Â°C, so an open circuit gets detectet!

	return LookedupValue;
}



/*
 * Converts the ADW Reading into 0.1Bar (must be divided by 10 for Bar)
 * @deprecated too slow!
 */
int MultidisplayController::GetVDOPressure(unsigned int ADWreading) {
	int LookedupValue = 0;
	//This searches the 2 surrounding values, and then linear interpolates between them.
	for(int i = 0; i<12;i++) {
		if(ADWreading >= pgm_read_word(&tempVDOPressure[i]) && ADWreading <= pgm_read_word(&tempVDOPressure[i+1])) {
			LookedupValue = ((i)*10) + ((10L *(ADWreading - pgm_read_word(&tempVDOPressure[i]))) / ((pgm_read_word(&tempVDOPressure[i+1]) - pgm_read_word(&tempVDOPressure[i]))));
			break;
		}
	}

	LookedupValue -= 10;   //there is an offset of 10 due to my lazy coding
	LookedupValue = constrain(LookedupValue,0,999);    //Limits the Output to 99.9Bars, so an open circuit gets detectet!

	return LookedupValue;
}

//-------------------------------------------------------------------------------------------------------


/**
 * the original typK method
 */
void MultidisplayController::FetchTypK()  {
	//This will read in all the needed Analog values, convert them to ï¿œC, and make the calibration with the LM35
	//so it must be called after the AnaConversion!

	unsigned int Temp = 0;

	//http://www.arduino.cc/playground/Learning/4051
	int r0;      //value select pin at the 4051 (s0)
	int row;     // storeing the bin code
	int  bin [] = {000, 1, 10, 11, 100, 101, 110, 111};//bin = binary, some times it is so easy

	for (int i=0; i < NUMBER_OF_ATTACHED_TYPE_K; i++) {
		//there are 8 connections, so i have to set the 3 pins according to all channels

		//This needs to be modded to fit the new IO Handler
		row = bin[i];
		//r0 = row<<5;   		//Shift them 5 bits so they are 3 MSB (thats where the Selection Pins from the multiplexer are)

		r0 = row<<2;   		//Shift them 2 bits becouse the MSB are the CS from the ADWs, and the next 3 are this ones. the 3 LSB are free but can blink :)

		//Now i need some bitmath to get the bits set in r0 into the IOport2 byte.
		//First we clear this 3 spots

		byte Mask = B11100011;
		IOport2 = IOport2 & Mask;

		//Now we set the apropiate Bits:
		IOport2 = IOport2 | r0;

		//And write it to the Chip:

		expanderWrite2(IOport2);    //writes it to the port

		delay(20);   //Due to the 0.1 ÂµF cap this is needed. The Cap should be there to get a stable reading! (from open to RT it takes 15ms, plus 5ms safety)
		//then read in the value from the ADW

		Temp = ((5.0*read_adc(AGTPIN))/4095.0)*10000;   //gets the Volts and makes ï¿œV out of it (100 is already added from the Amp)
		Temp = GetTypKTemp(Temp);                       //Converts the ï¿œV into Â°C

		//Check if it is open:

		if(Temp>=MAX_TYPE_K) {
			Temp = 0;
		} else {
			Temp += int(data.calCaseTemp);                       //apply the Correction
		}

		data.calEgt[i] = Temp;              //Save it into the array

		//repeat for all NumTypK+1 channels.
	}
	data.checkAndSaveMaxEgt();
}

/**
 * still untested!
 * does not need the 20 msecs delay befor the typK reading
 * TODO testme!
 */
void MultidisplayController::fetchTypK3_fast() {
	switch ( typK_state ) {
	case TYPK_STATE_NEXT_SELECT_CHANNEL :
		incTypKChannel(); //checks and saves max EGT
		selectTypKchannelForReading (typK_state_cur_channel);
		typK_state_time2read = millis() + 20;
		typK_state = TYPK_STATE_NEXT_READ_CHANNEL;
		break;
	case TYPK_STATE_NEXT_READ_CHANNEL :
		if ( millis() > typK_state_time2read ) {
			readTypK(typK_state_cur_channel);
			typK_state = TYPK_STATE_NEXT_SELECT_CHANNEL;
		}
		break;
	}
}

/**
 * modularized fetchTypK method
 * TODO test me!
 */
void MultidisplayController::fetchTypK2()  {
	for (uint8_t i=0; i < NUMBER_OF_ATTACHED_TYPE_K; i++) {
		selectTypKchannelForReading (i);

		delay(20);   //Due to the 0.1 ÂµF cap this is needed. The Cap should be there to get a stable reading! (from open to RT it takes 15ms, plus 5ms safety)

		readTypK(i);
	}
	data.checkAndSaveMaxEgt();
}


void MultidisplayController::selectTypKchannelForReading (uint8_t channel) {
//	int  bin [] = {000, 1, 10, 11, 100, 101, 110, 111};
//	row = bin[i];
//	//r0 = row<<5;   		//Shift them 5 bits so they are 3 MSB (thats where the Selection Pins from the multiplexer are)
//
//	r0 = row<<2;   		//Shift them 2 bits becouse the MSB are the CS from the ADWs, and the next 3 are this ones. the 3 LSB are free but can blink :)
	int r0 = channel << 2;

	byte Mask = B11100011;
	IOport2 = (IOport2 & Mask) | r0;

	expanderWrite2(IOport2);    //writes it to the port
}

void MultidisplayController::readTypK ( uint8_t channel ) {
	unsigned int Temp = 0;
	Temp = ((5.0*read_adc(AGTPIN))/4095.0)*10000;   //gets the Volts and makes ï¿œV out of it (100 is already added from the Amp)
	Temp = GetTypKTemp(Temp);                       //Converts the ï¿œV into Â°C

	//Check if it is open:

	if(Temp>=MAX_TYPE_K) {
		Temp = 0;
	} else {
		Temp += int(data.calCaseTemp);                       //apply the Correction
	}

	data.calEgt[channel] = Temp;              //Save it into the array
}


//----------------------------------------------------------------------------------------------------
//This checks certain values if they exceed limits or not, if so an alarm is triggered
void MultidisplayController::CheckLimits()
{
	uint8_t Brightness = 0;
#ifdef LCD
	Brightness = lcdController.brightness;
#endif
	uint8_t FlashTrigger=0;

	/*
  if(VDOPres1>MaxOP)
  {
    FlashTrigger = 1;    //Enable the LCDFlash
  }
	 */

#ifdef LAMBDA_WIDEBAND
//	if( data.calLambdaF > 1.1 && data.calThrottle == 100 ) {
//		FlashTrigger = 1;
//		//TODO counter 1sec!
//	}
#endif


	//Set The new Flash Timeout
	if(FlashTrigger>0)
	{
		FlashETimeU = millis() + FLASH_E_TIME;
		FlashTrigger = 0;
	}


	//And Flash!
	if(FlashETimeU>=millis())                     //Check if the Flash should be shown
	{
		if(FlashTimeU<=millis())                  //Check if the Flash is about to be changed
		{
			//Now lets change the Brightness
			if(Brightness>0) {
				Brightness = 0;
			} else {
				Brightness = 2;
			}

			FlashTimeU = millis() + FLASH_TIME;      //And save the Next Changetime
#ifdef LCD
			lcdController.setBrightness(Brightness);              //And set the Brightness
#endif
		}
	}
	else {
		//?!?
		Brightness = EEPROM.read(EEPROM_BRIGHTNESS);           //Set back the Brightness
#ifdef LCD
		lcdController.setBrightness(Brightness);              //And set the Brightness
#endif
	}
#ifdef LCD
	lcdController.setBrightness(Brightness);
#endif

}

//----------------------------------------------------------------------------------------------------



void MultidisplayController::mainLoop() {
	//Current Time:
	time = millis();

	//Read in all Analog values:
	for(uint8_t i = 1; i <=16;i++) {
		data.anaIn[i] = read_adc_fast_mega (i);
//		data.anaIn[i] = read_adc_fast (i);
//		data.anaIn[i] = read_adc (i);
	}

#ifdef LAMBDASTANDALONE
	//Lambda Standalone Patch -> hängt am Arduino direkt!
	data.anaIn[LAMBDAPIN] = analogRead (LAMBDASTANDALONE);
#endif

	AnaConversion();



#ifdef V1_RPM_SHIFT_LIGHT
	V1_Shiftlight();
#endif

#ifdef V2_RGB_WARNLED_ON_SHIFTLED123
	//Check for Limits:
	V2_WarnLED();
#endif

	// ui knows what screen is active and draws it!
#ifdef LCD
	if ( drawTime < millis() ) {
		lcdController.draw();
		drawTime = millis() + LCD_DRAW_TIME;
	}
#endif

#ifdef OLED
//	if ( oled_flags == 0 && millis() > 3000 ) {
//		oled.myconstructor();
//		oled_flags = 1;
//	}
#endif

#ifdef MULTIDISPLAY_V1
	// button check for V1
	buttonCheck_V1(expanderRead());
#else
	buttonCheck_V2();
#endif

	//Saves the Screen when needed:
	if(millis()>= ScreenSave) {
		//and now save it:
#ifdef LCD
		EEPROM.write(EEPROM_ACTIVESCREEN, lcdController.activeScreen);
#endif
		//And also prevent a double save!
		ScreenSave = 429400000;        // (thats close to 50days of runtime...)
	}

#if FREEMEM
	//Free Mem Debug:
	Serial.print("FM ");
	Serial.println( freeMem());
#endif

#ifdef RPMDebug
	//Debug RPM generator:
	if(debug>=DEBUGRPM) {
		debug = 0;
		digitalWrite(14,HIGH);
		delay(2);
		digitalWrite(14,LOW);
	} else {
		debug++;
	}
#endif

#ifdef BOOSTN75
	//normal or race mode?
	//internal pull-up active
	//switch connects pin to groud
	boostController.toggleMode ( digitalRead(NORDSCHLEIFENPIN) );

#ifdef V2DEVDEBUG
	if ( (v2devdebugflag & 2) == 0 ) {
#endif
	boostController.compute();
#ifdef V2DEVDEBUG
	}
#endif
	analogWrite(N75PIN, (int) boostController.boostOutput);
//	analogWrite(FREEPWM2, (int) boostController.boostOutput);
#endif

	if ( millis() > serialTime ) {
		serialReceive();
//		data.generate_debugData();
		serialSend();
		serialTime += serialFreq;
	}

#if defined(MULTIDISPLAY_V2) && defined(DIGIFANT_KLINE)
	if ( millis() > DF_KlineSerialTime ) {
		DF_KlineSerialTime += DF_KLINESERIALFREQ;
		DFKlineSerialReceive();
	}
#endif
#if defined(MULTIDISPLAY_V2) && defined(KWP1281_KLINE)
	kwp1281 ();
#endif

#if defined(MULTIDISPLAY_V2) && defined(BLUETOOTH_SETUP_ON_SERIAL2)
	//to setup the bluetooth module
	//read its output and forward it to the usb serial
	if ( millis() > 500 ) {
		BTSerialTime += 500;

		while(Serial2.available()) {
			int r = Serial2.read();
			if ( r != -1 ) {
				unsigned char c = (unsigned char) r;
				Serial.print(c);
			}
		}
	}
#endif

#if defined(MULTIDISPLAY_V2) && defined(GEAR_RECOGNITION)

//	/*
//	 * clutch in -> sw connects through
//	 * clutch separates -> infinity resistance
//	 *
//	 * -> add pulldown
//	 *
//	 * init: after start gear 1
//	 *
//	 */
//	if ( ! digitalRead(CLUTCHPIN) ) {
//		//clutch pedal pressed
//		gear_state = GEAR_STATE_NEED_RECOGNITION_WAIT_FOR_CLUTCH_IN;
//	} else {
//		if ( gear_state == GEAR_STATE_NEED_RECOGNITION_WAIT_FOR_CLUTCH_IN ) {
//			gear_state = GEAR_STATE_NEED_RECOGNITION;
//			last_gear = data.gear;
//			gear_wait_before_idle = millis() + GEAR_WAIT_MSECS_BEFORE_IDLE;
//			gear_computation();
//		}
//	}

	if ( millis() > gear_computation_time ) {
		gear_state = GEAR_STATE_NEED_RECOGNITION;
		gear_computation();
		gear_computation_time += GEAR_COMPUTATION_INTERVALL;
	}
#endif

}



/*==============================================================================
 * BUTTON FUNCTIONS
 *============================================================================*/


void MultidisplayController::buttonCheck_V2()  {
	//TODO implement me!
}


void MultidisplayController::buttonAHold() {

#ifdef DEBUG
	Serial.println ("A Hold");
#endif

#ifdef LCD
	lcdController.toggleScreen();
#endif

	//Set the Timestamp for the Save:
	ScreenSave = millis() + SCREENSAVEDELAY;

#ifdef DEBUG
	Serial.print ("screen ");
	Serial.println (lcdController.activeScreen);
#endif
}

//-------------------------------------------------------------------------------------------------------

void MultidisplayController::buttonAPressed() {

	//Serial.print(time*1);
	//Serial.print(";");
	//Serial.println("Button A Pressed");
#ifdef DEBUG
	Serial.println ("A pressed");
#endif

//	switch(lcdController.activeScreen){
//	case 2:
////		//Toggle A and B MCP
//		lcdController.myScreens[1]->toggleScreenAB();
//		break;
//	case 6:                 //Switches the 2 Row Screen
//		lcdController.myScreens[5]->toggleScreenAB();
//		break;
//	case 7:
//		lcdController.myScreens[6]->toggleRefreshCounter();
//		break;
//	default:
//		break;
//	}
}

//-------------------------------------------------------------------------------------------------------

void MultidisplayController::buttonBHold() {

#ifdef DEBUG
	Serial.println ("B Hold");
#endif

#ifdef LCD
	switch(lcdController.activeScreen){
	case 0:
		break;
	default:
		break;
	}
#endif

}

//-------------------------------------------------------------------------------------------------------

void MultidisplayController::buttonBPressed() {

#ifdef DEBUG
	Serial.println ("B pressed");
#endif

	//Serial.print(time*1);
	//Serial.print(";");
	//Serial.println("Button B Pressed");

#ifdef LCD
//	LCDScreen* l;
//
//	switch(lcdController.activeScreen){
//	case 0:
//		break;
//	case 1:
//		//The MaxLD will be reset!
//		data.maxLd = 0.0;
//		data.maxLdt = data.ldCalPoint;
//		break;
//	case 2:
//		//Change LCD brightness
//		lcdController.toggleBrightness();
//
//		//and save the new value:
//		EEPROM.write(EEPROM_BRIGHTNESS,lcdController.brightness);
//		break;
//
//	case 7:
//		l = lcdController.getLCDScreen(6);
//		if ( l ) {
//			((LCDScreen7*)l)->min = 0;
//			((LCDScreen7*)l)->max = 5000;
//
//			((LCDScreen7*)l)->toggleMode();
//		}
//		break;
//
//	case 8:
//		l = lcdController.getLCDScreen(7);
//		if ( l ) {
//			((LCDScreen8*)l)->toggleMax();
//		}
//		break;
//
//	default:
//		break;
//	}
#endif
}

//-------------------------------------------------------------------------------------------------------

void MultidisplayController::buttonCheck_V1(int buttonState)  {
	/**
	 * A hold : switch screens
	 */

	/*
	3 = open
	1 = s1
	2 = s2
	 */

	//compensate for the LED:
	buttonState -= 128;

	//now a little inteligent stuff:
	switch(buttonState) {
	case 1:
		if(buttonTime == 0) {
			buttonAPressed();
			buttonTime = millis();
		} else {
			if(millis()>=buttonTime+BUTTONHOLD) {
				buttonAHold();
				buttonTime = millis();
			}
		}
		break;

	case 2:
		if(buttonTime == 0) {
			buttonBPressed();
			buttonTime = millis();
		} else {
			if(millis()>=buttonTime+BUTTONHOLD) {
				buttonBHold();
				buttonTime = millis();
			}
		}
		break;

	case 3:
		buttonTime = 0;
		break;
	}
	//Thats it... easy.
}

#if defined(MULTIDISPLAY_V2) && defined(DIGIFANT_KLINE)

void MultidisplayController::DFKlineSerialReceive() {

	while(Serial1.available()) {

		switch ( df_kline_status ) {

		case DF_KLINE_STATUS_FRAMEERROR:
		case DF_KLINE_STATUS_FRAME_COMPLETE: {
			//new frame --> check for start char
			df_kline_index = 0;
			for ( int i=0; i < (DF_KLINEFRAMESIZE-1) ; i++ )
				df_klineData[df_kline_active_frame].asBytes[i]=0xFF;

			uint8_t b = Serial1.read();
			if ( b != DF_KLINEBEGIN ) {
				//skip it
			} else {
				df_kline_status = DF_KLINE_STATUS_RECEIVING;
				df_klineData[df_kline_active_frame].asBytes[df_kline_index] = b;
				df_kline_index++;
			}
			break;
		}
		case DF_KLINE_STATUS_RECEIVING:
			if ( df_kline_index == DF_KLINEFRAMESIZE-1 ) {
				//last char -> check for end char
				uint8_t b = Serial1.read();
				if ( b != DF_KLINEEND ) {
					df_kline_discarded_frames++;
					df_kline_status = DF_KLINE_STATUS_FRAMEERROR;
				} else {
					df_klineData[df_kline_active_frame].asBytes[df_kline_index] = b;
					//frame complete!
					df_kline_status = DF_KLINE_STATUS_FRAME_COMPLETE;
					df_kline_index = 0;
					//do sth with it!
					df_kline_last_frame_completely_received = df_kline_active_frame;
					DFConvertReceivedData();

					df_kline_freq_milliseconds = (uint16_t) millis() - df_kline_millis_last_frame_received;
					df_kline_millis_last_frame_received = millis();

					df_kline_active_frame++;
					if ( df_kline_active_frame >= DF_KLINE_STORE_FRAME_COUNT )
						df_kline_active_frame = 0;
				}
			} else {
				df_klineData[df_kline_active_frame].asBytes[df_kline_index] = Serial1.read();
				df_kline_index++;
			}
			break;
		}
	}
}

void MultidisplayController::DFConvertReceivedData() {
	/*
	 * remember: data starts at Index 1!
	 */

	//remember: 68HC11 is big endian
	//avr8 is little endian!
	data.df_ignition = ( df_klineData[df_kline_last_frame_completely_received].asBytes[8] * -0.351563 ) + 73.9;
	data.df_cyl1_retard = ( df_klineData[df_kline_last_frame_completely_received].asBytes[10] * 0.351563 );
	data.df_cyl2_retard = ( df_klineData[df_kline_last_frame_completely_received].asBytes[12] * 0.351563 );
	data.df_cyl3_retard = ( df_klineData[df_kline_last_frame_completely_received].asBytes[14] * 0.351563 );
	data.df_cyl4_retard = ( df_klineData[df_kline_last_frame_completely_received].asBytes[16] * 0.351563 );
	data.df_total_retard = data.df_cyl1_retard + data.df_cyl2_retard + data.df_cyl3_retard + data.df_cyl4_retard;

#ifdef USE_DIGIFANT_MAPSENSOR
	//400kpa
	data.calAbsoluteBoost = ((5.0 * (df_klineData[df_kline_last_frame_completely_received].asBytes[1] /255.0)) / 0.012105 + 3.477902) / 100;
	//200kpa
//	data.calAbsoluteBoost = (((5.0 * (df_klineData[df_kline_last_frame_completely_received].asBytes[1] /255.0)) + 0.25) / 0.0252778 ) / 100;
	//250kpa
//	data.calAbsoluteBoost = (((5.0 * (df_klineData[df_kline_last_frame_completely_received].asBytes[1] /255.0)) + 0.2) / 0.02) / 100;
	//kpa
//	data.calAbsoluteBoost = ( (5.0 * (df_klineData[df_kline_last_frame_completely_received].asBytes[1]/255.0)) + 0.2) / 0.02;
//	data.calAbsoluteBoost = data.calAbsoluteBoost / 100.0;

	//300kpa
//	data.calAbsoluteBoost =  (float) (((5.0 * ((df_klineData[df_kline_last_frame_completely_received].asBytes[1])/255.0)) + 0.1765) / 0.0159) / 100.0;
//	data.calBoost = data.calAbsoluteBoost - data.boostAmbientPressureBar;
	data.calBoost = data.calAbsoluteBoost - 1.0;
#endif

#ifdef USE_DIGIFANT_RPM
	//anstatt 9 $4F und 18 $AD
	uint32_t df_delta_hall = (df_klineData[df_kline_last_frame_completely_received].asBytes[31]<<8) + df_klineData[df_kline_last_frame_completely_received].asBytes[32];
	if ( df_delta_hall > 0 )
		data.calRPM = (int) ( ((uint32_t) 30000000) / (uint32_t) df_delta_hall );
	else
		data.calRPM = 0;

	//Check if the RPM is a new Max RPM Event
	if ( data.calRPM >= data.maxValues[MAXVAL_RPM].rpm ) {
		data.saveMax(MAXVAL_RPM);
	}

	data.rpm_map_idx = (uint8_t) map (
			constrain (data.calRPM, RPM_MIN_FOR_BOOST_CONTROL, RPM_MAX_FOR_BOOST_CONTROL),
			RPM_MIN_FOR_BOOST_CONTROL, RPM_MAX_FOR_BOOST_CONTROL, 0, 255);

#endif
}

#endif

#if defined(MULTIDISPLAY_V2) && defined(KWP1281_KLINE)

void MultidisplayController::sendFiveBaudBit (int bit) {
	/* TX Port: USART must be disabled and port must be set as output
	 * L must be configured as output
	 *
	 * 5 Baud -> 200msec period
	 */
	if (bit) {
		setKLHigh();
	} else {
		setKLLow();
	}
	delay (200);
	return;
}

void MultidisplayController::kwp1281SendControllerAddress(uint8_t address) {
	//Start
	sendFiveBaudBit(0);
	//7 bits controller address; lsb first

	//disabled because libm problems:
	///opt/avr/lib/gcc/avr/4.3.4/../../../../avr/lib/avr51/libm.a(log.o):../../../../../source/avr-libc-1.6.8/libm/fplib/log.S:98: relocation truncated to fit: R_AVR_13_PCREL against symbol `__addsf3' defined in .text section in /opt/avr/lib/gcc/avr/4.3.4/avr51/libgcc.a(_addsub_sf.o)
//	uint8_t bits = 0;
//	for (uint8_t i = 0; i < 7 ; i++) {
//		uint8_t bit = address & ( (int) pow (2,i) );
//		sendFiveBaudBit( bit );
//		bits += bit;
//	}
//	//parity -> number of 1s in the data byte has to be odd
//	if ( bits % 2 )
//		//odd
//		sendFiveBaudBit( 0 );
//	else
//		sendFiveBaudBit( 1 );

	//ECU address 1
	sendFiveBaudBit( 1 );
	sendFiveBaudBit( 0 );
	sendFiveBaudBit( 0 );
	sendFiveBaudBit( 0 );
	sendFiveBaudBit( 0 );
	sendFiveBaudBit( 0 );
	sendFiveBaudBit( 0 );
	//parity
	sendFiveBaudBit( 0 );

	//Stop
	sendFiveBaudBit(1);
//	setKLow();
}

/*
 * hack: temp using Serial2 / Port PH0 / PH1
 *
 * PINH & 1
 * PINH & 1
 */
bool MultidisplayController::kwp1281Connect2 () {
	kwp1281_state = KWP1281_STATE_HANDSHAKE;

	Serial2.end();
	UCSR2B = bitClear(UCSR2B, 4);
	UCSR2B = bitClear(UCSR2B, 3);
	DDRH |= 2;
	DDRH &= ~1;

	setKLHigh();
	delay(300);
#ifdef KWP1281_KLINE_DEBUG
	Serial.println ("kwp1281Connect: send ecu address");
#endif
	//ecu
	kwp1281SendControllerAddress (1);
	//sync pattern from ecu
	//80ms << t_r1 << 210 ms

	Serial2.begin(10400);
	UCSR2C |=  48; // odd parity
	bitClear (UCSR2C,1); //7Bit

	delay(60);

	uint16_t millis_stop = millis() + 250;

	while ( millis() < millis_stop ) {
		//wait for sync byte 55h
		if ( Serial2.available() ) {
			if ( (Serial2.read() & 0x7F) == 0x55 ) {
				//got sync byte
#ifdef KWP1281_KLINE_DEBUG
				Serial.println ("kwp1281Connect awaiting key bytes");
#endif
				break;
			} else {
#ifdef KWP1281_KLINE_DEBUG
				Serial.println ("kwp1281Connect wrong sync pattern");
#endif
				//TODO: if 10400 baud fails -> retry with 9600 and 1200 baud!
				kwp1281_state = KWP1281_STATE_NO_CONNECTION;
				return false;
			}
		}
	}

	//low order key byte from ecu
	//5ms << t_r2 << 20 ms
	millis_stop = millis() + 40;
	uint8_t kb1, kb2;
	while ( millis() < millis_stop ) {
		if ( Serial2.available() ) {
			kb1 = Serial2.read();
			break;
		}
	}

	//high order key byte from ecu
	//1ms << t_r3 << 20 ms
	millis_stop = millis() + 40;
	while ( millis() < millis_stop ) {
		if ( Serial2.available() ) {
			kb2 = Serial2.read();
			break;
		}
	}
	//ecu sends keywords 701, we receive 7O1
//	kb1 = kb1 & 0x7F;
	uint8_t kb2_inv = 0x7F - kb2;
//	kb2 = kb2 & 0x7F;
	uint16_t keyword = kb2 * 128 + kb1;

	if ( keyword != 1281 ) {
		kwp1281_state = KWP1281_STATE_NO_CONNECTION;
		return false;
	}

	//send complement from kb2 back
	//25ms < tr_4 < 50ms
	delay(35);
//	Serial2.write (kb2_inv);
	kwp1281Write (kb2_inv);

#ifdef KWP1281_KLINE_DEBUG
	Serial.print ("connect kw ");
	Serial.print (keyword, 10);
	Serial.print ("\n");
#endif

	//controller changes to 8N1 too
	//no parity
	UCSR2C &=  ~48;
	UCSR2C |= 6; //8Bit

	kwp1281_kline_millis_last_byte_received = millis();

	//now ecu sends vehicle data
	kwp1281_state = KWP1281_STATE_SLAVE_RCV_CONTROLLER_DATA;
	kwp1281_controller_data_block_counter = 0;

	return true;
}

bool MultidisplayController::kwp1281Connect () {
	kwp1281_state = KWP1281_STATE_HANDSHAKE;

	setKLHigh();
	delay(300);
#ifdef KWP1281_KLINE_DEBUG
	Serial.println ("kwp1281Connect: send ecu address");
#endif
	//ecu
	kwp1281SendControllerAddress (1);
	//sync pattern from ecu
	//80ms << t_r1 << 210 ms
	delay (60);

	/* synchonisation pattern: controller "tells" the baud rate -> measure the usecs of a bit -> baudrate
	 * newer ecus use only 10400
	 * older ones could use also 9600 or 1200
	 */
	uint16_t millis_stop = millis() + 1000;
	uint16_t usecs = 0;
	uint8_t state = 0;
	uint8_t rx_last_read = 0;
	uint8_t bits_seen=0;
	while ( millis() < millis_stop ) {
		if (state == 0) {
			//wait for start bit (low)
			if ( (PINH & 1) == 0 ) {
				state = 1;
				rx_last_read = 0;
#ifdef KWP1281_KLINE_DEBUG
				Serial.println ("kwp1281Connect: state 0->1");
#endif
			}
		}
		if (state == 1 ) {
//#ifdef KWP1281_KLINE_DEBUG
//				Serial.print ("kwp1281Connect: state 1 " );
//				Serial.print (bits_seen);
//				Serial.print (" rx=");
//				if ( (PINH & 1) )
//					Serial.print ("1");
//				else
//					Serial.print ("0");
//				Serial.print ("\n");
//#endif
			//measure time
			if ( rx_last_read == 0 &&  (PINH & 1) == 1 ) {
				//rising
				usecs = micros();
				rx_last_read = 1;
				bits_seen++;
			}
			if ( rx_last_read == 1 &&  (PINH & 1) == 0 ) {
				//falling
				usecs = micros() - usecs;
				rx_last_read = 0;
				bits_seen++;
			}
			if ( bits_seen == 8 )
				state = 2;
		}
		if (state == 2) {
			//wait for high -> stop bit
			if ( rx_last_read == 0 &&  (PINH & 1) == 1 ) {
				//stop bit

			}
		}
	}
	if ( state < 2 ) {
		kwp1281_state = KWP1281_STATE_NO_CONNECTION;
		kwp1281_connect_failures++;
#ifdef KWP1281_KLINE_DEBUG
	Serial.print ("kwp1281Connect EE sync pattern state=");
	Serial.print (state, 10);
	Serial.print ("\n");
#endif
		return false;
	}
#ifdef KWP1281_KLINE_DEBUG
	Serial.print ("kwp1281Connect sync pattern received ");
	Serial.print (usecs, 10);
	Serial.print ("\n");
#endif

	if (usecs > 800)
		Serial2.begin(1200);
	else {
		if (usecs > 100)
			Serial2.begin(9600);
		if (usecs < 100)
			Serial2.begin(10400);
	}

#ifdef KWP1281_KLINE_DEBUG
	Serial.println ("kwp1281Connect awaiting key bytes");
#endif

	//low order key byte from ecu
	//5ms << t_r2 << 20 ms
	millis_stop = millis() + 40;
	uint8_t kb1, kb2;
	while ( millis() < millis_stop )
		if ( Serial2.available() )
			kb1 = Serial2.read();

	//high order key byte from ecu
	//1ms << t_r3 << 20 ms
	millis_stop = millis() + 40;
	while ( millis() < millis_stop )
		if ( Serial2.available() )
			kb2 = Serial2.read();
	//ecu sends keywords 701, we receive 8N1
	kb1 = kb1 & 0x7F;
	uint8_t kb2_inv = 0xFF - kb2;
	kb2 = kb2 & 0x7F;
	uint16_t keyword = kb1 + kb2*128;

	//send complement from kb2 back
	delay(25);
	Serial2.write (kb2_inv);

#ifdef KWP1281_KLINE_DEBUG
	Serial.print ("kwp1281Connect keyword=");
	Serial.print (keyword, 10);
	Serial.print ("\n");
#endif

	//controller changes to 8N1 too
	//now ecu sends vehicle data
	kwp1281_state = KWP1281_STATE_SLAVE_RCV_CONTROLLER_DATA;
	kwp1281_controller_data_block_counter = 0;
}

void MultidisplayController::kwp1281 () {

#ifdef KWP1281_KLINE_DEBUG
	if ( kwp1281_debug_printtime < millis() ) {
		Serial.print ("kwp1281 kwp1281_state=0x");
		Serial.print (kwp1281_state, 16);
		Serial.print ("\n");
		kwp1281_debug_printtime = millis() + KWP1281_DEBUG_TIME;
	}
#endif

	switch (kwp1281_state) {

	case KWP1281_STATE_NO_CONNECTION :
		if ( kwp1281_connect_failures < KWP1281_MAX_CONNECTION_ATTEMPTS )
			if ( kwp1281Connect2() ) {
				kwp1281_state = KWP1281_STATE_SLAVE_RCV_CONTROLLER_DATA;
				kwp1281ReceiveBlock (true);
			}
		break;
	case KWP1281_STATE_HANDSHAKE:
		break;
	case KWP1281_STATE_SLAVE_RCV_CONTROLLER_DATA:
		if ( kwp1281ReceiveBlock (false) ) {
			//block completely received!
			if ( kwp1281_controller_data_block_counter == 0 ) {
				if ( (kwp1281_klineData[kwp1281_kline_active_frame].asBytes[3]) & 0x80 ) {
					kwp1281_controller_data_more_than_four_blocks = true;
					bitClear ( (kwp1281_klineData[kwp1281_kline_active_frame].asBytes[3]), 8);
				} else
					kwp1281_controller_data_more_than_four_blocks = false;
				kwp1281_state = KWP1281_STATE_ESTABLISHED_MASTER_SEND_ACK;
				kwp1281_send_block_state = 0;
				//TODO do something with the received string
#ifdef KWP1281_KLINE_DEBUG
				Serial.println ("controller data");
				for (uint8_t i = 3 ; i < kwp1281_kline_index ; i++)
					Serial.write (kwp1281_klineData[kwp1281_kline_active_frame].asBytes[i]);
				Serial.print("\n");
#endif

			}
			kwp1281_controller_data_block_counter++;
		}

		break;
	case KWP1281_STATE_ESTABLISHED_MASTER:
		//we are master
		//either request a function or hold the connection alive with ACK (each 1000ms)

		if ( kwp1281_kline_millis_last_byte_received + KWP1281_KEEPALIVE_MSECS > millis() ) {
			kwp1281_state = KWP1281_STATE_ESTABLISHED_MASTER_SEND_ACK;
#ifdef KWP1281_KLINE_DEBUG
	Serial.println ("keepalive ACK");
#endif
		}

		break;
	case KWP1281_STATE_ESTABLISHED_MASTER_SEND_ACK:
		kwp1281_send_block_state = 0;
	case KWP1281_STATE_ESTABLISHED_MASTER_SENDING_ACK:
		//we are master but ecu expects an ACK
		//3 block length, block counter, ack cmd 9, block end 3 (no response)
		if ( kwp1281_send_block_state % 2 ) {
			//odd -> receiv
			 if ( Serial2.available() ) {
					Serial2.read();
					kwp1281_send_block_state++;
			 } else {
				 if ( kwp1281_kline_millis_last_byte_send + KWP1281_INTERBLOCK_TIMEOUT > millis() )
					 kwp1281_state = KWP1281_STATE_NO_CONNECTION;
			 }
		} else {
			//even ->  send
			switch ( kwp1281_send_block_state ) {
			case 0:
				kwp1281Write ( 3 );
				kwp1281_send_block_state++;
				break;
			case 2:
				kwp1281_block_counter++;
				kwp1281Write ( kwp1281_block_counter );
				kwp1281_send_block_state++;
				break;
			case 4:
				//special case if ecu wants to send more than 4 blocks
				if ( kwp1281_controller_data_block_counter == 4 && kwp1281_controller_data_more_than_four_blocks )
					kwp1281Write ( KWP1281_COMMAND_GET_CONTROLLER_DATA );
				else
					kwp1281Write ( KWP1281_COMMAND_ACK );
				kwp1281_send_block_state++;
				break;
			case 6:
				//block end; no response; ecu gets master
				kwp1281Write ( 3 );
				kwp1281_send_block_state = 0;
				kwp1281_state = KWP1281_STATE_ESTABLISHED_SLAVE;
			}
		}
		break;
	case KWP1281_STATE_ESTABLISHED_SLAVE:
		kwp1281ReceiveBlock (true);
		kwp1281_state = KWP1281_STATE_ESTABLISHED_SLAVE_RECEIVING_BLOCK;
		break;
	case KWP1281_STATE_ESTABLISHED_SLAVE_RECEIVING_BLOCK:
		if ( kwp1281_kline_millis_last_byte_received + KWP1281_INTERBLOCK_TIMEOUT > millis() ) {
			kwp1281_state = KWP1281_STATE_NO_CONNECTION;
		}
		if ( kwp1281ReceiveBlock (false) ) {
			kwp1281_kline_millis_last_byte_received = millis();
			//block completely received!
			switch ( kwp1281_block_title ) {
			case KWP1281_COMMAND_ASCII_DATA:
				//TODO do somethin with
				kwp1281_state = KWP1281_STATE_ESTABLISHED_MASTER_SEND_ACK;
				break;
			case KWP1281_COMMAND_ACK:
				kwp1281_state = KWP1281_STATE_ESTABLISHED_MASTER;
				break;
			case KWP1281_COMMAND_GROUP_READING_RESPONSE:
				//TODO do something with the data
				//ACK or next function
				kwp1281_state = KWP1281_STATE_ESTABLISHED_MASTER;
				break;
			}
		}
		break;
	}
}

bool MultidisplayController::kwp1281ReceiveBlock( bool init ) {
	if ( init ) {
		kwp1281_block_state = KWP1281_BLOCKREAD_STATE_WAIT_LENGTH;
		kwp1281IncActiveFrame ();
		kwp1281_kline_index = -1;
	}

	if ( ( kwp1281_kline_millis_last_byte_send + KWP1281_INTERBLOCK_TIMEOUT < millis() )
	|| ( kwp1281_kline_millis_last_byte_received + KWP1281_INTERBLOCK_TIMEOUT < millis() ) ) {
		kwp1281_state = KWP1281_STATE_NO_CONNECTION;
#ifdef KWP1281_KLINE_DEBUG
		Serial.print ("kwp1281 timeout -> state 0");
#endif
		return false;
	}

	switch ( kwp1281_block_state ) {
		case KWP1281_BLOCKREAD_STATE_WAIT_LENGTH:
			if ( kwp1281Read () ) {
				//we got data
				kwp1281_block_length = kwp1281_klineData[kwp1281_kline_active_frame].asBytes[kwp1281_kline_index];
				kwp1281Write( KWP1281_COMP(kwp1281_block_length) );
				kwp1281_block_state = KWP1281_BLOCKREAD_STATE_WAIT_COUNTER;
			}
			break;
		case KWP1281_BLOCKREAD_STATE_WAIT_COUNTER:
			if ( kwp1281Read () ) {
				//we got data
				kwp1281_block_counter = kwp1281_klineData[kwp1281_kline_active_frame].asBytes[kwp1281_kline_index];
				kwp1281Write( KWP1281_COMP(kwp1281_block_counter) );
				kwp1281_block_state = KWP1281_BLOCKREAD_STATE_WAIT_TITLE;
			}
			break;
		case KWP1281_BLOCKREAD_STATE_WAIT_TITLE:
			if ( kwp1281Read () ) {
				//we got data
				kwp1281_block_title = kwp1281_klineData[kwp1281_kline_active_frame].asBytes[kwp1281_kline_index];
				kwp1281Write( KWP1281_COMP(kwp1281_block_title) );
				kwp1281_block_state = KWP1281_BLOCKREAD_STATE_WAIT_DATA;
			}
			break;
		case KWP1281_BLOCKREAD_STATE_WAIT_DATA:
			if ( kwp1281Read () ) {
				//we got data
				kwp1281Write( KWP1281_COMP( kwp1281_klineData[kwp1281_kline_active_frame].asBytes[kwp1281_kline_index] ) );
				if ( kwp1281_kline_index == kwp1281_block_length - 1 )
					//last data byte -> next will be block end
					kwp1281_block_state = KWP1281_BLOCKREAD_STATE_WAIT_BLOCKEND;
			}
			break;
		case KWP1281_BLOCKREAD_STATE_WAIT_BLOCKEND:
			if ( kwp1281Read () ) {
				//read should be 3
				return true;
			}
			break;
	}
	return false;
}

bool MultidisplayController::kwp1281Read () {
	if ( Serial2.available() ) {
		kwp1281_kline_index++;
		kwp1281_klineData[kwp1281_kline_active_frame].asBytes[kwp1281_kline_index] = Serial2.read();
		kwp1281_kline_millis_last_byte_received = millis();
#ifdef KWP1281_KLINE_DEBUG
		Serial.print ("data ");
		Serial.print (kwp1281_klineData[kwp1281_kline_active_frame].asBytes[kwp1281_kline_index]);
		Serial.write ("\n");
#endif
		return true;
	}
	return false;
}
void MultidisplayController::kwp1281Write (uint8_t d) {
	Serial2.write(d);
	kwp1281_kline_millis_last_byte_send = millis();
}

#endif

#if defined(MULTIDISPLAY_V2) && defined(GEAR_RECOGNITION)

void MultidisplayController::gear_computation () {

	float ratio = 0;

	switch (gear_state) {
	case GEAR_STATE_NEED_RECOGNITION:
		/*
		 * need recomputation if clutch pressed
		 * main loop sets state GEAR_STATE_NEED_RECOGNITION
		 *
		 */

		if ( data.speed == 0 ) {
			data.gear = 0;
			if ( data.calThrottle >= 50 )
				data.gear = 1;
			gear_state = GEAR_STATE_MATCHED;
			return;
		}

		ratio = ( data.calRPM * 6 * ABROLLUMFANG ) / ( data.speed * 100 );

		for ( uint8_t i = 0 ; i < GEARS ; i++ ) {
			float lower = gear_ratio[i] * 0.9;
			float upper = gear_ratio[i] * 1.1;

			if ( (lower < ratio) && (ratio < upper) ) {
				data.gear = i+1;
				gear_state = GEAR_STATE_MATCHED;
				return;
			} else {
				// no gear found
//				if ( gear_wait_before_idle < millis() ) {
//					//we waited long enough
//					if ( data.calThrottle == 0 && data.calRPM <= GEAR_IDLE_RPM )
//						// we're idle with no gear
//						data.gear = 0;
//				}
				/*
				 * should we save the rpm before the clutch was pressed ?
				 * this would allow to see quick (new rpm lower or greater)
				 * if its a down or upshift.
				 *
				 * -> TODO test it on the road!
				 */
			}
		}

		break;
	case GEAR_STATE_MATCHED:
		/*
		 * we've successfully detected a gear
		 * TODO should we periodically recompute?
		 */
		//data.gear = 0;

		break;
	}
}

#endif
