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


#include <stdio.h>

#include "wiring.h"

#include "MultidisplayDefines.h"
#include "LCDController.h"
#include "LCDScreen1.h"
#include "LCDScreen2.h"
#include "LCDScreen3.h"
#include "LCDScreen4.h"
#include "LCDScreen5.h"
#include "LCDScreen6.h"
#include "LCDScreen7.h"
#include "LCDScreen8.h"
#include "LCDScreen9.h"

const uint8_t	LCDController::ystart[] = { 0x80, 0xC0, 0x94, 0xD4 };


LCDController::LCDController() : activeScreen(0), brightness(2) {
	lcdp = new LCD4Bit(4);
	cbuf = (char*) malloc (sizeof(char) * LCD_BUFSIZE);

	lcdp->init();

    for ( int i = 0 ; i < 20 ; i++ )
            scopeInt[i]=0;

    myScreens[0] = new LCDScreen1();
    myScreens[1] = new LCDScreen2();
    myScreens[2] = new LCDScreen3();
    myScreens[3] = new LCDScreen4();
    myScreens[4] = new LCDScreen5();
    myScreens[5] = new LCDScreen6();
    myScreens[6] = new LCDScreen7();
    myScreens[7] = new LCDScreen8();
    myScreens[8] = new LCDScreen9();
}

void LCDController::init() {
	myScreens[activeScreen]->init();
}



void LCDController::draw() {
	myScreens[activeScreen]->init();
}

//The LCD brightness, what could be cycled through
const uint8_t LCDController::LCDBrightnessD[]= {
   50,125,255        //Change this for other settings depending on what you like
 };

void LCDController::toggleBrightness() {
	if (brightness == 3)
		brightness = 0;
	else
		brightness++;
	setBrightness ( brightness );
}

void LCDController::setBrightness(uint8_t bright)  {
	brightness = constrain(bright,0,2);
    analogWrite(LCDBRIGHTPIN,pgm_read_word(&LCDBrightnessD[brightness]));
}

void LCDController::print2Blanks() {
   lcdp->print(32);
   lcdp->print(32);
}

void LCDController::toggleScreen () {
	// 9 screens atm -> 0..8!
	if ( activeScreen == 8 )
		activeScreen = 0;
	else
		activeScreen++;
	init();
}


//TODO convert into one 2D array!
//const prog_uchar LCDController::bn12[]={255,1,255,0, 1,255,254,0, 1,3,255,0, 1,3,255,0, 255,2,2,0, 255,3,1,0, 255,3,1,0, 1,1,255,0, 255,3,255,0, 255,3,255,0};
char LCDController::bn12[]={255,1,255,0, 1,255,254,0, 1,3,255,0, 1,3,255,0, 255,2,2,0, 255,3,1,0, 255,3,1,0, 1,1,255,0, 255,3,255,0, 255,3,255,0};
char LCDController::bn22[]={255,2,255,0, 2,255,2,0,   255,2,2,0, 2,2,255,0, 254,255,254,0, 2,2,255,0,   255,2,255,0, 254,255,254,0, 255,2,255,0, 254,254,255,0};
// Array index into parts of big numbers. Numbers consist of 12 custom characters in 4 lines
//              			0               1            2              3                4             5             6              7              8              9
char LCDController::bn14[]={1,2,3,0,       2,3,254,0,     1,2,3,0,       1,2,3,0,       2,254,254,0,   2,2,2,0,       1,2,3,0,       2,2,2,0,       1,2,3,0,       1,2,3,0};
char LCDController::bn24[]={255,254,255,0, 254,255,254,0, 1,2,255,0,     254,2,255,0,   255,2,2,0,     255,2,2,0,     255,2,3,0,     254,2,255,0,   255,2,255,0,   255,254,255,0};
char LCDController::bn34[]={255,254,255,0, 254,255,254,0, 255,254,254,0, 254,254,255,0, 254,255,254,0, 254,254,255,0, 255,254,255,0, 254,255,254,0, 255,254,255,0, 4,6,255,0};
char LCDController::bn44[]={4,6,5,0,       6,6,6,0,       4,6,6,0,       4,6,5,0,       254,6,254,0,   6,6,5,0,       4,6,5,0,       254,6,254,0,   4,6,5,0,       254,254,6,0};



//Bigfont Code
//
//BIG parts from http://opengauge.googlecode.com/svn/trunk/mpguino/mpguino.pde

void LCDController::cgramBigFont2() {
	//255 is the "Full Block" Code, and 254 is blank
	const prog_uint8_t data1[] = {31,31,31,0,0,0,0,0};    //Small Block on top
	const prog_uint8_t data2[] = {0,0,0,0,0,31,31,31};    //Small Block on bottom
	const prog_uint8_t data3[] = {31,31,31,0,0,0,31,31};  //Small Block on top and bottom
	// const prog_uint8_t data4[] = {28,30,31,31,31,31,30,28};  //Full Block roundet right
	// const prog_uint8_t data5[] = {7,15,31,31,31,31,15,7};  //Full Block roundet left
	const prog_uint8_t data6[] = {0,0,0,14,14,14,12,8};  //Dot, for Decimal.

	lcdUploadUdef5x8_P (1,data1);
	lcdUploadUdef5x8_P (2,data2);
	lcdUploadUdef5x8_P (3,data3);
	// LcdUploadUdef5x8_P (4,data4);
	// LcdUploadUdef5x8_P (5,data5);
	lcdUploadUdef5x8_P (6,data6);
}
//-------------------------------------------------------------------------------------------------------

void LCDController::cgramBigFont4() {

	//255 is the "Full Block" Code, and 254 is blank
	const prog_uint8_t data1[] = {0,0,0,0,3,15,15,31};    //Small Block on bottom left
	const prog_uint8_t data2[] = {0,0,0,0,31,31,31,31};    //Small Block on bottom right
	const prog_uint8_t data3[] = {0,0,0,0,24,30,30,31};  //Small Block on bottom full
	const prog_uint8_t data4[] = {31,15,15,3,0,0,0,0};  //Small Block on top left
	const prog_uint8_t data5[] = {31,30,30,24,0,0,0,0};  //Small Block on top right
	const prog_uint8_t data6[] = {31,31,31,31,0,0,0,0};  //Small Block on top full
	const prog_uint8_t data7[] = {14,14,14,14,12,8,0,0};  //Dot, for Decimal.

	lcdUploadUdef5x8_P (1,data1);
	lcdUploadUdef5x8_P (2,data2);
	lcdUploadUdef5x8_P (3,data3);
	lcdUploadUdef5x8_P (4,data4);
	lcdUploadUdef5x8_P (5,data5);
	lcdUploadUdef5x8_P (6,data6);
	lcdUploadUdef5x8_P (7,data7);

}

/**
 * prints a 2 line number
 * @param digit
 * @param x_offset 1 unit is one lcd char, not one big char!
 * @param y_offset lcd lines!
 * TODO do we need boundary check?
 */
void LCDController::printOneNumber2(uint8_t digit, uint8_t x_offset, uint8_t y_offset) {
	// Line 1 of the one digit number
	lcdp->commandWrite( ystart[y_offset] + x_offset); //Line1

	lcdp->print (bn12[digit*3+digit*1]);
	lcdp->print (bn12[digit*3+1+digit*1]);
	lcdp->print (bn12[digit*3+2+digit*1]);

	// Line 2 of the one-digit number
	lcdp->commandWrite(ystart[y_offset+1] + x_offset); // Line 2
	lcdp->print(bn22[digit*3+digit*1]);
	lcdp->print(bn22[digit*3+1+digit*1]);
	lcdp->print(bn22[digit*3+2+digit*1]);
}

//-------------------------------------------------------------------------------------------------------

/**
 * prints a 4 line number
 * @param digit
 * @param x_offset 1 unit is one lcd char, not one big char!
 * @param y_offset lcd lines!
 * TODO do we need boundary check?
 * TODO untested!
 */
void LCDController::printOneNumber4(uint8_t digit, uint8_t x_offset, uint8_t y_offset) {

	// Line 1 of the one digit number
	lcdp->commandWrite( ystart[y_offset] + x_offset);                  //Line 1
	lcdp->print(bn14[digit*3]);
	lcdp->print(bn14[digit*3+1]);
	lcdp->print(bn14[digit*3+2]);

	// Line 2 of the one-digit number
	lcdp->commandWrite( ystart[y_offset+1] + x_offset);
	lcdp->print(bn24[digit*3]);
	lcdp->print(bn24[digit*3+1]);
	lcdp->print(bn24[digit*3+2]);

	// Line 3 of the one digit number
	lcdp->commandWrite( ystart[y_offset+2] + x_offset);
	lcdp->print(bn34[digit*3]);
	lcdp->print(bn34[digit*3+1]);
	lcdp->print(bn34[digit*3+2]);

	// Line 4 of the one-digit number
	lcdp->commandWrite( ystart[y_offset+3] + x_offset);
	lcdp->print(bn44[digit*3]);
	lcdp->print(bn44[digit*3+1]);
	lcdp->print(bn44[digit*3+2]);
}

//-------------------------------------------------------------------------------------------------------


void LCDController::blanks(uint8_t c) {
	for ( uint8_t i = 0 ; i < c ; i++ )
		lcdp->print(254);
}



//http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1239715016/3#3
void LCDController::printfloat(float all , int decimals){
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


void LCDController::lcdShowIntro(int delayValue) {

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
	lcdp->printIn_P ( PSTR("ulti") );
	lcdp->commandWrite(0xC0+10); 			//Line2
	lcdp->print(7);
	lcdp->print(8);
	lcdp->printIn_P ( PSTR("isplay") );

	lcdp->commandWrite(0xD4+1);                  //Line=4
	lcdp->printIn_P ( PSTR("www.designer2k2.at") );

	//Set the LCD brightness: (This turns on the Light, looks nice)
	lcdController.setBrightness(2);

	//Flash the Shiftlight:

	analogWrite(RPMSHIFTLIGHTPIN,RPM_HIGH_BRIGHT);
	delay(50);
	analogWrite(RPMSHIFTLIGHTPIN,RPM_LOW_BRIGHT);
	delay(50);
	analogWrite(RPMSHIFTLIGHTPIN,RPM_NO_BRIGHT);


	delay(delayValue);                      //Short delay, for the Intro screen :)

}

/**
 * Upload a user-defined character stored in program mem to character generator RAM of LCD (5x8)
 * 8 5x8 custom chars are available
 * \param character index of the character you want to define (there's eight user definable characters)
 * \param data pointer into program memory (!!) to an array of eight bytes that defines the character.
 *
 * To print a user defined character you just write the index to the LCD. E.g. user defined character 1 is printed by writing the byte 1. That's it.
 *
 * @see http://www.quinapalus.com/hd44780udg.html for character generator.
 * @see http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1231533425/1#1
 *
 */
void LCDController::lcdUploadUdef5x8_P(uint8_t charIndex, const uint8_t *data) {
	charIndex = charIndex << 3;
	for (uint8_t i = 0; i < 8; i++) {
		lcdp->commandWrite(0x40 | charIndex | i);
		lcdp->print( pgm_read_byte (data[i]) );
	}
}


/**
 * uploads dots to cgram
 * ATTENTION: this arent dots but rather 2 bars (at top and bottom) plus minus, plus etc
 */
void LCDController::cgramDot() {

	const prog_uint8_t data1[] = {31,0,0,0,0,0,0,31};        //only 2 dots
	const prog_uint8_t data2[] = {31,0,0,14,0,0,0,31};       //the 2 dots with a minus
	const prog_uint8_t data3[] = {31,0,4,14,4,0,0,31};       //the 2 dots with a plus
	const prog_uint8_t data4[] = {31,14,31,31,31,31,14,31};  //thats a block with little rounded edges

	lcdUploadUdef5x8_P (5,data1);
	lcdUploadUdef5x8_P (6,data2);
	lcdUploadUdef5x8_P (7,data3);
	lcdUploadUdef5x8_P (8,data4);
}
//-------------------------------------------------------------------------------------------------------


/**
 * uploads bar chars to cgram
 */
void LCDController::cgramBar() {
	const prog_uint8_t data1[] = {16,16,16,16,16,16,16,16};  //1 line
	const prog_uint8_t data2[] = {24,24,24,24,24,24,24,24};  //2 line
	const prog_uint8_t data3[] = {28,28,28,28,28,28,28,28};  //3 line
	const prog_uint8_t data4[] = {30,30,30,30,30,30,30,30};  //4 line

	lcdUploadUdef5x8_P (1,data1);
	lcdUploadUdef5x8_P (2,data2);
	lcdUploadUdef5x8_P (3,data3);
	lcdUploadUdef5x8_P (4,data4);
}
//-------------------------------------------------------------------------------------------------------

void LCDController::cgramIntro() {
	const prog_uint8_t data1[] = {28,28,28,30,30,31,31,31};  // 1--- k
	const prog_uint8_t data2[] = {7,7,7,15,15,31,31,31};     // -1-- k
	const prog_uint8_t data3[] = {31,31,31,28,28,28,28,28};  // --1- k
	const prog_uint8_t data4[] = {24,28,30,15,7,7,7,7};      // ---1 k

	lcdUploadUdef5x8_P (1,data1);
	lcdUploadUdef5x8_P (2,data2);
	lcdUploadUdef5x8_P (3,data3);
	lcdUploadUdef5x8_P (4,data4);
}
//-------------------------------------------------------------------------------------------------------

void LCDController::cgramIntro2() {
	const prog_uint8_t data1[] = {31,29,29,28,28,28,28,28};  // 2--- k
	const prog_uint8_t data2[] = {31,23,23,7,7,7,7,7};       // -2-- k
	const prog_uint8_t data3[] = {28,28,28,28,28,31,31,31};  // --2- k
	const prog_uint8_t data4[] = {7,7,7,7,15,30,28,24};      // ---2 k

	lcdUploadUdef5x8_P (5,data1);
	lcdUploadUdef5x8_P (6,data2);
	lcdUploadUdef5x8_P (7,data3);
	lcdUploadUdef5x8_P (8,data4);
}


/**
 * draws a bar to the lcd
 * @param Digits number of chars the bar has on the lcd screen
 * @param Value the actual value. 0 <= value <= 10 x Digits ?!?
 */
void LCDController::drawBar(int Digits, int Value) {

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
void LCDController::cgramVertBar() {

	const prog_uint8_t data1[] = {0,0,0,0,0,0,0,31};              //1 row
	const prog_uint8_t data2[] = {0,0,0,0,0,0,31,31};             //2 rows
	const prog_uint8_t data3[] = {0,0,0,0,0,31,31,31};            //3 rows
	const prog_uint8_t data4[] = {0,0,0,0,31,31,31,31};           //4 rows
	const prog_uint8_t data5[] = {0,0,0,31,31,31,31,31};          //5 rows
	const prog_uint8_t data6[] = {0,0,31,31,31,31,31,31};         //6 rows
	const prog_uint8_t data7[] = {0,31,31,31,31,31,31,31};        //7 rows

	lcdUploadUdef5x8_P (1,data1);
	lcdUploadUdef5x8_P (2,data2);
	lcdUploadUdef5x8_P (3,data3);
	lcdUploadUdef5x8_P (4,data4);
	lcdUploadUdef5x8_P (5,data5);
	lcdUploadUdef5x8_P (6,data6);
	lcdUploadUdef5x8_P (7,data7);
}
//-------------------------------------------------------------------------------------------------------

void LCDController::drawVertBar(int Val, int Pos) {
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



void LCDController::scopeMode() {

	//but work the screen from left side
	for(int i=0; i<=18; i++){

		//Print the Buffer
		drawVertBar(scopeInt[i],i);

		//shift Data to the left
		scopeInt[i] = scopeInt[i+1];
	}
}

void LCDController::printBigNum (uint16_t value, uint8_t length, uint8_t x_offset, uint8_t y_offset, uint8_t type) {
	snprintf(cbuf, LCD_BUFSIZE, "%d", value);
	printBigNum ( cbuf, length, x_offset, y_offset, type );
}

void LCDController::printBigNum (double value, uint8_t length, uint8_t x_offset, uint8_t y_offset, uint8_t type) {
	snprintf(cbuf, LCD_BUFSIZE, "%.2f", value);
	printBigNum ( cbuf, length, x_offset, y_offset, type );
}

/**
* displays a digit Number over 2 or 4 lines
* right aligned, it clears all in front starting at x_offset
* @param str string to be printed
* @param length
* @param x_offset
* @param y_offset
* @param type LCD_BIGFONT_2 or LCD_BIGFONT_4 (2 or 4 rows)
*/
void LCDController::printBigNum (char* str, uint8_t length, uint8_t x_offset, uint8_t y_offset, uint8_t type) {
	//blank 254
	//point 7
	//comma 5

	//set to pos where decimal point should be, 0 if no
	uint8_t dp_pos = 99;


	uint8_t strlen = 0;

	//TODO do we need one blank between 4 row numbers ?!?

	//compute array of uint8_t out of the char array
	uint8_t value_i[21];
    char c[2];
    c[1] = '\0';
    for ( uint8_t i = 0 ; i < sizeof(&str) ; i++ ) {
    	c[0] = str[i];
    	if (c[0]=='.' || c[0]==';') {
    		value_i[i]=c[0];
    		dp_pos = i;
    		if (type==LCD_BIGFONT_2)
    			strlen++;
    	} else {
    		if ( c[0] != ' ') {
    			value_i[i] = atoi(&c[0]);
    			if (type==LCD_BIGFONT_4)
    				strlen += 4;
    			else
    				strlen += 3;
    		}
    	}
	 }

    //clear digits in front
    for ( uint8_t l = 0 ; l < type ; l++ ) {
    	lcdp->commandWrite( ystart[l + y_offset] + x_offset);
    	//TODO boundary check!
    	blanks( length - strlen );
    }

    //now set x_offset to x_offset + blanks!
    x_offset += length - strlen;
	for ( uint8_t i = 0 ; i < strlen ; i++ ) {
		if ( i == dp_pos ) {
			//TODO print bigger decimal point!
			lcdp->print(5);
			if ( type == LCD_BIGFONT_2 )
				x_offset++;
		} else {
			if ( type == LCD_BIGFONT_2 ) {
				printOneNumber2(value_i[i], x_offset, y_offset );
				x_offset += 3;
			} else {
				printOneNumber4(value_i[i], 0, x_offset);
				x_offset += 4;
			}
		}
	}
}

