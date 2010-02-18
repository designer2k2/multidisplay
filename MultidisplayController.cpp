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

#include <stdlib.h>
#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <wiring.h>
#include <EEPROM.h>
#include <WProgram.h>


 //Lookup Table for the TypK:
 //from 0-1350ï¿½C in steps of 50ï¿½C, the list is in ï¿½V according to that Temp.
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
 //from 0-10Bar in steps of 1Bar, the list is in 12Bit Digital Reading when supplyed with 5V and a 200Ohm Resistor in series
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
 //from -30C-180C in steps of 10C, the list is in 12Bit Digital Reading when supplyed with 5V and a 200Ohm Resistor in series
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
	// TODO Auto-generated constructor stub
	IOport2 = 0b11111111;
	wire = TwoWire();
	sdata = SensorData();
	ui = UserInterface(&sdata, this);

	FlashETimeU = 0;
	FlashTimeU = 0;
	ScreenSave = 0;
	time = 0;
	val3=0;
	sdata.calLd = 0.0;          //calibration from the Boost
	sdata.maxLd = 0;
	sdata.maxLdt=0;               //max LD for the screen

	DoCheck = 1;

	// the following was method void setup() {
	pinMode(LCDBRIGHTPIN, OUTPUT);

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
	Serial.println(" ");
	Serial.println("MultiDisplay PRE!");

	//Read the Values from the EEPROM back
	lcdController.activeScreen = EEPROM.read(100);        //what screen was last shown?

	lcdController.setBrightness (EEPROM.read(105));    //The Brightness from the LCD
	sdata.ldCalPoint = EEPROM.read(205);
	sdata.calLd = EEPROMReadDouble(200)/1000.0;      //gets the float back (thats accurate enough)

	lcdController.lcdShowIntro(INITTIME);                      //Shows the Into


	lcdController.init();


	//Init the Buttons:
	expanderWrite(0b10000011);        //This may needs to be modified when a third button is attached.
}

MultidisplayController::~MultidisplayController() {
	// TODO Auto-generated destructor stub
}



//http://www.arduino.cc/playground/Code/MCP3208
int MultidisplayController::read_adc(uint8_t channel){
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

void MultidisplayController::AnaConversion()
{
	//This converts all 16 Analog Values into the real deal :)

	//Boost:
	sdata.calRAWBoost = 5.0*sdata.anaIn[BOOSTPIN]/4096.0;             //only gets 0-5V
	sdata.calRAWBoost = (sdata.calRAWBoost * 50 - 10)/100;     	//makes 0-250kPa out of it
	sdata.calBoost = sdata.calRAWBoost - sdata.calLd;			//apply the offset (ambient pressure)

	//Check if the Boost is a new Max Boost Event
	if( sdata.calBoost >= sdata.maxLdE[1] ) 	{
		SaveMax(1);
	}

	//Lambda:
	sdata.calLambda = map(sdata.anaIn[LAMBDAPIN], LAMBDAMIN, LAMBDAMAX, 0, 200);    //gets about the 0-1V into 0-200 values
	sdata.calLambda = constrain(sdata.calLambda, 0, 200);

	//CaseTemp: (damped)
	sdata.calCaseTemp = sdata.calCaseTemp*0.9 + (500.0*sdata.anaIn[CASETEMPPIN]/4096.0)*0.1;  //thats how to get °C out from a LM35 with 12Bit ADW

	//Throttle:
	sdata.calThrottle = map(sdata.anaIn[THROTTLEPIN], THROTTLEMIN, THROTTLEMAX, 0, 100);
	sdata.calThrottle = constrain(sdata.calThrottle, 0, 100);
	sdata.calThrottle = 100 - sdata.calThrottle;                 //VR6 has 5V for closed and 0V for open throttle...

	//LMM:
	sdata.calLMM = 5.0*sdata.anaIn[LMMPIN]/4096.0;		   //makes 0.0 to 5.0 Volt out of it, with VagCom this could be maped to gr Air i think

	//RPM
	//(from the smoothing example)
	sdata.rpmTotal -= sdata.rpmReadings[sdata.rpmIndex];               // subtract the last reading
	sdata.rpmReadings[sdata.rpmIndex] = sdata.anaIn[RPMPIN];           // read from the sensor
	sdata.rpmTotal += sdata.rpmReadings[sdata.rpmIndex];               // add the reading to the total
	sdata.rpmIndex = (sdata.rpmIndex + 1);                       // advance to the next index

	if (sdata.rpmIndex >= RPMSMOOTH)                       // if we're at the end of the array...
	{
		sdata.rpmIndex = 0;                                  // ...wrap around to the beginning
	}

	sdata.rpmAverage = sdata.rpmTotal / RPMSMOOTH;               // calculate the average
	sdata.calRPM = sdata.rpmAverage*RPMFACTOR;                   // apply the factor for calibration

	//Check if the RPM is a new Max RPM Event
	if ( sdata.calRPM >= sdata.maxRpmE[2] ) {
		SaveMax(2);
	}


	//Battery Voltage: (Directly from the Arduino!, so only 1024, not 4096.)
	//measured Voltage * 4,09 + 0,7V should give the supply voltage
	sdata.batVolt = ((5.0*analogRead(BATTERYPIN)/1023.0)*4.09)+0.7;

	//Lets do the Typ K Conversion:
	if(DoTypK==1) 	{
		FetchTypK();
	}

	//VDO Stuff:

	sdata.VDOTemp1 = GetVDOTemp(sdata.anaIn[VDOT1PIN]);
	sdata.VDOTemp2 = GetVDOTemp(sdata.anaIn[VDOT2PIN]);
	sdata.VDOTemp3 = GetVDOTemp(sdata.anaIn[VDOT3PIN]);
	sdata.VDOPres1 = GetVDOPressure(sdata.anaIn[VDOP1PIN]);
	sdata.VDOPres2 = GetVDOPressure(sdata.anaIn[VDOP2PIN]);
	sdata.VDOPres3 = GetVDOPressure(sdata.anaIn[VDOP3PIN]);

}


void MultidisplayController::Shiftlight()
{
	//This sets the brightness according to the RPM
	if(sdata.calRPM >= RPM_SHIFT_LOW && sdata.calRPM <= RPM_SHIFT_HIGH) 	{
		analogWrite(RPMSHIFTLIGHTPIN,RPM_LOW_BRIGHT);
	} else if(sdata.calRPM>= RPM_SHIFT_HIGH) 	{
		analogWrite(RPMSHIFTLIGHTPIN,RPM_HIGH_BRIGHT);
	} else if(sdata.calRPM< RPM_SHIFT_LOW) {
		analogWrite(RPMSHIFTLIGHTPIN,RPM_NO_BRIGHT);
	}
}



void MultidisplayController::SerialPrint() {
	switch(SerOut){
	case SERIALOUT_DISABLED:
		break;
	case SERIALOUT_RAW:
		//RAW Output:
		Serial.print(time);
		Serial.print(";");
		for(int I = 1; I <=15;I++){
			Serial.print(sdata.anaIn[I]);
			Serial.print(";");
		}
		Serial.println(sdata.anaIn[16]);
		break;
	case SERIALOUT_ENABLED:
		//Convertet Output:
		Serial.print(time);
		Serial.print(";");
		Serial.print(sdata.calRPM);
		Serial.print(";");
		Serial.print(sdata.calBoost);
		Serial.print(";");
		Serial.print(sdata.calThrottle);
		Serial.print(";");
		Serial.print(sdata.calLambda);
		Serial.print(";");
		Serial.print(sdata.calLMM);
		Serial.print(";");
		Serial.print(sdata.calCaseTemp);
		Serial.print(";");
		Serial.print(sdata.calAgt[0]);
		Serial.print(";");
		Serial.print(sdata.calAgt[1]);
		Serial.print(";");
		Serial.println(sdata.batVolt);
		break;
	default:
		SerOut = SERIALOUT_DISABLED;
		break;
	}

}

void MultidisplayController::HeaderPrint()
{

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
		Serial.println("Time;sdata.anaIn0;sdata.anaIn1;sdata.anaIn2;sdata.anaIn3;sdata.anaIn4;sdata.anaIn5;sdata.anaIn6;sdata.anaIn7");
		break;
	case SERIALOUT_ENABLED:
		Serial.println(" ");
		Serial.println("Time;RPM;Boost;Throttle;Lambda;LMM;CaseTemp;TypK1;TypK2;Battery");
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
	sdata.calLd = sdata.calRAWBoost;
	// changed from global val3 to caluclation of mapped boost
	sdata.ldCalPoint = map(sdata.anaIn[BOOSTPIN], 0, 4096, 0, 200) / 10;
	//and saved:
	EEPROM.write(205,sdata.ldCalPoint);
	EEPROMWriteDouble(200,sdata.calLd*1000);    //writes the float as long, will do it.

	//The MaxLD will be reset!
	sdata.maxLd = 0.0;
	sdata.maxLdt = sdata.ldCalPoint;

	Serial.println(" ");
	Serial.print("CalBoost: ");
	Serial.println(sdata.calLd);
}

//-------------------------------------------------------------------------------------------------------

//This converts the thermocouple µV reading into some usable °C
int MultidisplayController::GetTypKTemp(unsigned int microVolts)
{
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
			Temp += int(sdata.calCaseTemp);                       //apply the Correction
		}

		sdata.calAgt[i] = Temp;              //Save it into the array

		//Check if the Temp is a new Max Temp Event
		if(Temp>=sdata.maxAgtValE[3]) {
			SaveMax(3);
		}

		//repeat for all NumTypK+1 channels.
	}
}






//----------------------------------------------------------------------------------------------------
//This checks certain values if they exceed limits or not, if so an alarm is triggered
void MultidisplayController::CheckLimits()
{
	int Brightness = lcdController.brightness;
	int FlashTrigger=0;

	/*
  if(VDOPres1>MaxOP)
  {
    FlashTrigger = 1;    //Enable the LCDFlash
  }
	 */

	if(sdata.calAgt[0]>MaxAGT) {
		FlashTrigger = 1;    //Enable the LCDFlash
	}


	if(sdata.calAgt[1]>MaxAGT) 	{
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
void MultidisplayController::SaveMax(int Num)
{
	sdata.maxAgtValE[Num] = sdata.calAgt[0];
	sdata.maxLdE[Num] = sdata.calBoost;
	sdata.maxRpmE[Num] = sdata.calRPM;
	sdata.maxLmmE[Num] = sdata.calLMM;
	sdata.maxOilE[Num] = sdata.VDOPres1;
}







void MultidisplayController::mainLoop() {

	//Current Time:
	time = millis();

	//Read in all Analog values:
	for(uint8_t i = 1; i <=16;i++) {
		sdata.anaIn[i] = read_adc(i);
	}

	//Calibrate them:
	if(DoCal == 1) {
		AnaConversion();

#if RPMShiftLight
		Shiftlight();
#endif //RPMShiftLight
	}

	//Check for Limits:
	//if(DoCheck == 1)
	// {
	CheckLimits();
	// }

	//Print it:
	SerialPrint();

	// ui knows what screen is active and draws it!
	lcdController.draw();

	//My own Button Check Function

	ui.buttonCheck(expanderRead());

	//Saves the Screen when needed:

	if(millis()>= ScreenSave) {
		//and now save it:
		EEPROM.write(100, lcdController.activeScreen);
		//And also prevent a double save!
		ScreenSave = 429400000;        // (thats close to 50days of runtime...)
	}

#if FreeMem
	//Free Mem Debug:
	Serial.print("FM ");
	Serial.println( freeMem());
#endif

#if RPMDebug
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
}


UserInterface* MultidisplayController::getUiP () {
	return &ui;
}
