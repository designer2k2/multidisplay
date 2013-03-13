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
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <avr/pgmspace.h>
#include <Arduino.h>

#include "MultidisplayDefines.h"
#include "MultidisplayController.h"
#include "LCDController.h"


#ifdef LCD

const uint8_t	LCDController::ystart[] = { 0x80, 0xC0, 0x94, 0xD4 };

LCDController::LCDController() {
}

void  LCDController::myconstructor() {
//	activeScreen = SCREENCOUNT-1;
	activeScreen = 0;
	brightness = 2;

//	lcdp = new LiquidCrystal(4,2, 6,7,8,9);
//	lcd.begin(20,4);

	cbuf = (char*) malloc (sizeof(char) * LCD_BUFSIZE);

	lcd.init();

    for ( int i = 0 ; i < 20 ; i++ )
            scopeInt[i]=0;
}

void LCDController::init() {
	lcd.clear();
	switch (activeScreen) {
	case 0:
		screen1_init();
		break;
	case 1:
		screen2_init();
		break;
	case 2:
		screen3_init();
		break;
	case 3:
		screen4_init();
		break;
	case 4:
		screen5_init();
		break;
	}
}

void LCDController::draw() {
	switch (activeScreen) {
	case 0:
		screen1_draw();
		break;
	case 1:
		screen2_draw();
		break;
	case 2:
		screen3_draw();
		break;
	case 3:
		screen4_draw();
		break;
	case 4:
		screen5_draw();
		break;
	}
}

//The LCD brightness, what could be cycled through
#ifdef MULTIDISPLAY_V2
const uint8_t LCDController::LCDBrightnessD[]= {
   255,125,50        //Change this for other settings depending on what you like
 };
#else
const uint8_t LCDController::LCDBrightnessD[]= {
   50,125,255        //Change this for other settings depending on what you like
 };
#endif

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

void LCDController::toggleScreen () {
	// 9 screens atm -> 0..8!
	if ( activeScreen == SCREENCOUNT-1 )
		activeScreen = 0;
	else
		activeScreen++;
	init();
}

void LCDController::setActiveScreen(uint8_t as) {
	if ( as >= 0 && as < SCREENCOUNT )
		activeScreen = as;
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




//255 is the "Full Block" Code, and 254 is blank
const uint8_t LCDController::bigfont2data1[] = {31,31,31,0,0,0,0,0};    //Small Block on top
const uint8_t LCDController::bigfont2data2[] = {0,0,0,0,0,31,31,31};    //Small Block on bottom
const uint8_t LCDController::bigfont2data3[] = {31,31,31,0,0,0,31,31};  //Small Block on top and bottom
// const prog_uint8_t bigfont2data4[] = {28,30,31,31,31,31,30,28};  //Full Block roundet right
// const prog_uint8_t bigfont2data5[] = {7,15,31,31,31,31,15,7};  //Full Block roundet left
const uint8_t LCDController::bigfont2data6[] = {0,0,0,14,14,14,12,8};  //Dot, for Decimal.
const uint8_t LCDController::bigfont2data8[] = {0,0,0,0,0,0,31,31};  //minus sign

//Bigfont Code
//
//BIG parts from http://opengauge.googlecode.com/svn/trunk/mpguino/mpguino.pde

/**
 * load custom fonts for 2 line bigfonts
 */
void LCDController::cgramBigFont2() {
	lcdUploadUdef5x8_P (1,bigfont2data1);
	lcdUploadUdef5x8_P (2,bigfont2data2);
	lcdUploadUdef5x8_P (3,bigfont2data3);
	// LcdUploadUdef5x8_P (4,data4);
	// LcdUploadUdef5x8_P (5,data5);
	lcdUploadUdef5x8_P (6,bigfont2data6);
	lcdUploadUdef5x8_P (8,bigfont2data8);
}
//-------------------------------------------------------------------------------------------------------

//255 is the "Full Block" Code, and 254 is blank
const uint8_t LCDController::bigfont4data1[] = {0,0,0,0,3,15,15,31};    //Small Block on bottom left
const uint8_t LCDController::bigfont4data2[] = {0,0,0,0,31,31,31,31};    //Small Block on bottom right
const uint8_t LCDController::bigfont4data3[] = {0,0,0,0,24,30,30,31};  //Small Block on bottom full
const uint8_t LCDController::bigfont4data4[] = {31,15,15,3,0,0,0,0};  //Small Block on top left
const uint8_t LCDController::bigfont4data5[] = {31,30,30,24,0,0,0,0};  //Small Block on top right
const uint8_t LCDController::bigfont4data6[] = {31,31,31,31,0,0,0,0};  //Small Block on top full
const uint8_t LCDController::bigfont4data7[] = {14,14,14,14,12,8,0,0};  //Dot, for Decimal.
const uint8_t LCDController::bigfont4data8[] = {0,0,0,0,0,0,31,31};  //minus sign


/**
 * load custom fonts for 4 line bigfonts
 */
void LCDController::cgramBigFont4() {

	lcdUploadUdef5x8_P (1,bigfont4data1);
	lcdUploadUdef5x8_P (2,bigfont4data2);
	lcdUploadUdef5x8_P (3,bigfont4data3);
	lcdUploadUdef5x8_P (4,bigfont4data4);
	lcdUploadUdef5x8_P (5,bigfont4data5);
	lcdUploadUdef5x8_P (6,bigfont4data6);
	lcdUploadUdef5x8_P (7,bigfont4data7);
	lcdUploadUdef5x8_P (8,bigfont4data8);
}

/**
 * prints a 2 line number
 * @param digit
 * @param x_offset 1 unit is one lcd char, not one big char!
 * @param y_offset lcd lines!
 */
void LCDController::printOneNumber2(uint8_t digit, uint8_t x_offset, uint8_t y_offset) {
	// Line 1 of the one digit number
	lcd.commandWrite( ystart[y_offset] + x_offset); //Line1

	lcd.print (bn12[digit*3+digit*1]);
	lcd.print (bn12[digit*3+1+digit*1]);
	lcd.print (bn12[digit*3+2+digit*1]);

	// Line 2 of the one-digit number
	lcd.commandWrite(ystart[y_offset+1] + x_offset); // Line 2
	lcd.print(bn22[digit*3+digit*1]);
	lcd.print(bn22[digit*3+1+digit*1]);
	lcd.print(bn22[digit*3+2+digit*1]);
}

//-------------------------------------------------------------------------------------------------------

/**
 * prints a 4 line number
 * @param digit
 * @param x_offset 1 unit is one lcd char, not one big char!
 * @param y_offset lcd lines!
 */
void LCDController::printOneNumber4(uint8_t digit, uint8_t x_offset, uint8_t y_offset) {
	//TODO do we need the 4. col in the bnx4 arrays?

	// Line 1 of the one digit number
	lcd.commandWrite( ystart[y_offset] + x_offset);                  //Line 1
	lcd.print(bn14[digit*4]);
	lcd.print(bn14[digit*4+1]);
	lcd.print(bn14[digit*4+2]);

	// Line 2 of the one-digit number
	lcd.commandWrite( ystart[y_offset+1] + x_offset);
	lcd.print(bn24[digit*4]);
	lcd.print(bn24[digit*4+1]);
	lcd.print(bn24[digit*4+2]);

	// Line 3 of the one digit number
	lcd.commandWrite( ystart[y_offset+2] + x_offset);
	lcd.print(bn34[digit*4]);
	lcd.print(bn34[digit*4+1]);
	lcd.print(bn34[digit*4+2]);

	// Line 4 of the one-digit number
	lcd.commandWrite( ystart[y_offset+3] + x_offset);
	lcd.print(bn44[digit*4]);
	lcd.print(bn44[digit*4+1]);
	lcd.print(bn44[digit*4+2]);
}

//-------------------------------------------------------------------------------------------------------

/**
 * prints blanks
 * \param c count of blanks to be printed
 */
void LCDController::blanks(uint8_t c) {
	for ( uint8_t i = 0 ; i < c ; i++ )
		lcd.print(254);
}


/**
 * converts a float to a string
 * \param buffer storage for the string
 * \param f float val to convert
 * \param dp factor for the decimal places: 10^x for x decimal places, e.g. 10^2 = 100 to get 2 decimal places
 */
void LCDController::float2string ( char* buffer, float f, int dp ) {
	int v = 0;
	if ( f >= 0 )
		v = floor (f);
	else
		v = ceil (f);

	float t = (f-v) * dp;
	int ni = (int) t;

	bool makenegative = false;
	if ( ni < 0) {
		ni = abs (ni);
		if ( v == 0 )
			makenegative = true;
	}

	uint8_t addnull = 0;
	for ( int i = 10 ; i <= dp/10 ; i *= 10 ) {
		if (ni < i)
			addnull++;
	}

	char a[15];
	char b[15];
	if ( makenegative ) {
		a[0] = '-';
		itoa(v, &(a[1]), 10);
	} else
		itoa(v, &(a[0]), 10);

	for ( uint8_t j = 0 ; j < addnull ; j++  )
		b[j]='0';
	itoa(ni, &(b[addnull]), 10);
	b[dp/10] = '\0';
	strcpy (buffer, &a[0]);
	strcat (buffer, ".");
	strcat (buffer, &b[0]);
}

/**
 * prints a float value with 2 decimal places
 */
void LCDController::printFloat2DP(float f) {
	printFloat (f, 100);
}

/**
 * prints a float value with configurable decimal places
 * \param f value to print
 * \param dp factor for the decimal places: 10^x for x decimal places, e.g. 10^2 = 100 to get 2 decimal places
 */
void LCDController::printFloat(float f, uint16_t dp) {
	float2string ( cbuf, f, dp );
	lcd.printIn(cbuf);
}

/**
 * prints a float value with 2 decimal places to position pos
 */
void LCDController::printFloat2DP(uint8_t pos, float f) {
	printFloat (pos, f, 100);
}

/**
 * prints a float value with configurable decimal places to position pos
 * \param pos position
 * \param f value to print
 * \param dp factor for the decimal places: 10^x for x decimal places, e.g. 10^2 = 100 to get 2 decimal places
 */
void LCDController::printFloat(uint8_t pos, float f, uint16_t dp ) {
	lcd.commandWrite(pos);
	printFloat (f, dp);
}


const unsigned char LCDController::idata1[] = {28,28,28,30,30,31,31,31};  // 1--- k
const unsigned char LCDController::idata2[] = {7,7,7,15,15,31,31,31};     // -1-- k
const uint8_t LCDController::idata3[] PROGMEM = {31,31,31,28,28,28,28,28};  // --1- k
const uint8_t PROGMEM LCDController::idata4[] = {24,28,30,15,7,7,7,7};      // ---1 k

void LCDController::cgramIntro() {
	lcdUploadUdef5x8_P (1,idata1);
	lcdUploadUdef5x8_P (2,idata2);
	lcdUploadUdef5x8_P (3,idata3);
	lcdUploadUdef5x8_P (4,idata4);
}

const unsigned char LCDController::i2data1[] = {31,29,29,28,28,28,28,28};  // 2--- k
const unsigned char LCDController::i2data2[] = {31,23,23,7,7,7,7,7};       // -2-- k
const unsigned char LCDController::i2data3[] = {28,28,28,28,28,31,31,31};  // --2- k
const unsigned char LCDController::i2data4[] = {7,7,7,7,15,30,28,24};      // ---2 k
void LCDController::cgramIntro2() {

	lcdUploadUdef5x8_P (5,i2data1);
	lcdUploadUdef5x8_P (6,i2data2);
	lcdUploadUdef5x8_P (7,i2data3);
	lcdUploadUdef5x8_P (8,i2data4);
}


/**
 * shows the intro animation on the lcd
 */
void LCDController::lcdShowIntro(int delayValue) {

	//Must load this in 2 steps, somehow its not possible to load 8 chars at once...
	cgramIntro();
	cgramIntro2();

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
	lcd.printIn_P ( PSTR("ulti") );
	lcd.commandWrite(0xC0+10); 			//Line2
	lcd.print(7);
	lcd.print(8);
	lcd.printIn_P ( PSTR("isplay") );

	lcd.commandWrite(0xD4+1);                  //Line=4
	lcd.printIn_P ( PSTR("www.designer2k2.at") );

	//Set the LCD brightness: (This turns on the Light, looks nice)
	setBrightness(2);

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
		lcd.commandWrite(0x40 | charIndex | i);
		lcd.print( pgm_read_byte (data+i) );
	}
}

/**
 *
 * Upload a user-defined character stored in ram to character generator RAM of LCD (5x8)
 */
void LCDController::lcdUploadUdef5x8(uint8_t charIndex, const uint8_t *data) {
	charIndex = charIndex << 3;
	for (uint8_t i = 0; i < 8; i++) {
		lcd.commandWrite(0x40 | charIndex | i);
		lcd.print( data[i] );
	}
}


const uint8_t LCDController::dotdata1[] = {31,0,0,0,0,0,0,31};        //only 2 dots
const uint8_t LCDController::dotdata2[] = {31,0,0,14,0,0,0,31};       //the 2 dots with a minus
const uint8_t LCDController::dotdata3[] = {31,0,4,14,4,0,0,31};       //the 2 dots with a plus
const uint8_t LCDController::dotdata4[] = {31,14,31,31,31,31,14,31};  //thats a block with little rounded edges

/**
 * uploads dots to cgram
 * ATTENTION: this arent dots but rather 2 bars (at top and bottom) plus minus, plus etc
 */
void LCDController::cgramDot() {

	lcdUploadUdef5x8_P (5,dotdata1);
	lcdUploadUdef5x8_P (6,dotdata2);
	lcdUploadUdef5x8_P (7,dotdata3);
	lcdUploadUdef5x8_P (8,dotdata4);
}
//-------------------------------------------------------------------------------------------------------

const uint8_t LCDController::bardata1[] = {16,16,16,16,16,16,16,16};  //1 line
const uint8_t LCDController::bardata2[] = {24,24,24,24,24,24,24,24};  //2 line
const uint8_t LCDController::bardata3[] = {28,28,28,28,28,28,28,28};  //3 line
const uint8_t LCDController::bardata4[] = {30,30,30,30,30,30,30,30};  //4 line

/**
 * uploads bar chars to cgram
 */
void LCDController::cgramBar() {
	lcdUploadUdef5x8_P (1,bardata1);
	lcdUploadUdef5x8_P (2,bardata2);
	lcdUploadUdef5x8_P (3,bardata3);
	lcdUploadUdef5x8_P (4,bardata4);
}


/**
 * draws a bar to the lcd
 * @param Digits number of chars the bar has on the lcd screen
 * @param Value the actual value. 0 <= value <= 10 x Digits
 */
void LCDController::drawBar(uint8_t digits, uint8_t value) {

	uint8_t Curs = value/10;
	for (uint8_t i=0; i < digits; i++) {
		if ( i<Curs ) {
			lcd.print(255);
		} else {

			if(i==Curs) {
				//in val ist 84 und in val5 die 8
				//ich brauch die 4 /2 als int
				int ChrW = (value - Curs*10)/2;

				if(ChrW==0) {
					lcd.printIn(" ");
				} else {
					lcd.print(ChrW);
				}
			} else {
				lcd.printIn(" ");
			}
		}
	}
}

const uint8_t LCDController::vbardata1[] = {0,0,0,0,0,0,0,31};              //1 row
const uint8_t LCDController::vbardata2[] = {0,0,0,0,0,0,31,31};             //2 rows
const uint8_t LCDController::vbardata3[] = {0,0,0,0,0,31,31,31};            //3 rows
const uint8_t LCDController::vbardata4[] = {0,0,0,0,31,31,31,31};           //4 rows
const uint8_t LCDController::vbardata5[] = {0,0,0,31,31,31,31,31};          //5 rows
const uint8_t LCDController::vbardata6[] = {0,0,31,31,31,31,31,31};         //6 rows
const uint8_t LCDController::vbardata7[] = {0,31,31,31,31,31,31,31};        //7 rows

/**
 * uploads vertical bars to cgram
 */
void LCDController::cgramVertBar() {
	lcdUploadUdef5x8_P (1,vbardata1);
	lcdUploadUdef5x8_P (2,vbardata2);
	lcdUploadUdef5x8_P (3,vbardata3);
	lcdUploadUdef5x8_P (4,vbardata4);
	lcdUploadUdef5x8_P (5,vbardata5);
	lcdUploadUdef5x8_P (6,vbardata6);
	lcdUploadUdef5x8_P (7,vbardata7);
}
//-------------------------------------------------------------------------------------------------------

/**
 * draws a vertical bar
 * @param val must be between 0 and 32 (4x8)
 * @param pos must be between 0 and 20 (on a 20 char disp)
 */
void LCDController::drawVertBar(uint8_t val, uint8_t pos) {

	//whats the modulo with 8 ?
	uint8_t mod = val % 8;
	//This is directly the code for the top character!
	//But at what pos it must be printed?

	uint8_t printPos = (val - mod) / 8;

	//But if Mod = 0, the LCD nothing is 254.

	if(mod == 0)
		mod=254;

//	for ( uint8_t i = 3 ; i > 0 ; i-- ) {
//		//blank
//		uint8_t  draw = 254;
//		if (printPos == i)
//			draw = mod;
//		else if ( i < printPos )
//			draw = 255;
//		//now draw it!
//		lcd.commandWrite( ystart[i] + pos);
//		lcd.print( draw );
//	}

	//now lets make a switch case depending on the PrintPos.
	switch(printPos){
	case 0:
		//so there is only one character /3 blanks and the character in the lowest position
		lcd.commandWrite(0x80+pos);              //Line 1
		lcd.print(254); //Blank
		lcd.commandWrite(0xC0+pos);              //Line 2
		lcd.print(254); //Blank
		lcd.commandWrite(0x94+pos);              //Line 3
		lcd.print(254); //Blank
		lcd.commandWrite(0xD4+pos);              //Line 4
		lcd.print(mod); //The Custom Character presenting the value
		break;
	case 1:
		//so there is only one character /1 fulls and the character in the 2nd position
		lcd.commandWrite(0x80+pos);              //Line 1
		lcd.print(254); //Blank
		lcd.commandWrite(0xC0+pos);              //Line 2
		lcd.print(254); //Blank
		lcd.commandWrite(0x94+pos);              //Line 3
		lcd.print(mod); //The Custom Character presenting the value
		lcd.commandWrite(0xD4+pos);              //Line 4
		lcd.print(255); //Full
		break;
	case 2:
		//so there is only one character /2 fulls and the character in the 3rd position
		lcd.commandWrite(0x80+pos);              //Line 1
		lcd.print(254); //Blank
		lcd.commandWrite(0xC0+pos);              //Line 2
		lcd.print(mod); //The Custom Character presenting the value
		lcd.commandWrite(0x94+pos);              //Line 3
		lcd.print(255); //Full
		lcd.commandWrite(0xD4+pos);              //Line 4
		lcd.print(255); //Full
		break;
	case 3:
		//so there is only one character /3 fulls and the character in the highest position
		lcd.commandWrite(0x80+pos);              //Line 1
		lcd.print(mod); //The Custom Character presenting the value
		lcd.commandWrite(0xC0+pos);              //Line 2
		lcd.print(255); //Full
		lcd.commandWrite(0x94+pos);              //Line 3
		lcd.print(255); //Full
		lcd.commandWrite(0xD4+pos);              //Line 4
		lcd.print(255); //Full
		break;
	case 4:
		//easy, eveything is full!
		lcd.commandWrite(0x80+pos);              //Line 1
		lcd.print(255); //Full
		lcd.commandWrite(0xC0+pos);              //Line 2
		lcd.print(255); //Full
		lcd.commandWrite(0x94+pos);              //Line 3
		lcd.print(255); //Full
		lcd.commandWrite(0xD4+pos);              //Line 4
		lcd.print(255); //Full
		break;
	}

	//thats it, easy :)

}


/**
 * scope mode drawing method
 */
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
	itoa (value, cbuf, 10);
	printBigNum ( cbuf, length, x_offset, y_offset, type );
}

void LCDController::printBigNum (double value, uint8_t length, uint8_t x_offset, uint8_t y_offset, uint8_t type) {
	float2string ( cbuf, value, 100 );
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
	bool negative = false;

	//string length
	uint8_t stringlen = 0;
	//length of the string on the lcd screen (one char needs 3 or 4 positions on screen!)
	uint8_t screenlen = 0;
	uint8_t end = x_offset + length;

	//compute array of uint8_t out of the char array
	uint8_t value_i[21];
    char c[2];
    c[1] = '\0';
    for ( uint8_t i = 0 ; i < strlen(str) ; i++ ) {
    	c[0] = str[i];
    	if ( i == 0 && str[i] == '-') {
    		//we are negative
    		negative = true;
    		stringlen++;
    		screenlen++;
    	}
    	if (c[0]=='.' || c[0]==';' || c[0]==',') {
    		value_i[i]=c[0];
    		dp_pos = i;
   			stringlen++;
    	} else {
    		// 0-9 in ascii table
    		if ( c[0] >= 48 && c[0] <= 57 ) {
    			value_i[i] = atoi(&c[0]);
   				stringlen++;
   				screenlen += 4;
    		}
    	}
	 }

    //clear digits in front
    if ( length - screenlen > 0 ) {
    	for ( uint8_t l = 0 ; l < type ; l++ ) {
    		lcd.commandWrite( ystart[l + y_offset] + x_offset);
    		blanks( length - stringlen );
    	}
    }

    //now set x_offset to x_offset + blanks!
    if ( length - screenlen > 0 )
    	x_offset += length - screenlen;

	for ( uint8_t i = 0 ; i < stringlen ; i++ ) {
		if ( i == 0 && negative ) {
			if ( type == LCD_BIGFONT_4 )
				lcd.commandWrite( ystart[y_offset+1] + x_offset);
			else
				lcd.commandWrite( ystart[y_offset] + x_offset);
			lcd.print(8);
			x_offset++;
		} else {
			if ( i == dp_pos ) {
				//TODO print bigger decimal point!
				if ( type == LCD_BIGFONT_4 )
					lcd.print (255);
				else
					lcd.print(5);
			} else {
				//draw only of we have space
				if ( ! ((x_offset + 4) > end) ) {
					if ( type == LCD_BIGFONT_2 )
						printOneNumber2(value_i[i], x_offset, y_offset );
					else
						printOneNumber4(value_i[i], x_offset, y_offset );
					x_offset += 4;
				}
			}
		}
	}
}

/**
 * prints a integer value to postion pos
 * blanks the resulting places if length is > 0 and strlen(value) < length
 */
void LCDController::printInt (uint8_t pos, int value, uint8_t length) {
	itoa (value, cbuf, 10);
	uint8_t b = length - strlen ( cbuf );
	printString (pos, cbuf);
	if ( b > 0 && length > 0 )
		blanks(b);
}

/**
 * prints a integer value to postion pos
 */
//void LCDController::printt (uint8_t pos, int value) {
//	itoa (value, cbuf, 10);
//	printString (pos, cbuf);
//}
/**
 * print a string to position pos
 */
void LCDController::printString (uint8_t pos, char* str) {
	if (pos >= 0x80)
		lcd.commandWrite (pos);
	lcd.printIn (str);
}

void LCDController::screen1_init() {
    lcd.lcdCommandWriteAndPrintIn_P (0x80, PSTR("L  :"));
    lcd.lcdCommandWriteAndPrintIn_P (0x80+9, PSTR("AGT:"));
    lcd.lcdCommandWriteAndPrintIn_P (0xC0, PSTR("BST:"));
    lcd.lcdCommandWriteAndPrintIn_P (0xC0+9, PSTR("P1 :"));
}


void LCDController::screen1_draw() {
    printFloat2DP(ystart[0] + 4, data.calLambdaF);
//    lcdController.printInt(lcdController.ystart[0] + 13, data.calThrottle, 3 );

    printFloat2DP(ystart[1] + 4, data.calBoost);
    printInt(ystart[1] + 13, data.VDOPres1, 4);
}

void LCDController::screen2_init() {
    lcd.lcdCommandWriteAndPrintIn_P(ystart[0], PSTR("gear:"));
    lcd.lcdCommandWriteAndPrintIn_P(ystart[0]+9, PSTR("speed:"));
    lcd.lcdCommandWriteAndPrintIn_P(ystart[1], PSTR("Digifant:"));
//    lcd.lcdCommandWriteAndPrintIn_P(lcdController.ystart[2] + 0, PSTR("ECT:"));
//    lcd.lcdCommandWriteAndPrintIn_P(lcdController.ystart[2] + 9, PSTR("Ign:"));
    lcd.lcdCommandWriteAndPrintIn_P(ystart[3] + 0, PSTR("LC :"));
}
void LCDController::screen2_draw() {
    printInt(0x80 + 6, data.gear, 1 );
    printInt(0x80 + 15, data.speed, 3 );
#ifdef DIGIFANT_KLINE
    printInt(ystart[2]+10, mController.df_kline_active_frame, 1);
    printInt(ystart[2]+15, mController.df_kline_freq_milliseconds, 2);
    printInt(ystart[3]+10, mController.df_kline_freq_helper0, 2);
    printInt(ystart[3]+17, mController.df_kline_discarded_frames, 2);
#endif
}

void LCDController::screen3_init() {
	cgramVertBar();
	scopeMin=0;
	scopeMax=5000;
}
void LCDController::screen3_draw() {
	//new Constrains if needed
	int ScopeVal = 0;

//	switch(flags.f.mode){
//	case SCOPE_BOOST:
//		ScopeVal = data.anaIn[BOOSTPIN];
//		break;
//	case SCOPE_THROTTLE:
//		ScopeVal = data.calThrottle;
//		break;
//	case SCOPE_LMM:
//		ScopeVal = data.calLMM;
//		break;
//	case SCOPE_RPM:
//		ScopeVal = data.calRPM;
//		break;
//	case SCOPE_LAMBDA:
//		ScopeVal = data.calLambda;
//		break;
//	default:
//		break;
//	}
	ScopeVal = data.calLambda;

//#ifdef LCDTEST
	ScopeVal = random (5000);
//#endif

	if(ScopeVal < scopeMin)    {
		scopeMin = ScopeVal;
	}
	if(ScopeVal > scopeMax)    {
		scopeMax = ScopeVal;
	}

	//Get the Data:
	int d = 0;
	d = map(ScopeVal, scopeMin, scopeMax, 0, 33);
	d = constrain(d, 0, 32);

	//Show only the rightmost bar: (live screen)
	drawVertBar(d,19);

	//Feed it into the Buffer (on the right side)
	scopeInt[19] = d;
	scopeMode();

	//Draw the Caption:
	//Line1
//	printInt(0x80, flags.f.mode);
//	lcd.printIn(":");
//	printInt (0, refreshCounter);

}

void LCDController::screen4_init() {
	cgramBigFont4();
	lcd.lcdCommandWriteAndPrintIn_P(0x80, PSTR("Boost:"));
	lcd.lcdCommandWriteAndPrintIn_P(0x94, PSTR("Max:"));
}


void LCDController::screen4_draw() {
	lcd.commandWrite(0xC0+7);
	if(data.calBoost < 0.0 ) {
		lcd.print(2);
		lcd.print(2);
	} else {
		lcd.printIn("  ");
	}

	//bigNum(abs(CalBoost)*1000,0,9,1);

	printBigNum ( (double)data.calBoost, LCD_WIDTH-9, 9, 0, LCD_BIGFONT_4);

	//Check for new MaxLD
	if(data.calBoost > data.maxLd) {
		data.maxLd  = data.calBoost;
	}

//	lcd2.commandWrite(0xD4);
//	lcdController.printFloat2DP(data.maxLd);                    //Max Boost
	lcdController.printFloat2DP(0xD4,data.maxLd);                    //Max Boost
 }

void LCDController::screen5_init() {
	cgramBar();
	//Line1
	lcd.lcdCommandWriteAndPrintIn_P (0x80, PSTR("LAM:"));
	//Line=2
//	lcd.lcdCommandWriteAndPrintIn_P(0xC0, PSTR("LD :"));
//	//Line=3
//	lcd.lcdCommandWriteAndPrintIn_P(0x94, PSTR("THR:"));
//	//Line=4
//	lcd.lcdCommandWriteAndPrintIn_P(0xD4, PSTR("RPM:"));

}
void LCDController::screen5_draw() {
#ifdef LCDTEST
	data.anaIn[BOOSTPIN] = random (4096);
	data.anaIn[RPMPIN] = random (RPMMAX);
	data.calLambda = random (200);
	data.calThrottle = random (100);
#endif

	//First, show the Bars on the last 10 Chars:

	int boostMapped  = map(data.anaIn[BOOSTPIN], 0, 4096, 0, 100);
	boostMapped = constrain(boostMapped, 0, 100);

	//Now lets make the Bars:
	lcd.commandWrite(0x80+10);
	lcdController.drawBar(10,boostMapped);
	lcd.commandWrite(0xC0+10);
	lcdController.drawBar(10,boostMapped);
}

//void LCDController::screen6_init() {
//	lcd.lcdCommandWriteAndPrintIn_P (0x80, PSTR("DEBUG"));
//}
//void LCDController::screen6_draw() {
//	printInt (ystart[0]+6, data.efr_speed_reading);
//	printFloat(ystart[1], data.efr_speed, 1);
//}
#endif /* LCD */
