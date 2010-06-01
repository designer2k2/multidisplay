/*
    Copyright 2009-10 Stephan Martin, Dominik Gummel

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
#include "LCDScreen.h"
#include "LCDScreen8.h"
#include "LCDScreen7.h"
#include "BoostController.h"

#include <stdlib.h>
#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <wiring.h>
#include <EEPROM.h>
#include <WProgram.h>

#include <PID_Beta6.h>

/**
 * EEPROM Layout:
 *
 * Start Byte | type | name
 * 100 | byte | activeScreen
 * 105 | byte | brightness
 * 200 | double | calLD
 * 205 | byte | ldCalPoint
 */

//Lookup Table for the TypK:
//from 0-1350°C in steps of 50°C, the list is in microV according to that Temp.
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
//from 0-10Bar in steps of 1Bar, the list is in 12Bit Digital Reading when supplied with 5V and a 200Ohm Resistor in series
//(measuring the Voltage on the Sensor)
//it has a increasing Resistance with the Pressure.
const unsigned int MultidisplayController::tempVDOPressure[] =
{
		0,
		195,
		549,
		845,
		1073,
		1271,
		1427,
		1567,
		1686,
		1794,
		1881,
		1962
};

//Lookup Table for the VDOtemperature (22 Values) Its Calibration curve 92-027-006
//from -30C-180C in steps of 10C, the list is in 12Bit Digital Reading when supplied with 5V and a 200Ohm Resistor in series
//(measuring the Voltage on the Sensor)
//it has a decreasing Resistance with the Temperature
const unsigned int MultidisplayController::tempVDOTemp[] =
{
		4053,
		4019,
		3957,
		3857,
		3706,
		3497,
		3224,
		2893,
		2527,
		2150,
		1790,
		1471,
		1200,
		968,
		782,
		632,
		515,
		422,
		348,
		287,
		238,
		199
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
	SerOut = SERIALOUT_RAW;

	buttonTime = 0;

	pinMode (LCDBRIGHTPIN, OUTPUT);
	//test
	pinMode (NORDSCHLEIFENPIN, INPUT);
	digitalWrite( NORDSCHLEIFENPIN, HIGH); // turn on pullup resistors
	pinMode (FREEANALOG2, INPUT);
	digitalWrite( FREEANALOG2, HIGH); // turn on pullup resistors

	pinMode (N75PIN, OUTPUT);
	//http://www.arduino.cc/playground/Main/TimerPWMCheatsheet
	//http://www.arcfn.com/2009/07/secrets-of-arduino-pwm.html
	//set pwm freq tp 30Hz
	//11
	TCCR2B = (TCCR2B & 0b11111000) | 0x7;

//	pinMode (FREEPWM2, OUTPUT);
//	TCCR1B = (TCCR1B & 0b11111000) | 0x5;

	//set pin modes
	pinMode(DATAOUT, OUTPUT);
	pinMode(DATAIN, INPUT);
	pinMode(SPICLOCK, OUTPUT);
	digitalWrite(DATAOUT,LOW);
	digitalWrite(SPICLOCK,LOW);

	Serial.begin(57600);

	wire.begin();                  //Start the Wire Libary for the PCF8574

	expanderWrite2(IOport2); //Switch off all Devices

	//Print the Info:
//	Serial.println(" ");
	Serial.println("MultiDisplay 1.1!");


#ifdef READFROMEEPROM
	//Read the Values from the EEPROM back

	//what screen was last shown?
	//FIXME
//	lcdController.setActiveScreen (EEPROM.read(100));

	lcdController.setBrightness (EEPROM.read(105));    //The Brightness from the LCD
	uint8_t ldp = EEPROM.read(205);
	if ( ldp >= 0 && ldp <= 20 )
		data.ldCalPoint = ldp;
	float ldt = EEPROMReadDouble(200)/1000.0;      //gets the float back (thats accurate enough)
	if ( ldt > 0.0 && ldt < 1.2 )
		data.boostAmbientPressureBar = ldt;
#endif

	lcdController.lcdShowIntro(INITTIME);                      //Shows the Into
	lcdController.init();

	//Init the Buttons:
	expanderWrite(0b10000011);        //This may needs to be modified when a third button is attached.

	serialTime = millis();
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

byte MultidisplayController::expanderRead() {
	byte _data = 0;
	wire.requestFrom(EXPANDER, 1);
	if(wire.available()) {
		_data = wire.receive();
	}
	return _data;
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

void MultidisplayController::AnaConversion() {
	//This converts all 16 Analog Values into the real deal :)

	//Boost:
#ifdef BOOST_PLX_SMVACBOOST
	//see http://plxdevices.com/images/SM-VacBoostVolts.jpg
	// voltage = boost(psi)/9 + 1,66
	// boost (psi) = voltage * 9 - 15
	data.calAbsoluteBoost = 5.0* ( (float) data.anaIn[BOOSTPIN])/4096.0;
	data.calAbsoluteBoost = data.calAbsoluteBoost * 9.0 - 15; //psi
	data.calAbsoluteBoost = data.calAbsoluteBoost / BAR2PSI; //bar
	data.calBoost = data.calAbsoluteBoost; //sm vac/boost gets relative boost
#else
	//or Motorola MPX 4250 datasheet
	data.calAbsoluteBoost = 5.0* ((float) data.anaIn[BOOSTPIN])/4096.0;             //only gets 0-5V
	data.calAbsoluteBoost = (data.calAbsoluteBoost * 50 - 10)/100;     	//makes 0-250kPa out of it
//	data.calBoostPSI = data.calBoostBar * BAR2PSI;
	data.calBoost = data.calAbsoluteBoost - data.boostAmbientPressureBar;			//apply the offset (ambient pressure)
#endif
    //Calibration for RPM (its 2.34!)
	//Check if the Boost is a new Max Boost Event
	if( data.calBoost >= data.maxLdE[1] ) 	{
		SaveMax(1);
	}

	//Lambda:
#ifdef LAMBDA_WIDEBAND
	//http://www.plxdevices.com/InstallationInstructions/SM-AFRUsersGuide.pdf
	// air fuel ratio = 2*Voltage + 10
	data.calLambdaF = ( (5.0*( ((float) data.anaIn[LAMBDAPIN]) /4096)) * 2 + 10 ) / 14.7;
	data.calLambda = map(data.anaIn[LAMBDAPIN], 0, 4096, 0, 200);
#else
	data.calLambda = map(data.anaIn[LAMBDAPIN], LAMBDAMIN, LAMBDAMAX, 0, 200);    //gets about the 0-1V into 0-200 values
	data.calLambdaF = (flaot) data.calLambda;
#endif
	data.calLambda = constrain(data.calLambda, 0, 200);

	//CaseTemp: (damped)
	data.calCaseTemp = data.calCaseTemp*0.9 + (500.0*data.anaIn[CASETEMPPIN]/4096.0)*0.1;  //thats how to get °C out from a LM35 with 12Bit ADW

#ifdef VR6_MOTRONIC
	//Throttle:
	data.calThrottle = map(data.anaIn[THROTTLEPIN], THROTTLEMIN, THROTTLEMAX, 0, 100);
	data.calThrottle = constrain(data.calThrottle, 0, 100);
	data.calThrottle = 100 - data.calThrottle;                 //VR6 has 5V for closed and 0V for open throttle...
#endif

#ifdef DIGIFANT
    //digifant idle switch (LL-Schalter) gets 0V for closed throttle valve; ~5V if open
    // assumption: if > 4 V: open
    if ( data.anaIn[LMMPIN] < 3276 )
            data.calThrottle = 0;
    else {
            //full throttle ?
            if ( data.anaIn[THROTTLEPIN] > 3276 )
                    data.calThrottle = 50;
            else
                    data.calThrottle = 100;
    }
#endif

#ifdef VR6_MOTRONIC
	//LMM
	data.calLMM = 5.0*data.anaIn[LMMPIN]/4096.0;		   //makes 0.0 to 5.0 Volt out of it, with VagCom this could be maped to gr Air i think
#endif

	//RPM
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
	if ( data.calRPM >= data.maxRpmE[2] ) {
		SaveMax(2);
	}


	//Battery Voltage: (Directly from the Arduino!, so only 1024, not 4096.)
	//measured Voltage * 4,09 + 0,7V should give the supply voltage
	data.batVolt = ((5.0*analogRead(BATTERYPIN)/1023.0)*4.09)+0.7;

	//Lets do the Typ K Conversion:
	if(DoTypK==1) 	{
		FetchTypK();
	}

	//VDO Stuff:

	data.VDOTemp1 = GetVDOTemp(data.anaIn[VDOT1PIN]);
	data.VDOTemp2 = GetVDOTemp(data.anaIn[VDOT2PIN]);
	data.VDOTemp3 = GetVDOTemp(data.anaIn[VDOT3PIN]);
	data.VDOPres1 = GetVDOPressure(data.anaIn[VDOP1PIN]);
	data.VDOPres2 = GetVDOPressure(data.anaIn[VDOP2PIN]);
	data.VDOPres3 = GetVDOPressure(data.anaIn[VDOP3PIN]);

}


void MultidisplayController::Shiftlight()
{
	//This sets the brightness according to the RPM
	if(data.calRPM >= RPM_SHIFT_LOW && data.calRPM <= RPM_SHIFT_HIGH) 	{
		analogWrite(RPMSHIFTLIGHTPIN,RPM_LOW_BRIGHT);
	} else if(data.calRPM>= RPM_SHIFT_HIGH) 	{
		analogWrite(RPMSHIFTLIGHTPIN,RPM_HIGH_BRIGHT);
	} else if(data.calRPM< RPM_SHIFT_LOW) {
		analogWrite(RPMSHIFTLIGHTPIN,RPM_NO_BRIGHT);
	}
}





void MultidisplayController::serialReceive() {

	int index=0;
	byte Auto_Man = -1;
	while(Serial.available()&&index<25)
	{
		if(index==0) {
			Auto_Man = Serial.read();
		} else
			srData.asBytes[index-1] = Serial.read();
		index++;
	}

//	if ( index > 0 ) {
//		Serial.print ("DEBUG rcv");
//		Serial.print (index);
//		Serial.println();
//	}

	// if the information we got was in the correct format,
	// read it into the system
	// case 1: command for pid lib
	if(index==25  && (Auto_Man==0 || Auto_Man==1))
	{
#ifdef BOOSTN75

		/* we get fixed point values (base 1000) from the pc ! */

		boostController.boostSetPoint = double(srData.asFixedInt32[0] / 1000.0);
		if(Auto_Man==0) {
			// * only change the output if we are in manual mode
			boostController.boostOutput = double(srData.asFixedInt32[2] / 1000.0);
		}

		double p, i, d;  // * read in and set the controller tunings
		p = double(srData.asFixedInt32[3] / 1000.0);
		i = double(srData.asFixedInt32[4] / 1000.0);
		d = double(srData.asFixedInt32[5] / 1000.0);
		boostController.boostPid->SetTunings(p, i, d);

		if(Auto_Man==0)
			boostController.boostPid->SetMode(MANUAL);// * set the controller mode
		else
			boostController.boostPid->SetMode(AUTO);
#endif
	} else 	if (Auto_Man==2 && index >= 2) {
		//case 2: command for multidisplay

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

	Serial.flush();                         // * clear any random data from the serial buffer
}


void MultidisplayController::serialSend() {
#ifdef BOOSTN75
	  Serial.print("\2");
	  Serial.print("PID ");
	  Serial.print(boostController.boostSetPoint);
	  Serial.print(" ");
	  Serial.print(data.calBoost);
	  Serial.print(" ");
	  Serial.print(boostController.boostOutput);
	  Serial.print(" ");
	  Serial.print(boostController.boostPid->GetP_Param());
	  Serial.print(" ");
	  Serial.print(boostController.boostPid->GetI_Param());
	  Serial.print(" ");
	  Serial.print(boostController.boostPid->GetD_Param());
	  Serial.print(" ");
	  if (boostController.boostPid->GetMode()==AUTO)
		  Serial.print("Automatic");
	  else
		  Serial.print("Manual");
	  Serial.print("\3");
	  Serial.println();
#endif /* BOOSTN75 */

	Serial.print("\2");
	switch(SerOut){
	case SERIALOUT_DISABLED:
		break;
	case SERIALOUT_RAW:
		//RAW Output:
		Serial.print(time);
		Serial.print(";");
		for(int I = 1; I <=15;I++){
			Serial.print(data.anaIn[I]);
			Serial.print(";");
		}
		Serial.print(data.anaIn[16]);
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
		Serial.print(data.calAgt[0]);
		Serial.print(";");
		Serial.print(data.calAgt[1]);
		Serial.print(";");
		Serial.print(data.batVolt);
		break;
	default:
		SerOut = SERIALOUT_DISABLED;
		break;
	}
	Serial.print("\3");
	Serial.println();
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
	case SERIALOUT_RAW:
		Serial.println(" ");
		Serial.println_P(PSTR("Time;data.anaIn0;data.anaIn1;data.anaIn2;data.anaIn3;data.anaIn4;data.anaIn5;data.anaIn6;data.anaIn7"));
		break;
	case SERIALOUT_ENABLED:
		Serial.println(" ");
		Serial.print(SERIALOUT_ENABLED);
		Serial.print(":");
		Serial.println_P(PSTR("Time;RPM;Boost;Throttle;Lambda;LMM;CaseTemp;TypK1;TypK2;Battery"));
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
		SerOut = SERIALOUT_RAW;
		break;
	case SERIALOUT_RAW:
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

void MultidisplayController::CalibrateLD()
{
	data.boostAmbientPressureBar = data.calAbsoluteBoost;
	// changed from global val3 to caluclation of mapped boost
	data.ldCalPoint = map(data.anaIn[BOOSTPIN], 0, 4096, 0, 200) / 10;
	//and saved:
	EEPROM.write(205,data.ldCalPoint);
	EEPROMWriteDouble(200,data.boostAmbientPressureBar*1000);    //writes the float as long, will do it.

	//The MaxLD will be reset!
	data.maxLd = 0.0;
	data.maxLdt = data.ldCalPoint;

	Serial.println(" ");
	Serial.print("CalBoost: ");
	Serial.println(data.boostAmbientPressureBar);
}

//-------------------------------------------------------------------------------------------------------

//This converts the thermocouple µV reading into some usable °C
int MultidisplayController::GetTypKTemp(unsigned int microVolts)
{
	//TODO dont use linear search!

	int LookedupValue = 0;
	//This searches the 2 surrounding values, and then linear interpolates between them.
	for(int i = 0; i<TEMPTYPKREADINGS;i++) 	{
		if(microVolts >= pgm_read_word(&tempTypK[i]) && microVolts <= pgm_read_word(&tempTypK[i+1]))
		{
			LookedupValue = ((i)*50) + ((50L *(microVolts - pgm_read_word(&tempTypK[i]))) / ((pgm_read_word(&tempTypK[i+1]) - pgm_read_word(&tempTypK[i]))));
			break;
		}
	}
	return LookedupValue;
}

//Converts the ADW Reading into �C
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

	LookedupValue -= 30;   //must be reduced as the lookup table starts at -30°C.
	LookedupValue = constrain(LookedupValue,-40,999);    //Limits the Output to 999°C, so an open circuit gets detectet!

	return LookedupValue;
}

//Converts the ADW Reading into 0.1Bar (must be divided by 10 for Bar)
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


void MultidisplayController::FetchTypK()  {
	//This will read in all the needed Analog values, convert them to �C, and make the calibration with the LM35
	//so it must be called after the AnaConversion!

	unsigned int Temp = 0;

	//http://www.arduino.cc/playground/Learning/4051
	int r0;      //value select pin at the 4051 (s0)
	int row;     // storeing the bin code
	int  bin [] = {000, 1, 10, 11, 100, 101, 110, 111};//bin = binary, some times it is so easy

	for (int i=0; i <= NUMTYPK; i++) {
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

		delay(20);   //Due to the 0.1 µF cap this is needed. The Cap should be there to get a stable reading! (from open to RT it takes 15ms, plus 5ms safety)
		//then read in the value from the ADW

		Temp = ((5.0*read_adc(AGTPIN))/4096.0)*10000;   //gets the Volts and makes �V out of it (100 is already added from the Amp)
		Temp = GetTypKTemp(Temp);                       //Converts the �V into °C

		//Check if it is open:

		if(Temp>=MAXTYPK) {
			Temp = 0;
		} else {
			Temp += int(data.calCaseTemp);                       //apply the Correction
		}

		data.calAgt[i] = Temp;              //Save it into the array

		//Check if the Temp is a new Max Temp Event
		if(Temp>=data.maxAgtValE[3]) {
			SaveMax(3);
		}

		//repeat for all NumTypK+1 channels.
	}
}






//----------------------------------------------------------------------------------------------------
//This checks certain values if they exceed limits or not, if so an alarm is triggered
void MultidisplayController::CheckLimits()
{
	uint8_t Brightness = lcdController.brightness;
	uint8_t FlashTrigger=0;

	/*
  if(VDOPres1>MaxOP)
  {
    FlashTrigger = 1;    //Enable the LCDFlash
  }
	 */

	if(data.calAgt[0]>MaxAGT) {
		FlashTrigger = 1;    //Enable the LCDFlash
	}


	if(data.calAgt[1]>MaxAGT) 	{
		FlashTrigger = 1;    //Enable the LCDFlash
	}


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
			lcdController.setBrightness(Brightness);              //And set the Brightness
		}
	}
	else {
		//?!?
		Brightness = EEPROM.read(105);           //Set back the Brightness
		lcdController.setBrightness(Brightness);              //And set the Brightness
	}
	lcdController.setBrightness(Brightness);

}

//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
//Saves all Data to the arrays
void MultidisplayController::SaveMax(uint8_t Num)
{
	data.maxAgtValE[Num] = data.calAgt[0];
	data.maxLdE[Num] = data.calBoost;
	data.maxRpmE[Num] = data.calRPM;
	data.maxLmmE[Num] = data.calLMM;
	data.maxOilE[Num] = data.VDOPres1;
}







void MultidisplayController::mainLoop() {
	//Current Time:
	time = millis();

	//Read in all Analog values:
	for(uint8_t i = 1; i <=16;i++) {
		data.anaIn[i] = read_adc(i);
	}

	AnaConversion();
	//Calibrate them:
	if(DoCal == 1) {
		AnaConversion();

#ifdef RPM_SHIFT_LIGHT
		Shiftlight();
#endif //RPM_SHIFT_LIGHT
	}


	//Check for Limits:
	//if(DoCheck == 1)
	// {
	CheckLimits();
	// }

	// ui knows what screen is active and draws it!
	lcdController.draw();

	//My own Button Check Function
	buttonCheck(expanderRead());

	//Saves the Screen when needed:

	if(millis()>= ScreenSave) {
		//and now save it:
		EEPROM.write(100, lcdController.activeScreen);
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

	boostController.boostPid->Compute();
	analogWrite(N75PIN, (int) boostController.boostOutput);
//	analogWrite(FREEPWM2, (int) boostController.boostOutput);
#endif

	if ( millis() > serialTime ) {
		serialReceive();
		//Print it:
		serialSend();
		serialTime += SERIALFREQ;
	}
}



/*==============================================================================
 * BUTTON FUNCTIONS
 *============================================================================*/

void MultidisplayController::buttonAHold() {

#ifdef DEBUG
	Serial.println ("A Hold");
#endif

	lcdController.toggleScreen();
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

	switch(lcdController.activeScreen){
	case 1:
		ChangeSerOut();      //Switch from RAW to Cal to Nothing and vise versa.
		break;
	case 2:
		//Toggle A and B MCP
		lcdController.myScreens[1]->toggleScreenAB();
		break;
	case 3:
		ChangeSerOut();      //Switch from RAW to Cal to Nothing and vise versa.
		break;
//	case 6:                 //Switches the 2 Row Screen
//		lcdController.myScreens[5]->toggleScreenAB();
//		break;
//	case 7:
//		lcdController.myScreens[6]->toggleRefreshCounter();
//		break;
	default:
		break;
	}
}

//-------------------------------------------------------------------------------------------------------

void MultidisplayController::buttonBHold() {

#ifdef DEBUG
	Serial.println ("B Hold");
#endif

	//Serial.print(time*1);
	//Serial.print(";");
	//Serial.println("Button B Hold");

	switch(lcdController.activeScreen){
	case 0:
		break;
	case 1:
		//The Calibration from the LD will be done
		CalibrateLD();
		break;
	default:
		break;
	}

}

//-------------------------------------------------------------------------------------------------------

void MultidisplayController::buttonBPressed() {

#ifdef DEBUG
	Serial.println ("B pressed");
#endif

	//Serial.print(time*1);
	//Serial.print(";");
	//Serial.println("Button B Pressed");

	LCDScreen* l;

	switch(lcdController.activeScreen){
	case 0:
		break;
	case 1:
		//The MaxLD will be reset!
		data.maxLd = 0.0;
		data.maxLdt = data.ldCalPoint;
		break;
	case 2:
		//Change LCD brightness
		lcdController.toggleBrightness();

		//and save the new value:
		EEPROM.write(105,lcdController.brightness);
		break;

	case 7:
		l = lcdController.getLCDScreen(6);
		if ( l ) {
			((LCDScreen7*)l)->min = 0;
			((LCDScreen7*)l)->max = 5000;

			((LCDScreen7*)l)->toggleMode();
		}
		break;

	case 8:
		l = lcdController.getLCDScreen(7);
		if ( l ) {
			((LCDScreen8*)l)->toggleMax();
		}
		break;

	default:
		break;
	}
}

//-------------------------------------------------------------------------------------------------------

void MultidisplayController::buttonCheck(int buttonState)  {
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


