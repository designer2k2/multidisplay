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

/*==============================================================================
 * Other FUNCTIONS                                                                
 *============================================================================*/

//-------------------------------------------------------------------------------------------------------
//http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1239715016/3#3
void printfloat(float all , int decimals){ // will only work with lcd4bit library, basically it breaks the float into 2 integers then prints them seperatley with decimal point in the middle.
int decimal = 1 ;// intial multiplier so you dont get the situation decimal = 0 * 10 arrising
int dec2 = decimals;

while(decimals > 0) // calculate the multipler for moving the decimal along i.e. *10 *100 *1000 etc
   {
  decimal = decimal * 10 ;
  decimals = decimals - 1;
   }

int parta; // the first part of the float (numbers after the decimal point)
int partb; // the last part of the part  (numbers before the decimal point)

parta = int(all); // converting the float into an integer droping the info after the decimal
partb = (all * decimal) - (parta * decimal); // messing around to derive the integer form of the numbers before the decimal
itoa (parta, buf, 10);
lcd.printIn(buf);

lcd.printIn(".");

//if the partb has less then the decimals -1, make a additional 0
if(dec2 ==2)
{
if(partb<10)
{
  lcd.printIn("0");
}
}

itoa (partb, buf, 10);
lcd.printIn(buf);  
}

//-------------------------------------------------------------------------------------------------------
#if InitTime
void lcdShowIntro(int delayValue)
{
  
  //Must load this in 2 steps, somehow its not possible to load 8 chars at once...
  CGRAM_Intro();
  CGRAM_Intro2();

  //Show the Intro: 
  lcd.commandWrite(0x80+2);                  //Line1 
  lcd.print(1);
  lcd.print(2);
  lcd.commandWrite(0x80+10);                  //Line1 
  lcd.print(3);
  lcd.print(4);

  lcd.commandWrite(0xC0+2); 			//Line2
  lcd.print(5);
  lcd.print(6);
  lcd.printIn("ulti");
  lcd.commandWrite(0xC0+10); 			//Line2
  lcd.print(7);
  lcd.print(8);
  lcd.printIn("isplay");
  
  lcd.commandWrite(0xD4+1);                  //Line=4
  lcd.printIn("www.designer2k2.at");
  
  //Set the LCD Brightness: (This turns on the Light, looks nice)
  LCDBrightness(Brightness);
  
  //Flash the Shiftlight:
  
  analogWrite(RPMShiftlightPin,RPMHighBright);
  delay(50);
  analogWrite(RPMShiftlightPin,RPMLowBright);
  delay(50);
  analogWrite(RPMShiftlightPin,RPMNoBright);
  
  
  delay(delayValue);                      //Short delay, for the Intro screen :)
 
}
#endif
//-------------------------------------------------------------------------------------------------------

//http://www.arduino.cc/playground/Code/MCP3208
int read_adc(int channel){
  int adcvalue = 0;
  byte commandbits = B11000000; //command bits - start, mode, chn (3), dont care (3) 
  int Chan = channel;  //Save the channel, to make it possible to deselect the MCP later.
  
  //choose the right MCP3208, depending on number to read.
  if(channel>8)
  {
    //Channels 9-16 on second MCP3208
    IOport2 = IOport2 & B01111101;     //Clears the Selection Pin for MCP 2 (Bitwise AND) (right is the MCP, left the LED)
    //to adapt the right Channel selection
    channel = channel - 8;
  }
  else
  {
    //Channels 1-8 on first MCP3208
    IOport2 = IOport2 & B01111110;     //Clears the Selection Pin for MCP 1 (Bitwise AND) http://www.programmers-corner.com/article/27
  }

  //allow channel selection
  commandbits|=((channel-1)<<3);
  
  expanderWrite2(IOport2); //Select ADC
  
  // setup bits to be written
  for (int i=7; i>=3; i--){
    digitalWriteF(DATAOUT,commandbits&1<<i);
    //cycle clock
    digitalWriteF(SPICLOCK,HIGH);
    digitalWriteF(SPICLOCK,LOW);   
  }

  digitalWriteF(SPICLOCK,HIGH);    //ignores 2 null bits
  digitalWriteF(SPICLOCK,LOW);
  digitalWriteF(SPICLOCK,HIGH); 
  digitalWriteF(SPICLOCK,LOW);

  //read bits from adc
  for (int i=11; i>=0; i--){
    adcvalue+=digitalReadF(DATAIN)<<i;
    //cycle clock
    digitalWriteF(SPICLOCK,HIGH);
    digitalWriteF(SPICLOCK,LOW);
  }
  
    //Deselct the MCP
  if(Chan>8)
  {
    //Channels 9-16 on second MCP3208
    IOport2 = IOport2 | B10000010;     //Sets the Selection Pin for MCP 2    
  }
  else
  {
    //Channels 1-8 on first MCP3208
    IOport2 = IOport2 | B10000001;     //Sets the Selection Pin for MCP 1  
  }
  
  expanderWrite2(IOport2); //Deselect ADC
  
  return adcvalue;
}

//-------------------------------------------------------------------------------------------------------


//http://www.openobject.org/opensourceurbanism/Storing_Data
void EEPROMWriteDouble(int p_address, long p_value)
{
  byte Byte1 = ((p_value >> 0) & 0xFF);
  byte Byte2 = ((p_value >> 8) & 0xFF);
  byte Byte3 = ((p_value >> 16) & 0xFF);
  byte Byte4 = ((p_value >> 24) & 0xFF);

  EEPROM.write(p_address, Byte1);
  EEPROM.write(p_address + 1, Byte2);
  EEPROM.write(p_address + 2, Byte3);
  EEPROM.write(p_address + 3, Byte4);
}
long EEPROMReadDouble(int p_address)
{
  byte Byte1 = EEPROM.read(p_address);
  byte Byte2 = EEPROM.read(p_address + 1);
  byte Byte3 = EEPROM.read(p_address + 2);
  byte Byte4 = EEPROM.read(p_address + 3);

  long firstTwoBytes = ((Byte1 << 0) & 0xFF) + ((Byte2 << 8) & 0xFF00);
  long secondTwoBytes = (((Byte3 << 0) & 0xFF) + ((Byte4 << 8) & 0xFF00));
  secondTwoBytes *= 65536; // multiply by 2 to power 16 - bit shift 24 to the left

  return (firstTwoBytes + secondTwoBytes);
}

//-------------------------------------------------------------------------------------------------------

// Upload a user-defined character (5x8)
// See http://www.quinapalus.com/hd44780udg.html for
// character generator.
// http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1231533425/1#1
void LcdUploadUdef5x8(int character, int *data)
{
  int c = character << 3, i;
  for (i = 0; i < 8; i++)
  {
    lcd.commandWrite(0x40 | c | i);
    lcd.print(data[i]);
  }
}
//-------------------------------------------------------------------------------------------------------

void CGRAM_Dot()
{
   
  int data1[] = {31,0,0,0,0,0,0,31};        //only 2 dots
  int data2[] = {31,0,0,14,0,0,0,31};       //the 2 dots with a minus
  int data3[] = {31,0,4,14,4,0,0,31};       //the 2 dots with a plus
  int data4[] = {31,14,31,31,31,31,14,31};  //thats a block with little rounded edges
  
  LcdUploadUdef5x8(5,data1);
  LcdUploadUdef5x8(6,data2);
  LcdUploadUdef5x8(7,data3);
  LcdUploadUdef5x8(8,data4);
}  
//-------------------------------------------------------------------------------------------------------
  
void CGRAM_Bar()
{
  int data1[] = {16,16,16,16,16,16,16,16};  //1 line
  int data2[] = {24,24,24,24,24,24,24,24};  //2 line
  int data3[] = {28,28,28,28,28,28,28,28};  //3 line
  int data4[] = {30,30,30,30,30,30,30,30};  //4 line
 
  LcdUploadUdef5x8(1,data1);
  LcdUploadUdef5x8(2,data2);
  LcdUploadUdef5x8(3,data3);
  LcdUploadUdef5x8(4,data4);
}
//-------------------------------------------------------------------------------------------------------

void CGRAM_Intro()
{
  int data1[] = {28,28,28,30,30,31,31,31};  // 1--- k
  int data2[] = {7,7,7,15,15,31,31,31};     // -1-- k
  int data3[] = {31,31,31,28,28,28,28,28};  // --1- k
  int data4[] = {24,28,30,15,7,7,7,7};      // ---1 k

  LcdUploadUdef5x8(1,data1);
  LcdUploadUdef5x8(2,data2);
  LcdUploadUdef5x8(3,data3);
  LcdUploadUdef5x8(4,data4); 
}
//-------------------------------------------------------------------------------------------------------

void CGRAM_Intro2()
{
  int data1[] = {31,29,29,28,28,28,28,28};  // 2--- k
  int data2[] = {31,23,23,7,7,7,7,7};       // -2-- k
  int data3[] = {28,28,28,28,28,31,31,31};  // --2- k
  int data4[] = {7,7,7,7,15,30,28,24};      // ---2 k
  
  LcdUploadUdef5x8(5,data1);
  LcdUploadUdef5x8(6,data2);
  LcdUploadUdef5x8(7,data3);
  LcdUploadUdef5x8(8,data4);
}
//-------------------------------------------------------------------------------------------------------

void DrawBar(int Digits, int Value)
{
   
  int Curs = Value/10;
  for (int i=0; i <= Digits-1; i++){
    if(i<Curs)
      {
        lcd.print(255);              
      }
    else
      {
        
        if(i==Curs)
        {
         //in val ist 84 und in val5 die 8
         //ich brauch die 4 /2 als int
         int ChrW = (Value - Curs*10)/2;  
         
         if(ChrW==0)
          {
            lcd.printIn(" "); 
          } 
          else
         {        
           lcd.print(ChrW); 
         }
        }
        else
          {       
            lcd.printIn(" ");
          }
      } 
  }

}

//-------------------------------------------------------------------------------------------------------

void AnaConversion()
{
  //This converts all 16 Analog Values into the real deal :)
 
  //Boost:
  CalRAWBoost = 5.0*AnaIn[BoostPin]/4096.0;             //only gets 0-5V
  CalRAWBoost = (CalRAWBoost * 50 - 10)/100;     	//makes 0-250kPa out of it
  CalBoost = CalRAWBoost - CalLD;			//apply the offset (ambient pressure)
  
 //Check if the Boost is a new Max Boost Event
 if(CalBoost>=MaxLDE[1])
 {
	SaveMax(1);
 }
   
  //Lambda:
  CalLambda = map(AnaIn[LambdaPin], LambdaMin, LambdaMax, 0, 200);    //gets about the 0-1V into 0-200 values
  CalLambda = constrain(CalLambda, 0, 200);

  //CaseTemp: (damped)
  CalCaseTemp = CalCaseTemp*0.9 + (500.0*AnaIn[CaseTempPin]/4096.0)*0.1;  //thats how to get °C out from a LM35 with 12Bit ADW

  //Throttle:
  CalThrottle = map(AnaIn[ThrottlePin], ThrottleMin, ThrottleMax, 0, 100); 
  CalThrottle = constrain(CalThrottle, 0, 100);
  CalThrottle = 100 - CalThrottle;                 //VR6 has 5V for closed and 0V for open throttle...

  //LMM:
  CalLMM = 5.0*AnaIn[LMMPin]/4096.0;		   //makes 0.0 to 5.0 Volt out of it, with VagCom this could be maped to gr Air i think

  //RPM
  //(from the smoothing example)
  RPMtotal -= RPMreadings[RPMindex];               // subtract the last reading
  RPMreadings[RPMindex] = AnaIn[RPMPin];           // read from the sensor
  RPMtotal += RPMreadings[RPMindex];               // add the reading to the total
  RPMindex = (RPMindex + 1);                       // advance to the next index
  
  if (RPMindex >= RPMsmooth)                       // if we're at the end of the array...
  {
    RPMindex = 0;                                  // ...wrap around to the beginning
  }
  
  RPMaverage = RPMtotal / RPMsmooth;               // calculate the average
  CalRPM = RPMaverage*RPMfactor;                   // apply the factor for calibration
  
 //Check if the RPM is a new Max RPM Event
 if(CalRPM>=MaxRPME[2])
 {
   SaveMax(2);
 }
  
   
   #if RPMShiftLight
   Shiftlight();
   #endif //RPMShiftLight
  
  //Battery Voltage: (Directly from the Arduino!, so only 1024, not 4096.)
  //measured Voltage * 4,09 + 0,7V should give the supply voltage
  BatVolt = ((5.0*analogRead(BatteryPin)/1023.0)*4.09)+0.7;
  
  //Lets do the Typ K Conversion:
  if(DoTypK==1)
  {
   FetchTypK();
  }
  
  //VDO Stuff:
  
  VDOTemp1 = GetVDOTemp(AnaIn[VDOT1Pin]);
  VDOTemp2 = GetVDOTemp(AnaIn[VDOT2Pin]);
  VDOTemp3 = GetVDOTemp(AnaIn[VDOT3Pin]);
  VDOPres1 = GetVDOPressure(AnaIn[VDOP1Pin]);
  VDOPres2 = GetVDOPressure(AnaIn[VDOP2Pin]);
  VDOPres3 = GetVDOPressure(AnaIn[VDOP3Pin]);
  
}
//-------------------------------------------------------------------------------------------------------
#if RPMShiftLight
void Shiftlight()
{
  
  //This sets the brightness according to the RPM
  
  if(CalRPM >= RPMShiftLow && CalRPM <= RPMShiftHigh)
  {
    analogWrite(RPMShiftlightPin,RPMLowBright);
  }
  else if(CalRPM>= RPMShiftHigh)
  {
    analogWrite(RPMShiftlightPin,RPMHighBright);
  }
  else if(CalRPM< RPMShiftLow)
  {
    analogWrite(RPMShiftlightPin,RPMNoBright);
  }
  
}
#endif //RPMShiftLight
//-------------------------------------------------------------------------------------------------------

void SerialPrint()
{  
   switch(SerOut){
    case 0:
     break;
    case 1:
     //RAW Output:
     Serial.print(time);
     Serial.print(";");
     for(int I = 1; I <=15;I++){
        Serial.print(AnaIn[I]);
        Serial.print(";");
     }
     Serial.println(AnaIn[16]);  
     break;
    case 2:
     //Convertet Output:
     Serial.print(time);
     Serial.print(";");
     Serial.print(CalRPM);
     Serial.print(";");
     Serial.print(CalBoost);
     Serial.print(";");
     Serial.print(CalThrottle);
     Serial.print(";");
     Serial.print(CalLambda);
     Serial.print(";");
     Serial.print(CalLMM);
     Serial.print(";");
     Serial.print(CalCaseTemp);
     Serial.print(";");
     Serial.print(CalAGT[0]);
     Serial.print(";");
     Serial.print(CalAGT[1]);
     Serial.print(";");
     Serial.println(BatVolt);
     break;
    default:
     SerOut = 0;
     break; 
  }

}
//-------------------------------------------------------------------------------------------------------

void HeaderPrint()
{

 switch(SerOut){
  case 0:
   //This prints Debugging stuff:
   /*
   Serial.println(" ");
   Serial.print("CalBoost: ");
   Serial.println(CalLD);
   */
   break;
  case 1:
   Serial.println(" ");
   Serial.println("Time;AnaIn0;AnaIn1;AnaIn2;AnaIn3;AnaIn4;AnaIn5;AnaIn6;AnaIn7");
   break;
  case 2:
   Serial.println(" ");
   Serial.println("Time;RPM;Boost;Throttle;Lambda;LMM;CaseTemp;TypK1;TypK2;Battery");
   break;
  default:
   break;  
 }

}
//-------------------------------------------------------------------------------------------------------

void ChangeSerOut()
{
//Switch from RAW to Cal and vise versa.
      switch(SerOut){
        case 0:
         SerOut = 1;
         break;
        case 1:
         SerOut = 2;
         break;
        case 2:
         SerOut = 0;
         break;
        default:
         SerOut = 0;
         break;
        }
       HeaderPrint();
}
//-------------------------------------------------------------------------------------------------------

void CalibrateLD()
{
      CalLD = CalRAWBoost;
      LDcalPoint = val3/10;
      //and saved:
      EEPROM.write(205,LDcalPoint);
      EEPROMWriteDouble(200,CalLD*1000);    //writes the float as long, will do it.

      //The MaxLD will be reset!
      MaxLD = 0.0;
      MaxLDt = LDcalPoint;
      
      Serial.println(" ");
      Serial.print("CalBoost: ");
      Serial.println(CalLD);
}

//-------------------------------------------------------------------------------------------------------

//This converts the thermocouple µV reading into some usable °C
int GetTypKTemp(unsigned int microVolts)
{
  int LookedupValue;
  //This searches the 2 surrounding values, and then linear interpolates between them.
  for(int i = 0; i<tempTypKReadings;i++)
  {
   if(microVolts >= pgm_read_word(&tempTypK[i]) && microVolts <= pgm_read_word(&tempTypK[i+1]))
   {
    LookedupValue = ((i)*50) + ((50L *(microVolts - pgm_read_word(&tempTypK[i]))) / ((pgm_read_word(&tempTypK[i+1]) - pgm_read_word(&tempTypK[i]))));
    break;
   }
  }
   
  return LookedupValue;
 }

//Converts the ADW Reading into °C
int GetVDOTemp(int ADWreading)
{
  int LookedupValue;
  //This searches the 2 surrounding values, and then linear interpolates between them.
  for(int i = 0; i<22;i++)
  {
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
int GetVDOPressure(int ADWreading)
{
  int LookedupValue;
  //This searches the 2 surrounding values, and then linear interpolates between them.
  for(int i = 0; i<12;i++)
  {
   if(ADWreading >= pgm_read_word(&tempVDOPressure[i]) && ADWreading <= pgm_read_word(&tempVDOPressure[i+1]))
   {
    LookedupValue = ((i)*10) + ((10L *(ADWreading - pgm_read_word(&tempVDOPressure[i]))) / ((pgm_read_word(&tempVDOPressure[i+1]) - pgm_read_word(&tempVDOPressure[i]))));
    break;
   }
  }  
  
   LookedupValue -= 10;   //there is an offset of 10 due to my lazy coding

   LookedupValue = constrain(LookedupValue,0,999);    //Limits the Output to 99.9Bars, so an open circuit gets detectet!
 
   return LookedupValue;
}

//-------------------------------------------------------------------------------------------------------


void FetchTypK()
 {
 //This will read in all the needed Analog values, convert them to °C, and make the calibration with the LM35
 //so it must be called after the AnaConversion! 

//http://www.arduino.cc/playground/Learning/4051
int r0;      //value select pin at the 4051 (s0)
int row;     // storeing the bin code
int  bin [] = {000, 1, 10, 11, 100, 101, 110, 111};//bin = binary, some times it is so easy

 for (int i=0; i <= NumTypK; i++)
 { 

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

   Temp = ((5.0*read_adc(AGTPin))/4096.0)*10000;   //gets the Volts and makes µV out of it (100 is already added from the Amp)
   Temp = GetTypKTemp(Temp);                       //Converts the µV into °C 
   
 //Check if it is open:
 
   if(Temp>=MaxTypK)
    {
     Temp = 0; 
    }
   else
    { 
     Temp += int(CalCaseTemp);                       //apply the Correction
    }
  
   CalAGT[i] = Temp;              //Save it into the array
 
    //Check if the Temp is a new Max Temp Event
    if(Temp>=MaxAGTValE[3])
    {
      SaveMax(3);
    }
 
 //repeat for all NumTypK+1 channels.
 }

}
 
//-------------------------------------------------------------------------------------------------------

 
void CGRAM_VertBar()
{
   
  int data1[] = {0,0,0,0,0,0,0,31};              //1 row
  int data2[] = {0,0,0,0,0,0,31,31};             //2 rows
  int data3[] = {0,0,0,0,0,31,31,31};            //3 rows
  int data4[] = {0,0,0,0,31,31,31,31};           //4 rows
  int data5[] = {0,0,0,31,31,31,31,31};          //5 rows
  int data6[] = {0,0,31,31,31,31,31,31};         //6 rows
  int data7[] = {0,31,31,31,31,31,31,31};        //7 rows
  
  LcdUploadUdef5x8(1,data1);
  LcdUploadUdef5x8(2,data2);
  LcdUploadUdef5x8(3,data3);
  LcdUploadUdef5x8(4,data4);
  LcdUploadUdef5x8(5,data5);
  LcdUploadUdef5x8(6,data6);
  LcdUploadUdef5x8(7,data7);
  
} 
  //-------------------------------------------------------------------------------------------------------

void DrawVertBar(int Val, int Pos)
{
//Val must be between 0 and 32 (4x8)
//Pos must be between 0 and 20 (on a 20 char disp)

//whats the modulo with 8 ?
int Mod;
Mod = Val % 8;
//This is directly the code for the top character!
//But at what pos it must be printed?

int PrintPos;
PrintPos = (Val - Mod) / 8;

//But if Mod = 0, the LCD nothing is 254.

if(Mod == 0)
 Mod=254;

//now lets make a switch case depending on the PrintPos.

switch(PrintPos){
  case 0:
    //so there is only one character /3 blanks and the character in the lowest position
    lcd.commandWrite(0x80+Pos);              //Line 1 
    lcd.print(254); //Blank
    lcd.commandWrite(0xC0+Pos);              //Line 2
    lcd.print(254); //Blank
    lcd.commandWrite(0x94+Pos);              //Line 3
    lcd.print(254); //Blank
    lcd.commandWrite(0xD4+Pos);              //Line 4
    lcd.print(Mod); //The Custom Character presenting the value
    break;
  case 1:
    //so there is only one character /1 fulls and the character in the 2nd position
    lcd.commandWrite(0x80+Pos);              //Line 1 
    lcd.print(254); //Blank
    lcd.commandWrite(0xC0+Pos);              //Line 2
    lcd.print(254); //Blank
    lcd.commandWrite(0x94+Pos);              //Line 3
    lcd.print(Mod); //The Custom Character presenting the value
    lcd.commandWrite(0xD4+Pos);              //Line 4
    lcd.print(255); //Full
    break;
  case 2:
    //so there is only one character /2 fulls and the character in the 3rd position
    lcd.commandWrite(0x80+Pos);              //Line 1 
    lcd.print(254); //Blank
    lcd.commandWrite(0xC0+Pos);              //Line 2
    lcd.print(Mod); //The Custom Character presenting the value
    lcd.commandWrite(0x94+Pos);              //Line 3
    lcd.print(255); //Full
    lcd.commandWrite(0xD4+Pos);              //Line 4
    lcd.print(255); //Full
    break;
  case 3:
    //so there is only one character /3 fulls and the character in the highest position
    lcd.commandWrite(0x80+Pos);              //Line 1 
    lcd.print(Mod); //The Custom Character presenting the value
    lcd.commandWrite(0xC0+Pos);              //Line 2
    lcd.print(255); //Full
    lcd.commandWrite(0x94+Pos);              //Line 3
    lcd.print(255); //Full
    lcd.commandWrite(0xD4+Pos);              //Line 4
    lcd.print(255); //Full
    break;
  case 4:
    //easy, eveything is full!
    lcd.commandWrite(0x80+Pos);              //Line 1 
    lcd.print(255); //Full
    lcd.commandWrite(0xC0+Pos);              //Line 2
    lcd.print(255); //Full
    lcd.commandWrite(0x94+Pos);              //Line 3
    lcd.print(255); //Full
    lcd.commandWrite(0xD4+Pos);              //Line 4
    lcd.print(255); //Full
    break;
}

 //thats it, easy :) 
    
}

 //-------------------------------------------------------------------------------------------------------

void ScopeMode()
{
  
   //but work the screen from left side
  for(int i=0; i<=18; i++){
    
   //Print the Buffer    
   DrawVertBar(ScopeInt[i],i);
    
   //shift Data to the left    
   ScopeInt[i] = ScopeInt[i+1];
    
  } 
  
}

#if FreeMem
//Free Mem: http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1246358387
#include <stdlib.h>
#include <inttypes.h>
#include <avr/io.h>

extern char * const __brkval;

uint16_t freeMem(void)
{
  char *brkval;
  char *cp;

  brkval = __brkval;
  if (brkval == 0) {
    brkval = __malloc_heap_start;
  }
  cp = __malloc_heap_end;
  if (cp == 0) {
    cp = ((char *)AVR_STACK_POINTER_REG) - __malloc_margin;
  }
  if (cp <= brkval) return 0;

  return cp - brkval;
}
#endif
  
//----------------------------------------------------------------------------------------------------
//This checks certain values if they exceed limits or not, if so an alarm is triggered
void CheckLimits()
{

  int FlashTrigger=0;
  
  /*
  if(VDOPres1>MaxOP)
  {
    FlashTrigger = 1;    //Enable the LCDFlash
  }
  */  
  
  if(CalAGT[0]>MaxAGT)
  {
    FlashTrigger = 1;    //Enable the LCDFlash
  }
  
  
  if(CalAGT[1]>MaxAGT)
  {
    FlashTrigger = 1;    //Enable the LCDFlash
  }

   
  //Set The new Flash Timeout 
  if(FlashTrigger>0)
  {
    FlashETimeU = millis() + FlashETime;
    FlashTrigger = 0;
  }

  
  //And Flash!      
  if(FlashETimeU>=millis())                     //Check if the Flash should be shown
    {
      if(FlashTimeU<=millis())                  //Check if the Flash is about to be changed
      {
        //Now lets change the Brightness
        if(Brightness>0)
        {
          Brightness = 0;
        }
        else
        {
          Brightness = 2;
        }
               
        FlashTimeU = millis() + FlashTime;      //And save the Next Changetime
        LCDBrightness(Brightness);              //And set the Brightness
      }     
    }
    else
    {
       Brightness = EEPROM.read(105);           //Set back the Brightness
       LCDBrightness(Brightness);              //And set the Brightness
    }
     
}

//----------------------------------------------------------------------------------------------------
//http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1173047782/5
void expanderWrite(byte _data) {
  Wire.beginTransmission(expander);
  Wire.send(_data);
  Wire.endTransmission();
}

byte expanderRead() {
  byte _data;
  Wire.requestFrom(expander, 1);
  if(Wire.available()) {
    _data = Wire.receive();
  }
  return _data;
}

void expanderWrite2(byte _data) {
  Wire.beginTransmission(expander2);
  Wire.send(_data);
  Wire.endTransmission();
}

byte expanderRead2() {
  byte _data;
  Wire.requestFrom(expander2, 1);
  if(Wire.available()) {
    _data = Wire.receive();
  }
  return _data;
}

//----------------------------------------------------------------------------------------------------
//Saves all Data to the arrays
void SaveMax(int Num)
{
   MaxAGTValE[Num] = CalAGT[0];  
   MaxLDE[Num] = CalBoost; 
   MaxRPME[Num] = CalRPM; 
   MaxLMME[Num] = CalLMM;
   MaxOilE[Num] = VDOPres1;
}
  
void LCDBrightness(int Bright)
{
analogWrite(LCDBrightPin,pgm_read_word(&LCDBrightnessD[Bright]));
}
  

