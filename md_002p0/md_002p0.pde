/*   
    Copyright 2009 Stephan Martin
   
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

/*
 Version: md_002p0pre

This should work :)
*/

#include <EEPROM.h>            //for Settings / Max Value storage
#include <LCD4Bit.h>           //to output to the LCD
//#include <AFSoftSerial.h>      //The Serial Lib  24150
#include <avr/pgmspace.h>      //for PROGMEN 
#include <Wire.h>              //to get the I2C working.

//----------------------------------------------------------------------------------------------------
//Max Values:
#define MaxAGT 1200            //Maximum AGT
#define MaxOP 8                //Maximum Oilpressure
//----------------------------------------------------------------------------------------------------

//Options:

#define RPMDebug 0             //Little Frequency Generator
#define FreeMem 0              //Outputs how much free RAM is

#define InitTime 2000          //How long the Init screen will be shown 
#define RPMShiftLight 1        //Shiftlight?
//--------------------------------------

// Shiftlight Config:

#define RPMShiftLow 6000             //Shiftlight half brightness
#define RPMLowBright 40
#define RPMShiftHigh 6500            //Shiftlight full brightness
#define RPMHighBright 255
#define RPMNoBright 0                //Shiftlight normal brightness

//--------------------------------------

#define ButtonHold 1000         //ms to trigger a Hold (Every Xms a hold is triggered)
#define ScreenSaveDelay 10000	//ms after what it will save the Screen

#define FlashTime 100            //ms the interval when a Limit is exceeded, the LCD will flash with this interval)
#define FlashETime 5000          //how long the flash is shown at a minimum

//--------------------------------------
//Pin Config:

#define DATAOUT 17              //MOSI 
#define DATAIN 12               //MISO 
#define SPICLOCK 13             //Clock 

// The Sensors: (On the MD03 Print, see the Layout for the KLX Connectors)

#define BoostPin 1             //(KL7-2) Currently supports the Motorola 250kpa Sensor
#define Boost2Pin 2            //(KL7-3) Currently supports the Motorola 250kpa Sensor

#define VDOT1Pin 3             //(KL6) T1 VDO Temperature Pin
#define VDOT2Pin 4             //(KL5) T2 VDO Temperature Pin
#define VDOT3Pin 5             //(KL4) T3 VDO Temperature Pin
#define VDOP1Pin 6             //(KL3) P1 VD0 Pressure Pin
#define VDOP2Pin 7             //(KL2) P1 VD0 Pressure Pin
#define VDOP3Pin 8             //(KL1) P1 VD0 Pressure Pin

#define LambdaPin 11           //(KL9-4) Narrow Band Lambda Sensor Input
#define ThrottlePin 12         //(KL9-3) 0-5V Signal from the trothle signal
#define RPMPin 13              //(KL9-2) needs a small Circuit to converts the RPM Signal into Analog Voltages
#define LMMPin 14              //(KL9-1)Signal from the LMM


#define CaseTempPin 15         //The LM35 Signal.
#define AGTPin 16              //Input from the Mux

#define BatteryPin 0           //The Signal from the voltage divider (Analog Pin on the Arduino!)
#define LCDBrightPin 5		     //The Pin for the Brightness of the LCD, it will be PWM.
#define RPMShiftlightPin 3           //where is the Shiftlight connected

//--------------------------------------
//Others:

#define LambdaMin 100            //Calibration for Lambda
#define LambdaMax 1100
#define LambdaLowerLimit 3
#define LambdaUpperLimit 15

#define ThrottleMin 620            //Calibration for Throttle
#define ThrottleMax 3530

#define RPMfactor 2.34              //Calibration for RPM (its 2.34!)
#define RPMsmooth 5                 //how many readings will be taken for the smoothing (5 is quick and smooth, 10 would be supersmooth but its getting slow)
#define RPMmax 3400                 //its about 8k

#define tempTypKReadings 28         //how many entrys are in the Lookup Table
#define NumTypK 1                   //how many TypK`s are attached (enter Number -1)
#define MaxTypK 1170                //over that will be seen as Open.

#define expander B0100001       // Address with 2 address pins grounded. (from the PCF8574)
#define expander2 B0100000      // Address with three address pins grounded. (from the PCF8574)

#define debugRPM 10

int debug = 0;

LCD4Bit lcd = LCD4Bit(4); 		        //Init the LCD (4 Lines, but that dosnt matter)
//AFSoftSerial mySerial =  AFSoftSerial(3, 2);	//Init the Serial on the Real Serial Pins

/*==============================================================================
 * GLOBAL VARIABLES
 *============================================================================*/


//Variables:
int val; 
int val1;
int val2;
int val3;
int val4;

unsigned long ScreenSave = 0;	//timestamp for the Saving

unsigned int ButtonInt = 0;     //Button state
unsigned long ButtonTime = 0;   //Button time

unsigned int Temp;

unsigned long FlashTimeU = 0;    //Time for the Screen to Toggle back
unsigned long FlashETimeU = 0;  

int AnaIn[17];          //All the Analog Values are stored in this array

int ScopeInt[20];      //needed for the Scrolling Scope

int CalAGT[NumTypK+1];  //All Typ K Values will be stored inside this array

unsigned long time;    //the current time


char buf[12];           // "-2147483648\0"

 int Screen;           //current Screen
 int SerOut;           //what serial out will be done
 int DoCal;            //its 1 for doing it, and 0 for no calibration (only RAW possible)
 int DoTypK;            // 1 makes the cal, 0 not.
 int DoCheck = 1;
 int Brightness;
 
 byte IOport2 = B11111111;          //defines what to output on the Interface print (LSB´s are the ADW)
 
 //For the Boost:
 float CalLD = 0.0;          //calibration from the Boost
 float MaxLD = 0;
 int MaxLDt=0;               //max LD for the screen
 int LDcalPoint;             //where to draw the dot from the calibration

 //Saving of the MAx Values: (0=AGT, 1=Boost, 2=RPM)
 int MaxAGTValE[4];          //maximum AGT
 float MaxLDE[4]; 
 long MaxRPME[4];            //maximum RPM
 float MaxLMME[4];         //maximum LMM
 int MaxOilE[4];             //maximum Oilpressure
  
 int RefResh;

 //convertet values:

 float CalBoost = 0.0;
 float CalRAWBoost = 0.0;
 int CalLambda = 0;
 long CalRPM = 0;
 int CalThrottle = 0;
 float CalCaseTemp = 0.0;
 float CalLMM = 0.0;
 int VDOTemp1 = 0;
 int VDOTemp2 = 0;
 int VDOTemp3 = 0;
 int VDOPres1 = 0;
 int VDOPres2 = 0;
 int VDOPres3 = 0;
 float BatVolt = 0.0;
  
 
 //RPM Smoothing:
 int RPMreadings[RPMsmooth];                // the readings from the analog input
 int RPMindex = 0;                            // the index of the current reading
 int RPMtotal = 0;                            // the running total
 int RPMaverage = 0;                          // the average

 //Lookup Table for the TypK:
 //from 0-1350ï¿½C in steps of 50ï¿½C, the list is in ï¿½V according to that Temp.
 const unsigned int tempTypK[] PROGMEM = 
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
 const unsigned int tempVDOPressure[] PROGMEM = 
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
 const unsigned int tempVDOTemp[] PROGMEM = 
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

//The LCD Brightness, what could be cycled through
const unsigned int LCDBrightnessD[] PROGMEM = 
{ 
  50,125,255        //Change this for other settings depending on what you like
};


/*==============================================================================
 * SETUP()
 *============================================================================*/

void setup() { 
  
  pinMode(LCDBrightPin, OUTPUT);
  
  //set pin modes 
  pinMode(DATAOUT, OUTPUT); 
  pinMode(DATAIN, INPUT); 
  pinMode(SPICLOCK, OUTPUT);  
  digitalWrite(DATAOUT,LOW); 
  digitalWrite(SPICLOCK,LOW); 
   
  Serial.begin(57600);
  
  Wire.begin();                  //Start the Wire Libary for the PCF8574
  
  expanderWrite2(IOport2); //Switch off all Devices
  
  
  //Print the Info: 
  Serial.println(" ");
  Serial.println("MultiDisplay PRE!");  
 
  //Read the Values from the EEPROM back
  Screen = EEPROM.read(100);        //what screen was last shown?
  Brightness = EEPROM.read(105);    //The Brightness from the LCD
  LDcalPoint = EEPROM.read(205); 
  CalLD = EEPROMReadDouble(200)/1000.0;      //gets the float back (thats accurate enough)
  
  lcd.init();                              //Inits the LCD
  lcdShowIntro(InitTime);                      //Shows the Into 
  
  //RPM Smoothing init:
  for (int i = 0; i < RPMsmooth; i++)
  {
   RPMreadings[i] = 0;                      // initialize all the readings to 0
  }
  
  //TypK Array init:
  for (int i = 0; i < 7; i++)
  {
   CalAGT[i] = 0;                      // initialize all the readings to 0
  }
     
  //Load the right start Screen:
   switch(Screen){
   case 0:
      //Setup the Screen:
      break;
   case 1:
      Screen1Init();
      break;
   case 2:
      Screen2Init();
      break;
   case 3:
      Screen3Init();
      break;
   case 4:
      Screen4Init();
      break;
   case 5:
      Screen5Init();
      break;
   case 6:
      Screen6Init();
      break;
   case 7:
      Screen7Init();
      break;
   case 8:
      Screen8Init();
      break;
   case 9:
      Screen9Init();
      break;
   default:
      Screen1Init();
      break;
   }
   
   
   //Init the Buttons:
   expanderWrite(B10000011);        //This may needs to be modified when a third button is attached.

   
    
}

/*==============================================================================
 * LOOP()
 *============================================================================*/

void loop() {  
    
  //Current Time:
   time = millis();

  //Read in all Analog values:
  for(int I = 1; I <=16;I++){
    AnaIn[I] = read_adc(I);
  }

  //Calibrate them:
  if(DoCal == 1)
   {
    AnaConversion();
   }
   
  //Check for Limits:
  //if(DoCheck == 1)
  // {
    CheckLimits();
  // }

  //Print it:
  SerialPrint();
  
  //Depending on what screen, it will trigger different things:  
  switch(Screen){
   case 0:
      break;
   case 1:
      Screen1Work();
      break;
   case 2:
      Screen2Work();
      break;
   case 3:
      Screen3Work();
      break;
   case 4:
      Screen4Work();
      break;
   case 5:
      Screen5Work();
      break; 
   case 6:
      Screen6Work();
      break; 
   case 7:
      Screen7Work();
      break;       
   case 8:
      Screen8Work();
      break;  
   case 9:
      Screen9Work();
      break;  
   default:
      break;
  }  


  //My own Button Check Function

  ButtonCheck();
   
  //Saves the Screen when needed:
   
  if(millis()>= ScreenSave)
  {
    //and now save it:
    EEPROM.write(100,Screen);  
    //And also prevent a double save!
    ScreenSave = 4294000000;        // (thats close to 80days of runtime...) 
  }
   
  #if FreeMem
  //Free Mem Debug:
  Serial.print("FM ");
  Serial.println( freeMem());  
  #endif
  
  #if RPMDebug
  //Debug RPM generator:
  if(debug>=debugRPM)
  {
     debug = 0;
    digitalWrite(14,HIGH);
    delay(2);
    digitalWrite(14,LOW);
  }
  else
  {
    debug++;
  }
  #endif
    
}



