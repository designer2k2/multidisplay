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
 * BUTTON FUNCTIONS                                                                
 *============================================================================*/
 
void ButtonAHold()
{
  
 //This will switch the Screen
   switch(Screen){
   case 0:
      Screen = 1;
      Screen1Init();
      break;
   case 1:
      Screen = 2;
      Screen2Init();
      break;
   case 2:
      Screen = 3;
      Screen3Init();
      break;
   case 3:
      Screen = 4;
      Screen4Init();
      break;
   case 4:
      Screen = 5;
      Screen5Init();
      break;
   case 5:
      Screen = 6;
      Screen6Init();
      break;
   case 6:
      Screen = 7;
      Screen7Init();
      break;      
  case 7:
      Screen = 8;
      Screen8Init();
      break;   
  case 8:
      Screen = 1;
      Screen1Init();
      break;       
   default:
      Screen = 1;
      break;
   }
   
  //Set the Timestamp for the Save:
  
  ScreenSave = millis() + ScreenSaveDelay;
   
}

//-------------------------------------------------------------------------------------------------------

void ButtonAPressed()
{
  
  //Serial.print(time*1);
  //Serial.print(";");
  //Serial.println("Button A Pressed");
  
  switch(Screen){
   case 0:
      break;
   case 1:
      ChangeSerOut();      //Switch from RAW to Cal to Nothing and vise versa.
      break;
   case 2:
       //Toggle A and B MCP
      if(val1==8)
      {
        Screen2Init();
      }
      else
      {
        Screen2InitB();
      }
      break; 
   case 3:
      ChangeSerOut();      //Switch from RAW to Cal to Nothing and vise versa.
      break;
   case 6:                 //Switches the 2 Row Screen
	  switch(val1){
	   case 1:
	     val1 = 2;
             Screen6InitB();
	     break;
	   case 2:
	     val1 = 1;
             Screen6Init();
	     break;
	  }
      break;
   case 7:
      RefResh = RefResh + 10;
      if(RefResh>=110)
      {
        RefResh = 0;
      }
      break;
   default:
      break;
  }
 
}

//-------------------------------------------------------------------------------------------------------

void ButtonBHold()
{
 
  //Serial.print(time*1);
  //Serial.print(";");
  //Serial.println("Button B Hold"); 
   
  switch(Screen){
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

void ButtonBPressed()
{
  
  //Serial.print(time*1);
  //Serial.print(";");
  //Serial.println("Button B Pressed");
 
   switch(Screen){
   case 0:
      break;
   case 1:
      //The MaxLD will be reset!
      MaxLD = 0.0;
      MaxLDt = LDcalPoint;
      break;
   case 2:
      //Change LCD Brightness
      Brightness++;
      if(Brightness>=3)
      {
        Brightness = 0;
      }
      LCDBrightness(Brightness);
      //and save the new value:
      EEPROM.write(105,Brightness); 
      break;
   case 7:
      val4++;      //Change the Scope Screen
      if(val4>=5)
      {
        val4 = 1;
      }
      //Reset the Constrains:
      val1 = 0;
      val2 = 5000;
      break;
   case 8:
      val1++;	//changes the MAX screen through all values
      if(val1>=4)
      {
       val1 = 1;
      }
      break;

   default:
   break;
  }
 
 
}

//-------------------------------------------------------------------------------------------------------

 void ButtonCheck()
 {
	 
	int ButtonState = 0;

	ButtonState = expanderRead();

	/*
	3 = open
	1 = s1
	2 = s2
	*/

        //compensate for the LED:
        ButtonState = ButtonState - 128;

	//now a little inteligent stuff:

	switch(ButtonState) {
	  case 1:
	   if(ButtonTime == 0)
	   {
		 ButtonAPressed();
		 ButtonTime = millis();
	   }
	   else
	   {
		 if(millis()>=ButtonTime+ButtonHold)
		 {
		   ButtonAHold();
		   ButtonTime = millis();
		 }
	   }
	   break;
	   
	  case 2:
	   if(ButtonTime == 0)
	   {
		 ButtonBPressed();
		 ButtonTime = millis();
	   }
	   else
	   {
		 if(millis()>=ButtonTime+ButtonHold)
		 {
		   ButtonBHold();
		   ButtonTime = millis();
		 }
	   }
	   break;
	   
	  case 3:
	   ButtonTime = 0;
	   break;
	  }

	 //Thats it... easy.

 }


