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

char UserInterface::bn12[]={255,1,255,0, 1,255,254,0, 1,3,255,0, 1,3,255,0, 255,2,2,0, 255,3,1,0, 255,3,1,0, 1,1,255,0, 255,3,255,0, 255,3,255,0};
char UserInterface::bn22[]={255,2,255,0, 2,255,2,0,   255,2,2,0, 2,2,255,0, 254,255,254,0, 2,2,255,0,   255,2,255,0, 254,255,254,0, 255,2,255,0, 254,254,255,0};
// Array index into parts of big numbers. Numbers consist of 12 custom characters in 4 lines
//              0               1            2              3                4             5             6              7              8              9
char UserInterface::bn14[]={1,2,3,0,       2,3,254,0,     1,2,3,0,       1,2,3,0,       2,254,254,0,   2,2,2,0,       1,2,3,0,       2,2,2,0,       1,2,3,0,       1,2,3,0};
char UserInterface::bn24[]={255,254,255,0, 254,255,254,0, 1,2,255,0,     254,2,255,0,   255,2,2,0,     255,2,2,0,     255,2,3,0,     254,2,255,0,   255,2,255,0,   255,254,255,0};
char UserInterface::bn34[]={255,254,255,0, 254,255,254,0, 255,254,254,0, 254,254,255,0, 254,255,254,0, 254,254,255,0, 255,254,255,0, 254,255,254,0, 255,254,255,0, 4,6,255,0};
char UserInterface::bn44[]={4,6,5,0,       6,6,6,0,       4,6,6,0,       4,6,5,0,       254,6,254,0,   6,6,5,0,       4,6,5,0,       254,6,254,0,   4,6,5,0,       254,254,6,0};

//pgm_uint16_t UserInterface::test123[] PROGMEM={1,2,3,4,5};

//The LCD brightness, what could be cycled through
const unsigned int UserInterface::LCDBrightnessD[]= {
   50,125,255        //Change this for other settings depending on what you like
 };



UserInterface::UserInterface()  {
	sdataP = NULL;
	controllerP = NULL;
}

UserInterface::UserInterface(SensorData* p, MultidisplayController* c) {
	sdataP = p;
	controllerP=c;
	lcdp = new LCD4Bit(4);
	lcdp->init();

	activeScreen = 1;
	buttonInt = 0;     //Button state
	buttonTime = 0;   //Button time
	refresh = 0;
	for ( int i = 0 ; i < 20 ; i++ )
		scopeInt[i]=0;
}


UserInterface::~UserInterface() {
	// TODO Auto-generated destructor stub
}



//Bigfont Code
//
//BIG parts from http://opengauge.googlecode.com/svn/trunk/mpguino/mpguino.pde

void UserInterface::cgramBigFont2()
{
	//255 is the "Full Block" Code, and 254 is blank
	int data1[] = {31,31,31,0,0,0,0,0};    //Small Block on top
	int data2[] = {0,0,0,0,0,31,31,31};    //Small Block on bottom
	int data3[] = {31,31,31,0,0,0,31,31};  //Small Block on top and bottom
	// int data4[] = {28,30,31,31,31,31,30,28};  //Full Block roundet right
	// int data5[] = {7,15,31,31,31,31,15,7};  //Full Block roundet left
	int data6[] = {0,0,0,14,14,14,12,8};  //Dot, for Decimal.

	lcdUploadUdef5x8(1,data1);
	lcdUploadUdef5x8(2,data2);
	lcdUploadUdef5x8(3,data3);
	// LcdUploadUdef5x8(4,data4);
	// LcdUploadUdef5x8(5,data5);
	lcdUploadUdef5x8(6,data6);
}
//-------------------------------------------------------------------------------------------------------

void UserInterface::cgramBigFont4()
{

	//255 is the "Full Block" Code, and 254 is blank
	int data1[] = {0,0,0,0,3,15,15,31};    //Small Block on bottom left
	int data2[] = {0,0,0,0,31,31,31,31};    //Small Block on bottom right
	int data3[] = {0,0,0,0,24,30,30,31};  //Small Block on bottom full
	int data4[] = {31,15,15,3,0,0,0,0};  //Small Block on top left
	int data5[] = {31,30,30,24,0,0,0,0};  //Small Block on top right
	int data6[] = {31,31,31,31,0,0,0,0};  //Small Block on top full
	int data7[] = {14,14,14,14,12,8,0,0};  //Dot, for Decimal.

	lcdUploadUdef5x8(1,data1);
	lcdUploadUdef5x8(2,data2);
	lcdUploadUdef5x8(3,data3);
	lcdUploadUdef5x8(4,data4);
	lcdUploadUdef5x8(5,data5);
	lcdUploadUdef5x8(6,data6);
	lcdUploadUdef5x8(7,data7);

}
//-------------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------------

void UserInterface::printOneNumber2(uint8_t digit, uint8_t leftAdjust, int LineOffset)
{
	// LineOffset = 0, means 1 Line, LineOffset = 20 means 2 Line.
	// leftAdjust = 0, means 1 Pos, leftAdjust = 1 is second Pos.

	// Line 1 of the one digit number
	lcdp->commandWrite(0x80+LineOffset+leftAdjust*3+1*leftAdjust);                  //Line1

	lcdp->print(bn12[digit*3+digit*1]);
	lcdp->print(bn12[digit*3+1+digit*1]);
	lcdp->print(bn12[digit*3+2+digit*1]);

	// Line 2 of the one-digit number
	lcdp->commandWrite(0xC0+LineOffset+leftAdjust*3+1*leftAdjust);              // Line 2
	lcdp->print(bn22[digit*3+digit*1]);
	lcdp->print(bn22[digit*3+1+digit*1]);
	lcdp->print(bn22[digit*3+2+digit*1]);

}

//-------------------------------------------------------------------------------------------------------

void UserInterface::printOneNumber4(uint8_t digit, uint8_t leftAdjust)
{
	// leftAdjust = 0, means 1 Pos, leftAdjust = 1 is second Pos.

	// Line 1 of the one digit number
	lcdp->commandWrite(0x80+leftAdjust*3+1*leftAdjust);                  //Line 1
	lcdp->print(bn14[digit*3]);
	lcdp->print(bn14[digit*3+1]);
	lcdp->print(bn14[digit*3+2]);

	// Line 2 of the one-digit number
	lcdp->commandWrite(0xC0+leftAdjust*3+1*leftAdjust);              // Line 2
	lcdp->print(bn24[digit*3]);
	lcdp->print(bn24[digit*3+1]);
	lcdp->print(bn24[digit*3+2]);

	// Line 3 of the one digit number
	lcdp->commandWrite(0x94+leftAdjust*3+1*leftAdjust);                  //Line 3
	lcdp->print(bn34[digit*3]);
	lcdp->print(bn34[digit*3+1]);
	lcdp->print(bn34[digit*3+2]);

	// Line 4 of the one-digit number
	lcdp->commandWrite(0xD4+leftAdjust*3+1*leftAdjust);              // Line 4
	lcdp->print(bn44[digit*3]);
	lcdp->print(bn44[digit*3+1]);
	lcdp->print(bn44[digit*3+2]);

}

//-------------------------------------------------------------------------------------------------------

void UserInterface::bigNum (unsigned long t, int LineOffset, uint8_t leftAdjust, int d){

	// LineOffset = 0, means 1 Line, LineOffset = 20 means 2 Line.
	// d = 0, means no dp!

	//  unsigned long t = 98550ul;//number in thousandths
	//  unsigned long t = 9855ul;//number in thousandths
	char  dp = 254;
	char  dp2 = 254;


	char* r = "009.99"; //default to 999
	if(t<=99500){
		r=format(t/10); //0098.6
		dp=6;
	}else if(t<=999500){
		r=format(t/100);
	}
	if(t<=9950) {
		dp = 254;
		dp2 = 6;
		r=format(t); //009.86
	}

	if(d==0) {
		dp = 254;
		dp2 = 254;
	}

	lcdp->commandWrite(0x80+LineOffset+leftAdjust);
	lcdp->printIn(bn12+(r[2]-'0')*4);
	lcdp->printIn(" ");
	lcdp->printIn(bn12+(r[4]-'0')*4);
	lcdp->printIn(" ");
	lcdp->printIn(bn12+(r[5]-'0')*4);

	lcdp->commandWrite(0xC0+LineOffset+leftAdjust);
	lcdp->printIn(bn22+(r[2]-'0')*4);
	lcdp->print(dp2);
	lcdp->printIn(bn22+(r[4]-'0')*4);
	lcdp->print(dp);
	lcdp->printIn(bn22+(r[5]-'0')*4);

}

//-------------------------------------------------------------------------------------------------------

void UserInterface::bigNum4 (unsigned long t, uint8_t leftAdjust, int d){

	// LineOffset = 0, means 1 Line, LineOffset = 20 means 2 Line.
	// d = 0, means no dp!

	//  unsigned long t = 98550ul;//number in thousandths
	//  unsigned long t = 9855ul;//number in thousandths
	char  dp = 254;
	char  dp2 = 254;


	char * r = "009.99"; //default to 999
	if(t<=99500){
		r=format(t/10); //0098.6
		dp=7;
	}else if(t<=999500){
		r=format(t/100);
	}
	if(t<=9950)
	{
		dp = 254;
		dp2 = 7;
		r=format(t); //009.86
	}

	if(d==0)
	{
		dp = 254;
		dp2 = 254;
	}

	lcdp->commandWrite(0x80+leftAdjust);
	lcdp->printIn(bn14+(r[2]-'0')*4);
	lcdp->printIn(" ");
	lcdp->printIn(bn14+(r[4]-'0')*4);
	lcdp->printIn(" ");
	lcdp->printIn(bn14+(r[5]-'0')*4);

	lcdp->commandWrite(0xC0+leftAdjust);
	lcdp->printIn(bn24+(r[2]-'0')*4);
	lcdp->printIn(" ");
	lcdp->printIn(bn24+(r[4]-'0')*4);
	lcdp->printIn(" ");
	lcdp->printIn(bn24+(r[5]-'0')*4);

	lcdp->commandWrite(0x94+leftAdjust);
	lcdp->printIn(bn34+(r[2]-'0')*4);
	lcdp->printIn(" ");
	lcdp->printIn(bn34+(r[4]-'0')*4);
	lcdp->printIn(" ");
	lcdp->printIn(bn34+(r[5]-'0')*4);

	lcdp->commandWrite(0xD4+leftAdjust);
	lcdp->printIn(bn44+(r[2]-'0')*4);
	lcdp->print(dp2);
	lcdp->printIn(bn44+(r[4]-'0')*4);
	lcdp->print(dp);
	lcdp->printIn(bn44+(r[5]-'0')*4);

}



//-------------------------------------------------------------------------------------------------------
//Displays a 4 Digit Number (RPM, Thermocouple Temp) over 2 Lines
//only Full Digits are shown!
//no leading 0 or commas.
//it clears all in front

void UserInterface::bigNum24(int Value,int LineOffset)
{

	//how many digits?

	int Dig;

	if(Value>=999)
	{
		Dig = 4;
	}
	else
	{
		if(Value>=99)
		{
			Dig = 3;
		}
		else
		{
			if(Value>=9)
			{
				Dig = 2;
			}
			else
			{
				Dig = 1;
			}
		}
	}

	//depending from the Digits, i need to print it different.

	int D1;
	int D2;
	int D3;
	int D4;

	switch (Dig) {
	case 1:
		//Thats easy...
		printOneNumber2(Value,0,LineOffset+9);

		//And clear digit 1+2+3:
		lcdp->commandWrite(0x80+LineOffset+0);                  //Line1
		blanks3();
		blanks3();
		blanks3();

		lcdp->commandWrite(0xC0+LineOffset+0);                  //Line1
		blanks3();
		blanks3();
		blanks3();

		break;
	case 2:
		//Not so easy anymore.
		D2 = abs(Value/10);  //First Value
		D1 = Value - D2*10;     //Last Value

		printOneNumber2(D2,0,LineOffset+6);
		printOneNumber2(D1,0,LineOffset+9);

		//And clear digit 1+2:
		lcdp->commandWrite(0x80+LineOffset+0);                  //Line1
		blanks3();
		blanks3();


		lcdp->commandWrite(0xC0+LineOffset+0);                  //Line1
		blanks3();
		blanks3();

		break;
	case 3:
		//Not so easy anymore.
		D3 = abs(Value/100);  //First Value
		D2 = abs((Value - D3*100)/10);
		D1 = abs((Value - D3*100 - D2*10));     //Last Value

		printOneNumber2(D3,0,LineOffset+3);
		printOneNumber2(D2,0,LineOffset+6);
		printOneNumber2(D1,0,LineOffset+9);

		//And clear digit 1:
		lcdp->commandWrite(0x80+LineOffset+0);                  //Line1
		blanks3();

		lcdp->commandWrite(0xC0+LineOffset+0);                  //Line1
		blanks3();


		break;
	case 4:
		//Not so easy anymore.
		D4 = abs(Value/1000); //First Value
		D3 = abs((Value - D4*1000)/100);
		D2 = abs((Value - D4*1000 - D3*100)/10);
		D1 = abs((Value - D4*1000 - D3*100 - D2*10));     //Last Value

		printOneNumber2(D4,0,LineOffset+0);
		printOneNumber2(D3,0,LineOffset+3);
		printOneNumber2(D2,0,LineOffset+6);
		printOneNumber2(D1,0,LineOffset+9);


		break;
	}


}

void UserInterface::blanks3()
{
	lcdp->print(254);
	lcdp->print(254);
	lcdp->print(254);
}


//-------------------------------------------------------------------------------------------------------


char* UserInterface::format(unsigned long num){

	byte dp = 3;

	while(num > 999999){
		num /= 10;
		dp++;
		if( dp == 5 )
			break; // We'll lose the top numbers like an odometer
	}
	if(dp == 5)
		dp = 99; // We don't need a decimal point here.

	// Round off the non-printed value.
	if((num % 10) > 4)
		num += 10;
	num /= 10;
	byte x = 6;
	while(x > 0){
		x--;
		if(x==dp){ //time to poke in the decimal point?{
			fBuff[x]='.';
		}else{
			fBuff[x]= '0' + (num % 10);//poke the ascii character for the digit.
			num /= 10;
		}
	}
	fBuff[6] = 0;
	return fBuff;
}

//http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1239715016/3#3
void UserInterface::printfloat(float all , int decimals){
	// will only work with lcd4bit library, basically it breaks the float into 2 integers then prints them seperatley with decimal point in the middle.
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
	char buf[12];
	itoa (parta, buf, 10);
	lcdp->printIn(buf);

	lcdp->printIn(".");

	//if the partb has less then the decimals -1, make a additional 0
	if(dec2 ==2)
	{
		if(partb<10)
		{
			lcdp->printIn("0");
		}
	}

	itoa (partb, buf, 10);
	lcdp->printIn(buf);
}


void UserInterface::lcdShowIntro(int delayValue)
{

	//Must load this in 2 steps, somehow its not possible to load 8 chars at once...
	cgramIntro();
	cgramIntro2();

	//Show the Intro:
	lcdp->commandWrite(0x80+2);                  //Line1
	lcdp->print(1);
	lcdp->print(2);
	lcdp->commandWrite(0x80+10);                  //Line1
	lcdp->print(3);
	lcdp->print(4);

	lcdp->commandWrite(0xC0+2); 			//Line2
	lcdp->print(5);
	lcdp->print(6);
	lcdp->printIn("ulti");
	lcdp->commandWrite(0xC0+10); 			//Line2
	lcdp->print(7);
	lcdp->print(8);
	lcdp->printIn("isplay");

	lcdp->commandWrite(0xD4+1);                  //Line=4
	lcdp->printIn("www.designer2k2.at");

	//Set the LCD brightness: (This turns on the Light, looks nice)
	setBrightness(brightness);

	//Flash the Shiftlight:

	analogWrite(RPMSHIFTLIGHTPIN,RPM_HIGH_BRIGHT);
	delay(50);
	analogWrite(RPMSHIFTLIGHTPIN,RPM_LOW_BRIGHT);
	delay(50);
	analogWrite(RPMSHIFTLIGHTPIN,RPM_NO_BRIGHT);


	delay(delayValue);                      //Short delay, for the Intro screen :)

}

/**
 * Upload a user-defined character to character generator RAM of LCD (5x8)
 * 8 5x8 custom chars are available
 * @see http://www.quinapalus.com/hd44780udg.html for character generator.
 * @see http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1231533425/1#1
 *
 */
void UserInterface::lcdUploadUdef5x8(int character, int *data) {
	int c = character << 3, i;
	for (i = 0; i < 8; i++) {
		lcdp->commandWrite(0x40 | c | i);
		lcdp->print(data[i]);
	}
}


/**
 * uploads dots to cgram
 * ATTENTION: this arent dots but rather 2 bars (at top and bottom) plus minut, plus etc
 */
void UserInterface::cgramDot() {

	int data1[] = {31,0,0,0,0,0,0,31};        //only 2 dots
	int data2[] = {31,0,0,14,0,0,0,31};       //the 2 dots with a minus
	int data3[] = {31,0,4,14,4,0,0,31};       //the 2 dots with a plus
	int data4[] = {31,14,31,31,31,31,14,31};  //thats a block with little rounded edges

	lcdUploadUdef5x8(5,data1);
	lcdUploadUdef5x8(6,data2);
	lcdUploadUdef5x8(7,data3);
	lcdUploadUdef5x8(8,data4);
}
//-------------------------------------------------------------------------------------------------------


/**
 * uploads bar chars to cgram
 */
void UserInterface::cgramBar() {
	int data1[] = {16,16,16,16,16,16,16,16};  //1 line
	int data2[] = {24,24,24,24,24,24,24,24};  //2 line
	int data3[] = {28,28,28,28,28,28,28,28};  //3 line
	int data4[] = {30,30,30,30,30,30,30,30};  //4 line

	lcdUploadUdef5x8(1,data1);
	lcdUploadUdef5x8(2,data2);
	lcdUploadUdef5x8(3,data3);
	lcdUploadUdef5x8(4,data4);
}
//-------------------------------------------------------------------------------------------------------

void UserInterface::cgramIntro() {
	int data1[] = {28,28,28,30,30,31,31,31};  // 1--- k
	int data2[] = {7,7,7,15,15,31,31,31};     // -1-- k
	int data3[] = {31,31,31,28,28,28,28,28};  // --1- k
	int data4[] = {24,28,30,15,7,7,7,7};      // ---1 k

	lcdUploadUdef5x8(1,data1);
	lcdUploadUdef5x8(2,data2);
	lcdUploadUdef5x8(3,data3);
	lcdUploadUdef5x8(4,data4);
}
//-------------------------------------------------------------------------------------------------------

void UserInterface::cgramIntro2() {
	int data1[] = {31,29,29,28,28,28,28,28};  // 2--- k
	int data2[] = {31,23,23,7,7,7,7,7};       // -2-- k
	int data3[] = {28,28,28,28,28,31,31,31};  // --2- k
	int data4[] = {7,7,7,7,15,30,28,24};      // ---2 k

	lcdUploadUdef5x8(5,data1);
	lcdUploadUdef5x8(6,data2);
	lcdUploadUdef5x8(7,data3);
	lcdUploadUdef5x8(8,data4);
}


/**
 * draws a bar to the lcd
 * @param Digits number of chars the bar has on the lcd screen
 * @param Value the actual value. 0 <= value <= 10 x Digits ?!?
 */
void UserInterface::drawBar(int Digits, int Value) {

	int Curs = Value/10;
	for (int i=0; i <= Digits-1; i++) {
		if(i<Curs) {
			lcdp->print(255);
		} else {

			if(i==Curs) {
				//in val ist 84 und in val5 die 8
				//ich brauch die 4 /2 als int
				int ChrW = (Value - Curs*10)/2;

				if(ChrW==0) {
					lcdp->printIn(" ");
				} else {
					lcdp->print(ChrW);
				}
			} else {
				lcdp->printIn(" ");
			}
		}
	}
}


/**
 * uploads vertical bars to cgram
 */
void UserInterface::cgramVertBar() {

	int data1[] = {0,0,0,0,0,0,0,31};              //1 row
	int data2[] = {0,0,0,0,0,0,31,31};             //2 rows
	int data3[] = {0,0,0,0,0,31,31,31};            //3 rows
	int data4[] = {0,0,0,0,31,31,31,31};           //4 rows
	int data5[] = {0,0,0,31,31,31,31,31};          //5 rows
	int data6[] = {0,0,31,31,31,31,31,31};         //6 rows
	int data7[] = {0,31,31,31,31,31,31,31};        //7 rows

	lcdUploadUdef5x8(1,data1);
	lcdUploadUdef5x8(2,data2);
	lcdUploadUdef5x8(3,data3);
	lcdUploadUdef5x8(4,data4);
	lcdUploadUdef5x8(5,data5);
	lcdUploadUdef5x8(6,data6);
	lcdUploadUdef5x8(7,data7);
}
//-------------------------------------------------------------------------------------------------------

void UserInterface::drawVertBar(int Val, int Pos)
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
		lcdp->commandWrite(0x80+Pos);              //Line 1
		lcdp->print(254); //Blank
		lcdp->commandWrite(0xC0+Pos);              //Line 2
		lcdp->print(254); //Blank
		lcdp->commandWrite(0x94+Pos);              //Line 3
		lcdp->print(254); //Blank
		lcdp->commandWrite(0xD4+Pos);              //Line 4
		lcdp->print(Mod); //The Custom Character presenting the value
		break;
	case 1:
		//so there is only one character /1 fulls and the character in the 2nd position
		lcdp->commandWrite(0x80+Pos);              //Line 1
		lcdp->print(254); //Blank
		lcdp->commandWrite(0xC0+Pos);              //Line 2
		lcdp->print(254); //Blank
		lcdp->commandWrite(0x94+Pos);              //Line 3
		lcdp->print(Mod); //The Custom Character presenting the value
		lcdp->commandWrite(0xD4+Pos);              //Line 4
		lcdp->print(255); //Full
		break;
	case 2:
		//so there is only one character /2 fulls and the character in the 3rd position
		lcdp->commandWrite(0x80+Pos);              //Line 1
		lcdp->print(254); //Blank
		lcdp->commandWrite(0xC0+Pos);              //Line 2
		lcdp->print(Mod); //The Custom Character presenting the value
		lcdp->commandWrite(0x94+Pos);              //Line 3
		lcdp->print(255); //Full
		lcdp->commandWrite(0xD4+Pos);              //Line 4
		lcdp->print(255); //Full
		break;
	case 3:
		//so there is only one character /3 fulls and the character in the highest position
		lcdp->commandWrite(0x80+Pos);              //Line 1
		lcdp->print(Mod); //The Custom Character presenting the value
		lcdp->commandWrite(0xC0+Pos);              //Line 2
		lcdp->print(255); //Full
		lcdp->commandWrite(0x94+Pos);              //Line 3
		lcdp->print(255); //Full
		lcdp->commandWrite(0xD4+Pos);              //Line 4
		lcdp->print(255); //Full
		break;
	case 4:
		//easy, eveything is full!
		lcdp->commandWrite(0x80+Pos);              //Line 1
		lcdp->print(255); //Full
		lcdp->commandWrite(0xC0+Pos);              //Line 2
		lcdp->print(255); //Full
		lcdp->commandWrite(0x94+Pos);              //Line 3
		lcdp->print(255); //Full
		lcdp->commandWrite(0xD4+Pos);              //Line 4
		lcdp->print(255); //Full
		break;
	}

	//thats it, easy :)

}


void UserInterface::scopeMode() {

	//but work the screen from left side
	for(int i=0; i<=18; i++){

		//Print the Buffer
		drawVertBar(scopeInt[i],i);

		//shift Data to the left
		scopeInt[i] = scopeInt[i+1];

	}

}

void UserInterface::screenInit() {
	switch(activeScreen){
	case 0:
		//Setup the Screen:
		break;
	case 1:
		screen1Init();
		break;
	case 2:
		screen2Init();
		break;
	case 3:
		screen3Init();
		break;
	case 4:
		screen4Init();
		break;
	case 5:
		screen5Init();
		break;
	case 6:
		screen6Init();
		break;
	case 7:
		screen7Init();
		break;
	case 8:
		screen8Init();
		break;
	case 9:
		screen9Init();
		break;
	default:
		screen1Init();
		break;
	}
}

void UserInterface::screenDraw() {
	//Depending on what screen, it will trigger different things:
	switch(activeScreen){
	case 0:
		break;
	case 1:
		screen1Draw();
		break;
	case 2:
		screen2Draw();
		break;
	case 3:
		screen3Draw();
		break;
	case 4:
		screen4Draw();
		break;
	case 5:
		screen5Draw();
		break;
	case 6:
		screen6Draw();
		break;
	case 7:
		screen7Draw();
		break;
	case 8:
		screen8Draw();
		break;
	case 9:
		screen9Draw();
		break;
	default:
		break;
	}
}


/*==============================================================================
 * BUTTON FUNCTIONS
 *============================================================================*/

void UserInterface::buttonAHold()
{

 //This will switch the Screen
   switch(activeScreen){
   case 0:
      activeScreen = 1;
      screen1Init();
      break;
   case 1:
      activeScreen = 2;
      screen2Init();
      break;
   case 2:
      activeScreen = 3;
      screen3Init();
      break;
   case 3:
      activeScreen = 4;
      screen4Init();
      break;
   case 4:
      activeScreen = 5;
      screen5Init();
      break;
   case 5:
      activeScreen = 6;
      screen6Init();
      break;
   case 6:
      activeScreen = 7;
      screen7Init();
      break;
  case 7:
      activeScreen = 8;
      screen8Init();
      break;
  case 8:
      activeScreen = 9;
      screen9Init();
      break;
  case 9:
      activeScreen = 0;
      screen1Init();
      break;
   default:
      activeScreen = 0;
      break;
   }

  //Set the Timestamp for the Save:

  controllerP->setScreenSave ( millis() + SCREENSAVEDELAY );

}

//-------------------------------------------------------------------------------------------------------

void UserInterface::buttonAPressed()
{

  //Serial.print(time*1);
  //Serial.print(";");
  //Serial.println("Button A Pressed");

  switch(activeScreen){
   case 0:
      break;
   case 1:
      controllerP->ChangeSerOut();      //Switch from RAW to Cal to Nothing and vise versa.
      break;
   case 2:
       //Toggle A and B MCP
      if(mcp == 8) {
        screen2Init(1);
      } else  {
        screen2Init(2);
      }
      break;
   case 3:
	   controllerP->ChangeSerOut();      //Switch from RAW to Cal to Nothing and vise versa.
      break;
   case 6:                 //Switches the 2 Row Screen
	  switch(toggleAB){
	   case SCREENA:
	     toggleAB = SCREENB;
         screen6Init(SCREENA);
	     break;
	   case SCREENB:
	     toggleAB = SCREENA;
         screen6Init(SCREENB);
	     break;
	  }
      break;
   case 7:
      refresh += 10;
      if(refresh>=110)  {
        refresh = 0;
      }
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

  switch(activeScreen){
   case 0:
      break;
   case 1:
      //The Calibration from the LD will be done
      controllerP->CalibrateLD();
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

	switch(activeScreen){
	case 0:
		break;
	case 1:
		//The MaxLD will be reset!
		sdataP->maxLd = 0.0;
		sdataP->maxLdt = sdataP->ldCalPoint;
		break;
	case 2:
		//Change LCD brightness
		brightness++;
		if(brightness>=3)
			brightness = 0;

		setBrightness(brightness);
		//and save the new value:
		EEPROM.write(105,brightness);
		break;
	case 7:
		screen6DataSel++;      //Change the Scope Screen
		if(screen6DataSel >= 5)
			screen6DataSel = 1;

		//Reset the Constrains:
		//FIXME global ?!?
		screen6Val1 = 0;
		screen6Val2 = 5000;
		break;
	case 8:
		screen8Val++;	//changes the MAX screen through all values
		if(screen8Val >= 4)
			screen8Val = 1;
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


// ========================================================================================================

 //Main Screen

 void UserInterface::screen1Init()
 {

 	lcdp->init();

 	lcdp->commandWrite(0x80);                  //Line1
 	lcdp->printIn("EGT:");

 	lcdp->commandWrite(0x94);                  //Line=3
 	lcdp->printIn( "LD:        Max:");

 	//Load the specific data:
 	cgramBar();
 	cgramDot();

 	//Lets to the Conversion:
 	controllerP->DoCal = 1;

 	//Fetch Typ K
 	controllerP->DoTypK = 1;

 	//Select Serial Out Mode "Convertet":
 	controllerP->SerOut = SERIALOUT_ENABLED;

 	//print the Serial Header:
 	controllerP->HeaderPrint();

 }

 void UserInterface::screen1Draw()
 {
	int val = 0;
	int boostMapped = 0;
 	//Calibrate it:
 	val = sdataP->calLambda/10;
 	boostMapped = map(sdataP->anaIn[BOOSTPIN], 0, 4096, 0, 200);
 	boostMapped = constrain(boostMapped, 0, 200);

 	lcdp->commandWrite(0x80+5);               //Line 1, position 5!

 	char buf[12];
 	//AGT1+AGT2/2 thats because off my 2 Probes
 	lcdp->printIn( itoa((sdataP->calAgt[0]+sdataP->calAgt[1])/2, buf, 10));

 	lcdp->print(32);                           //Print a " " at the end, to clear in case its needed

 	lcdp->commandWrite(0x80+14);              //Line 3,

 	printfloat(sdataP->calCaseTemp,1);

 	lcdp->print(223);                         //Print a " " at the end, to clear in case its needed
 	lcdp->printIn( "C");                       //Print a " " at the end, to clear in case its needed

 	lcdp->commandWrite(0x94+3);
 	if(sdataP->calBoost<0.0) {
 		lcdp->printIn("-");
 	} else {
 		lcdp->printIn(" ");
 	}
 	printfloat(abs(sdataP->calBoost),2);              //Shows current Boost

 	//Check for new MaxLD, and then save the point where to make the square
 	if(sdataP->calBoost > sdataP->maxLd) {
 		sdataP->maxLd  = sdataP->calBoost;
 		sdataP->maxLdt = boostMapped/10;
 	}

 	lcdp->commandWrite(0x94+16);
 	printfloat(sdataP->maxLd,2);                    //Max Boost

 	//Lets draw the dot:
 	lcdp->commandWrite(0xC0);              // Line 2
 	for (int i=0; i <= 19; i++) {
 		if(i==val) {
 			lcdp->print(8);        //thats the Dot
 		} else {
 			if(i <= LAMBDALOWERLIMIT) {
 				//from 0-4 a minus then nothing
 				lcdp->print(6);
 			} else {
 				if(i <= LAMBDAUPPERLIMIT) {
 					//from 5-15 ist nothing and then a plus
 					lcdp->print(5);
 				} else {
 					lcdp->print(7);
 				}
 			}
 		}
 	}

 	//Now lets make the Bar:
 	lcdp->commandWrite(0xD4);                // Line 4
 	drawBar(20,boostMapped);

 	//And draw a Dot at the max pos & 0 Pos

 	lcdp->commandWrite(0xD4 + sdataP->maxLdt);                // Line 4
 	lcdp->print(255);

 	lcdp->commandWrite(0xD4 + sdataP->ldCalPoint);                // Line 4
 	lcdp->print(255);

 }

 void UserInterface::setBrightness(int Bright)  {
	 brightness = Bright;
	 analogWrite(LCDBRIGHTPIN,pgm_read_word(&LCDBrightnessD[Bright]));
 }



 int UserInterface::getActiveScreen() const
 {
     return activeScreen;
 }

 void UserInterface::setActiveScreen(int activeScreen)
 {
     this->activeScreen = activeScreen;
 }

 void UserInterface::screen2Init(uint8_t mode) {
	 lcdp->init();

	 if (mode > 1) {
		 lcdp->commandWrite(0x80);                  //Line1
		 lcdp->printIn("A1:       A2:");
		 lcdp->commandWrite(0xC0);                  //Line=2
		 lcdp->printIn("A3:       A4:");
		 lcdp->commandWrite(0x94);                  //Line=3
		 lcdp->printIn("A5:       A6:");
		 lcdp->commandWrite(0xD4);                  //Line=4
		 lcdp->printIn("A7:       A8:");
	 } else {
		 lcdp->commandWrite(0x80);                  //Line1
		 lcdp->printIn("B1:       B2:");
		 lcdp->commandWrite(0xC0);                  //Line=2
		 lcdp->printIn("B3:       B4:");
		 lcdp->commandWrite(0x94);                  //Line=3
		 lcdp->printIn("B5:       B6:");
		 lcdp->commandWrite(0xD4);                  //Line=4
		 lcdp->printIn("B7:       B8:");
	 }


	 refresh = 2;

	 refreshCounter = 2;

	 //Select Serial Out Mode RAW:
	 controllerP->SerOut = SERIALOUT_RAW;

	 //And calibration:
	 controllerP->DoCal = 1;

	 //Fetch Typ K
	 controllerP->DoTypK = 1;

	 //print the Serial Header:
	 controllerP->HeaderPrint();

	 //select A (0 Offset)
	 if ( mode == 1 )
		 mcp = 0;
	 else
		 //select B (8 Offset)
		 mcp = 8;
 }


 void UserInterface::print2Blanks() {
   lcdp->print(32);
   lcdp->print(32);
 }

 void UserInterface::screen2Draw()  {
   //This screen only shows the current Value from all 8 Inputs in RAW
   //and only after every X refreshes, so its faster on the serial & its easier to read

   if( refreshCounter >=refresh ) {
      refreshCounter = 0;

     lcdp->commandWrite(0x80+4);                  //A1
     lcdp->printIn(itoa((sdataP->anaIn[1+mcp]), buf, 10));
     print2Blanks();
     lcdp->commandWrite(0x80+14);                 //A2
     lcdp->printIn(itoa((sdataP->anaIn[2+mcp]), buf, 10));
     print2Blanks();
     lcdp->commandWrite(0xC0+4);                  //A3
     lcdp->printIn(itoa((sdataP->anaIn[3+mcp]), buf, 10));
     print2Blanks();
     lcdp->commandWrite(0xC0+14);                 //A4
     lcdp->printIn(itoa((sdataP->anaIn[4+mcp]), buf, 10));
     print2Blanks();
     lcdp->commandWrite(0x94+4);                  //A5
     lcdp->printIn(itoa((sdataP->anaIn[5+mcp]), buf, 10));
     print2Blanks();
     lcdp->commandWrite(0x94+14);                 //A6
     lcdp->printIn(itoa((sdataP->anaIn[6+mcp]), buf, 10));
     print2Blanks();
     lcdp->commandWrite(0xD4+4);                  //A7
     lcdp->printIn(itoa((sdataP->anaIn[7+mcp]), buf, 10));
     print2Blanks();
     lcdp->commandWrite(0xD4+14);                 //A8
     lcdp->printIn(itoa((sdataP->anaIn[8+mcp]), buf, 10));
     print2Blanks();
   } else {
     refreshCounter++;
   }
 }


 void UserInterface::screen3Init() {

	 lcdp->init();

	 lcdp->commandWrite(0x80);                  //Line1
	 lcdp->printIn("LAM:");
	 lcdp->commandWrite(0xC0);                  //Line=2
	 lcdp->printIn("LD :");
	 lcdp->commandWrite(0x94);                  //Line=3
	 lcdp->printIn("THR:");
	 lcdp->commandWrite(0xD4);                  //Line=4
	 lcdp->printIn("RPM:");

	 cgramBar();

	 //Select Serial Out Mode Converted:
	 controllerP->SerOut = 2;

	 //And calibration:
	 controllerP->DoCal = 1;

	 //Fetch Typ K
	 controllerP->DoTypK = 1;

	 //print the Serial Header:
	 controllerP->HeaderPrint();


	 refresh  = 10;

	 //FIXME TODO why ???
	 //   val = 50;

 }

 void UserInterface::screen3Draw()  {

	 //First, show the Bars on the last 10 Chars:

	 int boostMapped  = map(sdataP->anaIn[BOOSTPIN], 0, 4096, 0, 100);
	 boostMapped = constrain(boostMapped, 0, 100);
	 int rpmMapped = map(sdataP->anaIn[RPMPIN], 0, RPMMAX, 0, 100);
	 rpmMapped = constrain(rpmMapped, 0, 100);

	 //Print the Values:


	 lcdp->commandWrite(0x80+5);
	 lcdp->printIn(itoa(sdataP->calLambda, buf, 10));
	 lcdp->print(32);
	 lcdp->commandWrite(0xC0+4);

	 if(sdataP->calBoost<0.0) {
		 lcdp->printIn("-");
	 } else {
		 lcdp->printIn(" ");
	 }
	 printfloat(abs(sdataP->calBoost),2);              //Shows current Boost

	 lcdp->print(32);
	 lcdp->commandWrite(0x94+5);
	 lcdp->printIn(itoa(sdataP->calThrottle, buf, 10));
	 lcdp->print(32);
	 lcdp->commandWrite(0xD4+5);
	 lcdp->printIn(itoa((sdataP->calRPM/10)*10, buf, 10));    //gets rid of the last 10, thats not needed.
	 lcdp->print(32);


	 //Now lets make the Bars:
	 lcdp->commandWrite(0x80+10);
	 drawBar(10,sdataP->calLambda/2);
	 lcdp->commandWrite(0xC0+10);
	 drawBar(10,boostMapped);
	 lcdp->commandWrite(0x94+10);
	 drawBar(10,sdataP->calThrottle);
	 lcdp->commandWrite(0xD4+10);
	 drawBar(10,rpmMapped);
 }


 //Typ K Screen
 void UserInterface::screen4Init()  {
	 lcdp->init();

	 lcdp->commandWrite(0x80);                  //Line1
	 lcdp->printIn("T1:       T2:");
	 lcdp->commandWrite(0xC0);                  //Line=2
	 lcdp->printIn("T3:       T4:");
	 lcdp->commandWrite(0x94);                  //Line=3
	 lcdp->printIn("T5:       T6:");
	 lcdp->commandWrite(0xD4);                  //Line=4
	 lcdp->printIn("T7:       T8:");

	 refresh = 2;
	 refreshCounter = 2;

	 //Lets to the Conversion:
	 controllerP->DoCal = 1;

	 //Fetch Typ K
	 controllerP->DoTypK = 1;

	 controllerP->SerOut = SERIALOUT_ENABLED;

	 controllerP->HeaderPrint();
 }

 void UserInterface::screen4Draw()  {

	 //This just printÂ´s all the Thermocouple values:
	 //like screen2 it only updates every X repeatÂ´s

	 if( refreshCounter >= refresh ) {
		 refreshCounter = 0;

		 lcdp->commandWrite(0x80+4);                  //T1
		 lcdp->printIn(itoa((sdataP->calAgt[0]), buf, 10));
		 print2Blanks();
		 lcdp->commandWrite(0x80+14);                 //T2
		 lcdp->printIn(itoa((sdataP->calAgt[1]), buf, 10));
		 print2Blanks();
		 lcdp->commandWrite(0xC0+4);                  //T3
		 lcdp->printIn(itoa((sdataP->calAgt[2]), buf, 10));
		 print2Blanks();
		 lcdp->commandWrite(0xC0+14);                 //T4
		 lcdp->printIn(itoa((sdataP->calAgt[3]), buf, 10));
		 print2Blanks();
		 lcdp->commandWrite(0x94+4);                  //T5
		 lcdp->printIn(itoa((sdataP->calAgt[4]), buf, 10));
		 print2Blanks();
		 lcdp->commandWrite(0x94+14);                 //T6
		 lcdp->printIn(itoa((sdataP->calAgt[5]), buf, 10));
		 print2Blanks();
		 lcdp->commandWrite(0xD4+4);                  //T7
		 lcdp->printIn(itoa((sdataP->calAgt[6]), buf, 10));
		 print2Blanks();
		 lcdp->commandWrite(0xD4+14);                 //T8
		 lcdp->printIn(itoa((sdataP->calAgt[7]), buf, 10));
		 print2Blanks();
	 } else {
		 refreshCounter++;
	 }
	 //delay(50);
 }


 //1 Row Big Font Screen
 void UserInterface::screen5Init()  {

	 lcdp->init();

	 lcdp->commandWrite(0x80);                  //Line1
	 lcdp->printIn("Boost:");

	 lcdp->commandWrite(0x94);                  //Line2
	 lcdp->printIn("Max:");

	 //Load the specific data:
	 cgramBigFont4();

	 //Lets to the Conversion:
	 controllerP->DoCal = 1;

	 //Fetch Typ K
	 controllerP->DoTypK = 1;

	 controllerP->SerOut = SERIALOUT_ENABLED;

	 //print the Serial Header:
	 controllerP->HeaderPrint();

	 refresh = 10;
	 refreshCounter = 10;

 }

 void UserInterface::screen5Draw() {

	 //and only after every X refreshes, so its faster on the serial & its easier to read

	 if( refreshCounter >= refresh ) {
		 refreshCounter = 0;

		 lcdp->commandWrite(0xC0+7);
		 if( sdataP->calBoost < 0.0 ) {
			 lcdp->print(2);
			 lcdp->print(2);
		 } else {
			 lcdp->printIn("  ");
		 }

		 //bigNum(abs(CalBoost)*1000,0,9,1);

		 bigNum4(abs(sdataP->calBoost)*1000,9,1);

	 } else {
		 refreshCounter++;
	 }

	 //Check for new MaxLD
	 if( sdataP->calBoost > sdataP->maxLd) {
		 sdataP->maxLd  = sdataP->calBoost;
	 }

	 lcdp->commandWrite(0xD4);
	 printfloat(sdataP->maxLd,2);                    //Max Boost
 }


 //2 Row Big Font Screen
 void UserInterface::screen6Init(uint8_t mode) {

	 lcdp->init();

	 if ( mode == SCREENA ) {
		 lcdp->commandWrite(0x80);                  //Line1
		 lcdp->printIn("Boost:");

		 lcdp->commandWrite(0x94);                  //Line3
		 lcdp->printIn("Temp:");
	 } else {
		 lcdp->commandWrite(0x80);                  //Line1
		 lcdp->printIn("T1:   ");

		 lcdp->commandWrite(0x94);                  //Line3
		 lcdp->printIn("T2:   ");

		 //Clear the - from the screen A
		 lcdp->commandWrite(0x80+7);
		 lcdp->printIn("  ");

	 }

	 //Load the specific data:
	 cgramBigFont2();

	 //Lets to the Conversion:
	 controllerP->DoCal = 1;

	 //Fetch Typ K
	 controllerP->DoTypK = 1;

	 //Select Serial Out Mode "Convertet":
	 controllerP->SerOut = SERIALOUT_ENABLED;

	 //print the Serial Header:
	 controllerP->HeaderPrint();

	 refresh = 5;
	 refreshCounter  = 10;
	 screen6Var = 1;

 }


 void UserInterface::screen6Draw()  {

	 //and only after every X refreshes, so its faster on the serial & its easier to read

	 if(refreshCounter>=refresh) {
		 refreshCounter = 0;

		 switch(screen6Var){
		 case 1:

			 //The Boost:

			 lcdp->commandWrite(0x80+7);
			 if(sdataP->calBoost<0.0) {
				 lcdp->print(2);
				 lcdp->print(2);
			 } else {
				 lcdp->printIn("  ");
			 }

			 bigNum(abs(sdataP->calBoost)*1000,0,9,1);

			 //The Temp:
			 bigNum(abs(sdataP->calCaseTemp)*1000,20,9,1);


			 break;
		 case 2:
			 //T1:
			 bigNum24(sdataP->calAgt[0],0+8);

			 //T2:
			 //bigNum24(CalAGT[1],20);
			 bigNum24(sdataP->calAgt[1],20+8);

			 break;
		 case 3:
			 screen6Var = 1;
			 break;
		 }


	 }
	 else {
		 refreshCounter++;
	 }
 }


 //Scope Mode Screen
void UserInterface::screen7Init()  {

	 lcdp->init();

	 //Load the specific data:
	 cgramVertBar();

	 //Lets not to the Conversion:
	 controllerP->DoCal = 0;

	 //Fetch Typ K
	 controllerP->DoTypK = 0;

	 //Select Serial Out Mode "Nothing":
	 controllerP->SerOut = SERIALOUT_DISABLED;

	 //print the Serial Header:
	 controllerP->HeaderPrint();

	 //The Constrains:
	 screen6Val1 = 0;
	 screen6Val2 = 5000;

	 //The Data Selector
	 screen6DataSel = 1;

	 refresh = 10;
	 refreshCounter = 10;
 }


void UserInterface::screen7Draw() {

	//new Constrains if needed
	int ScopeVal = 0;

	switch(screen6DataSel){
	case 1:
		ScopeVal = sdataP->anaIn[BOOSTPIN];
		break;
	case 2:
		ScopeVal = sdataP->calThrottle;
		break;
	case 3:
		ScopeVal = sdataP->calLMM;
		break;
	case 4:
		ScopeVal = sdataP->calRPM;
		break;
	default:
		break;
	}


	if(ScopeVal<screen6Val2)    {
		screen6Val2 = ScopeVal;
	}

	if(ScopeVal>screen6Val1)    {
		screen6Val1 = ScopeVal;
	}

	//Get the Data:
	int val3;
	val3 = map(ScopeVal, screen6Val2, screen6Val1, 0, 33);
	//val3 = constrain(val3, 0, 32);

	//Show only the rightmost bar: (live screen)
	drawVertBar(val3,19);

	//and show it only after every X refreshes, so its faster on the serial & its easier to read
	if(refreshCounter>=refresh) {
		refreshCounter = 0;

		//Feed it into the Buffer (on the right side)
		scopeInt[19] = val3;

		scopeMode();

	} else {
		refreshCounter++;
	}


	//Draw the Caption:
	lcdp->commandWrite(0x80);                  //Line1
	lcdp->printIn(itoa(screen6DataSel, buf, 10));
	lcdp->printIn(":");
	lcdp->printIn(itoa(refresh, buf, 10));

	Serial.println(controllerP->getTime());
}


//Max Value Screen (This shows the Max LD, RPM or AGT or Oil values)
void UserInterface::screen8Init()  {
	lcdp->init();

	lcdp->commandWrite(0x80);                  //Line1
	lcdp->printIn("Max ");
	lcdp->commandWrite(0xC0);                  //Line=2
	lcdp->printIn("LD:       RPM:");
	lcdp->commandWrite(0x94);                  //Line=3
	lcdp->printIn("LMM:      AGT:");
	//lcdp->commandWrite(0xD4);                  //Line=4
	//lcdp->printIn("T7:       T8:");

	refresh = 2;
	refreshCounter = 2;

	//Selects the first MAX Screen
	screen8Val = 1;

	//Lets to the Conversion:
	controllerP->DoCal = 1;

	//Fetch Typ K
	controllerP->DoTypK = 1;

	//Select Serial Out Mode "Convertet":
	controllerP->SerOut = 2;

	//print the Serial Header:
	controllerP->HeaderPrint();

}

void UserInterface::screen8Draw()  {
	//like screen2 it only updates every X repeatÂ´s

	if(refreshCounter>=refresh)  {
		refreshCounter = 0;

		//lcdp->commandWrite(0x80+4);                  //T1
		//Print2Blanks();
		//lcdp->commandWrite(0x80+14);                 //T2
		//lcdp->printIn(itoa((CalAGT[1]), buf, 10));
		//Print2Blanks();

		//Header:
		lcdp->commandWrite(0x80+4);                  //Line
		lcdp->printIn(itoa(screen8Val, buf, 10));


		lcdp->commandWrite(0xC0+3);                  //Max1LD

		if(sdataP->maxLdE[screen8Val]<0.0)  {
			lcdp->printIn("-");
		} else {
			lcdp->printIn(" ");
		}
		printfloat(abs(sdataP->maxLdE[screen8Val]),2);              //Shows current Boost



		//lcdp->printIn(itoa((CalAGT[2]), buf, 10));
		//Print2Blanks();
		lcdp->commandWrite(0xC0+14);                 //Max1RPM
		lcdp->printIn(itoa(sdataP->maxRpmE[screen8Val], buf, 10));
		print2Blanks();
		lcdp->commandWrite(0x94+4);                  //LMM
		lcdp->printIn(itoa((sdataP->maxLmmE[screen8Val]), buf, 10));
		print2Blanks();
		lcdp->commandWrite(0x94+14);                 //Max1AGTVal
		lcdp->printIn(itoa(sdataP->maxAgtValE[screen8Val], buf, 10));
		print2Blanks();
		//lcdp->commandWrite(0xD4+4);                  //T7
		//lcdp->printIn(itoa((CalAGT[6]), buf, 10));
		//print2Blanks();
		//lcdp->commandWrite(0xD4+14);                 //T8
		//lcdp->printIn(itoa((CalAGT[7]), buf, 10));
		//print2Blanks();

	} else {
		refreshCounter++;
	}
	//delay(50);
}

//VDO Screen
void UserInterface::screen9Init() {
	lcdp->init();

	lcdp->commandWrite(0x80);                  //Line1
	lcdp->printIn("VDO Screen:");
	lcdp->commandWrite(0xC0);                  //Line=2
	lcdp->printIn("T1:       P1:");
	lcdp->commandWrite(0x94);                  //Line=3
	lcdp->printIn("T2:       P2:");
	lcdp->commandWrite(0xD4);                  //Line=4
	lcdp->printIn("T3:       P3:");

	refresh = 2;
	refreshCounter = 2;

	//Lets to the Conversion:
	controllerP->DoCal = 1;

	//Fetch Typ K
	controllerP->DoTypK = 1;

	//Select Serial Out Mode "Convertet":
	controllerP->SerOut = 2;

	//print the Serial Header:
	controllerP->HeaderPrint();
}

void UserInterface::screen9Draw() {

	if(refreshCounter>=refresh) {
		refreshCounter = 0;

		lcdp->commandWrite(0xC0+4);                  //T1
		lcdp->printIn(itoa(sdataP->VDOTemp1, buf, 10));
		print2Blanks();
		lcdp->commandWrite(0xC0+14);                 //P1
		printfloat(float(sdataP->VDOPres1/10.0),1);
		print2Blanks();
		lcdp->commandWrite(0x94+4);                  //T2
		lcdp->printIn(itoa(sdataP->VDOTemp2, buf, 10));
		print2Blanks();
		lcdp->commandWrite(0x94+14);                 //P2
		printfloat(float(sdataP->VDOPres2/10.0),1);
		print2Blanks();
		lcdp->commandWrite(0xD4+4);                  //T3
		lcdp->printIn(itoa(sdataP->VDOTemp3, buf, 10));
		print2Blanks();
		lcdp->commandWrite(0xD4+14);                 //P3
		printfloat(float(sdataP->VDOPres3/10.0),1);
		print2Blanks();

	} else  {
		refreshCounter++;
	}
	//delay(50);
}

