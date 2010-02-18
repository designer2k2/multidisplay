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


#include "UserInterface.h"
#include <wiring.h>
#include <stdlib.h>
#include "MultidisplayDefines.h"
#include "MultidisplayController.h"
#include <WProgram.h>
#include <EEPROM.h>

UserInterface::UserInterface()  {
        sdataP = NULL;
}

UserInterface::UserInterface(SensorData* p, MultidisplayController* c) {
        sdataP = p;

        buttonInt = 0;     //Button state
        buttonTime = 0;   //Button time
}


UserInterface::~UserInterface() {
        // TODO Auto-generated destructor stub
}



/*==============================================================================
 * BUTTON FUNCTIONS
 *============================================================================*/

void UserInterface::buttonAHold() {
	lcdController.toggleScreen();
	//Set the Timestamp for the Save:
	mController.setScreenSave ( millis() + SCREENSAVEDELAY );
}

//-------------------------------------------------------------------------------------------------------

void UserInterface::buttonAPressed() {

  //Serial.print(time*1);
  //Serial.print(";");
  //Serial.println("Button A Pressed");

  switch(lcdController.activeScreen){
   case 1:
      mController.ChangeSerOut();      //Switch from RAW to Cal to Nothing and vise versa.
      break;
   case 2:
       //Toggle A and B MCP
      lcdController.myScreens[1]->toggleScreenAB();
      break;
   case 3:
	   mController.ChangeSerOut();      //Switch from RAW to Cal to Nothing and vise versa.
      break;
   case 6:                 //Switches the 2 Row Screen
	   lcdController.myScreens[5]->toggleScreenAB();
      break;
   case 7:
	   lcdController.myScreens[6]->toggleRefreshCounter();
      break;
   default:
      break;
  }
}

//-------------------------------------------------------------------------------------------------------

void UserInterface::buttonBHold()
{

  //Serial.print(time*1);
  //Serial.print(";");
  //Serial.println("Button B Hold");

  switch(lcdController.activeScreen){
   case 0:
      break;
   case 1:
      //The Calibration from the LD will be done
      mController.CalibrateLD();
      break;
   default:
   break;
  }

}

//-------------------------------------------------------------------------------------------------------

void UserInterface::buttonBPressed() {

	//Serial.print(time*1);
	//Serial.print(";");
	//Serial.println("Button B Pressed");

	switch(lcdController.activeScreen){
	case 0:
		break;
	case 1:
		//The MaxLD will be reset!
		sdataP->maxLd = 0.0;
		sdataP->maxLdt = sdataP->ldCalPoint;
		break;
	case 2:
		//Change LCD brightness
		lcdController.toggleBrightness();

		//and save the new value:
		EEPROM.write(105,lcdController.brightness);
		break;
	case 7:
		//FIXME integrate into LCDSCreen6 its broken atm!
//		screen6DataSel++;      //Change the Scope Screen
//		if(screen6DataSel >= 5)
//			screen6DataSel = 1;
//
//		//Reset the Constrains:
//		//FIXME global ?!?
//		screen6Val1 = 0;
//		screen6Val2 = 5000;
		break;
	case 8:
		//FIXME integrate into LCDSCreen8 its broken atm!
//		screen8Val++;	//changes the MAX screen through all values
//		if(screen8Val >= 4)
//			screen8Val = 1;
		break;

	default:
		break;
	}
}

//-------------------------------------------------------------------------------------------------------

 void UserInterface::buttonCheck(int buttonState)  {
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
