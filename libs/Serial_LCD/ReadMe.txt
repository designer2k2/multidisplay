
	4D Systems uLCD-uLED-uVGA Serial_LCD Library Suite
	Arduino & chipKIT Library

	© Rei VILO, 2010-2013
	CC = BY NC SA Attribution-NonCommercial-ShareAlike
	http://creativecommons.org/licenses/by-nc-sa/3.0/

	For any enquiry about copyright and licence, 
	http://embeddedcomputing.weebly.com/contact


Links
	http://embeddedcomputing.weebly.com/serial-lcd.html
	including full documentation, examples, tutorials, code and downloads

For 
	4D Systems Goldelox and Picaso SGC Command Set
	http://www.4dsystems.com.au/


Serial_LCD
May 10, 2011 release   1	initial release
Jun 15, 2011 release   2	features added and bugs fixed
Jun 29, 2011 release   3	setBackGroundColour added and SD card
Jul 31, 2011 release   4	stdint.h types for chipKIT compatibility
Aug 07, 2011 release   5	playing sounds	up to 250 mA!
Oct 14, 2011 release   6	ellipse and detectTouchRegion from sebgiroux
Oct 27, 2011 release   7	setSpeed fixed for 155200 
Nov 29, 2011 release   8	read pixel colour and new colour functions
Dec 15, 2011 release 8.1	Arduino 1.0 implementation test	no longer compatible with 0022
Dec 17, 2011 release   9	max vertical and horizontal according to orientation
Dec 18, 2011 release  10	(x0, y0, dx, fy) functions added to (x1, x2, y1, y2) = (x0, y0, x0+dx, y0+dy)
Dec 29, 2011 release  11	readTextFile from sebgiroux
Jan 05, 2012 release  12	RAW for screen copy to/read from SD
Jan 11, 2012 release  13	VGA support and library release number check, updated examples
Jan 14, 2012 release  14	Goldelox support with 8-bits coordinates 
Jan 16, 2012 release  15	RAW partition automatic detection
Jan 20, 2012 release  16	self-generated screen parameters with example Info_main
Jan 24, 2012 release  17	uOLED 1.6"" and uLCD 4.3"" compatibility test successful
Jan 27, 2012 release  18	stability enhancement
Jan 28, 2012 release 118	new index
Jan 28, 2012 release 119	colours constants e.g. redColour
Feb 12, 2012 release 119a	__AVR_ATmega328P__ __AVR_ATmega2560__ added
Feb 04, 2012 release 120	stability enhancement
Feb 29, 2012 release 121	pin for reset fixed
Mar 13, 2012 release 122	enhancements for Gallery
Mar 19, 2012 release 223	Arduino 1.0 compatible
Apr 09, 2012 release 124	gText with multiplier for big text
Apr 09, 2012 release 224	gText with multiplier for big text
Apr 22, 2012 release 125	stability enhancement
Apr 24, 2012 release 126	stability enhancement for Gallery
Apr 25, 2012 release 127	4D Systems firmware 21
May 01, 2012 release 128	support for Wiring 1.0
May 23, 2012 release 129	graphic controller type based on screen size
Jun 04, 2012 release 130	RAW image bug fixed
Jun 14, 2012 release 131	unified release for Arduino 23 & 1.0, chipKIT and Wiring
Jul 08, 2012 release 132	more compact library
Jul 30, 2012 release 333	unified release numbering version 3xx
Aug 03, 2012 release 333a	revised examples based on Docedison's feed-back
Aug 21, 2012 release 334	__AVR_ATmega1280__ added
Sep 01, 2012 release 335	__AVR_ATmega32U4__ added  
Sep 19, 2012 release 336	stability enhancement
Dec 11, 2012 release 337	fixed playSoundSD - thanks bmeyer29
Jan 01, 2013 release 338	updated reference manual


proxySerial
Aug 04, 2011 release   1	chipKIT compatibility with external proxySerial.h
Sep 23, 2011 release   2	ms monitoring to avoid RX TX collapse
Oct 10, 2011 release   3	Stream.h class based i2cSerial library
Oct 24, 2011 release   4	serial port managed in main only, setSpeed added, proxySerial still needed
Nov 02, 2011 release   5	HardwareSerial derived from Stream on chipKIT platform by msproul
Dec 15, 2011 release 5.1	Arduino 1.0 implementation test	no longer compatible with 0022
Jan 28, 2012 release 105	new index
Feb 01, 2012 release 106	ttoa time to string added
Feb 12, 2012 release 106a	__AVR_ATmega328P__ __AVR_ATmega2560__ added
Mar 19, 2012 release 207	Arduino 1.0 compatible
Apr 25, 2012 release 107	4D Systems firmware 21
May 01, 2012 release 108	support for Wiring 1.0
Jun 14, 2012 release 109	unified release for Arduino 23 & 1.0, chipKIT and Wiring
Jul 08, 2012 release 110	more compact library
Jul 30, 2012 release 311	unified release numbering version 3xx
Aug 21, 2012 release 312	__AVR_ATmega1280__ added
Sep 01, 2012 release 313	__AVR_ATmega32U4__ added  
Nov 28, 2012 release 314	utoa renamed u64toa


button / GUI
Sep 18, 2011 release   1	dialog window with up to 3 buttons
Nov 25, 2011 release   2	faster dialog show/hide and optional area for screen copy to/read from SD
Nov 27, 2011 release   3	bugs fixed
Dec 15, 2011 release 3.1	Arduino 1.0 implementation test	no longer compatible with 0022
Dec 27, 2011 release   4	ready for GUI = button + dialog box + menu + label
Dec 28, 2011 release   5	item-based refactoring for dialog box, menu and label
Dec 29, 2011 release   6	button library deprecated, superseeded by GUI library
Jan 05, 2012 release   7	(x0, y0, dx, fy) functions added to (x1, x2, y1, y2) = (x0, y0, x0+dx, y0+dy)
Jan 25, 2012 release   8	button with instant option (no de-bouncing)
Jan 28, 2012 release 108	new index
Feb 12, 2012 release 108a	__AVR_ATmega328P__ __AVR_ATmega2560__ added
Mar 19, 2012 release 209	Arduino 1.0 compatible
Apr 22, 2012 release 109	slider added
Apr 28, 2012 release 110	better menu
May 01, 2012 release 111	support for Wiring 1.0
Jun 04, 2012 release 112	RAW image bug fixed
Jun 14, 2012 release 113	unified release for Arduino 23 & 1.0, chipKIT and Wiring
Jul 05, 2012 release 114	more compact library
Jul 30, 2012 release 315	unified release numbering version 3xx
Aug 21, 2012 release 316	__AVR_ATmega1280__ added
Sep 01, 2012 release 317	__AVR_ATmega32U4__ added  
Dec 01, 2012 release 318	new area object = zone for touch


Graphics
Jan 22, 2012 release   1	new Graphics library with example Graphics_main
Jan 25, 2012 release   2	(x0, y0, dx, fy) functions added to (x1, x2, y1, y2) = (x0, y0, x0+dx, y0+dy)
Jan 27, 2012 release   3	histogram graphic with example Histogram_main
Jan 28, 2012 release 103	new index
Jan 28, 2012 release 104	default colours for each graphic
Jan 30, 2012 release 105	improved consistency
Feb 01, 2012 release 106	ttoa used
Feb 12, 2012 release 106a	__AVR_ATmega328P__ added
Feb 14, 2012 release 107	gauge graphic
Feb 16, 2012 release 108	yaw, pitch, roll graphics
Mar 19, 2012 release 209	Arduino 1.0 compatible
May 01, 2012 release 109	support for Wiring 1.0
Jun 14, 2012 release 110	unified release for Arduino 23 & 1.0, chipKIT and Wiring
Jul 06, 2012 release 111	more compact library
Jul 09, 2012 release 112	graphic for multiple values
Jul 13, 2012 release 113	better dial for clock
Jul 30, 2012 release 314	unified release numbering version 3xx
Aug 21, 2012 release 315	__AVR_ATmega1280__ added
Sep 01, 2012 release 316	__AVR_ATmega32U4__ added  
Sep 12, 2012 release 317	Gauge stability enhancement


Gallery
Mar 13, 2012 release 101	initial release
Mar 19, 2012 release 202	Arduino 1.0 compatible
Apr 24, 2012 release 103	stability enhancement
May 01, 2012 release 104	support for Wiring 1.0
Jun 14, 2012 release 105	unified release for Arduino 23 & 1.0, chipKIT and Wiring
Jul 30, 2012 release 306	unified release numbering version 3xx
Aug 21, 2012 release 307	__AVR_ATmega1280__ added
Sep 01, 2012 release 308	__AVR_ATmega32U4__ added  
Sep 18, 2012 release 309	specific non-vector based table of pictures for chipKIT MPIDE
Sep 19, 2012 release 310	stability enhancement
Nov 29, 2012 release 311	array[MAXPICTURE] instead of vector - MAXPICTURE=64 by default


Energy
Apr 28, 2012 release 101	initial release
Apr 29, 2012 release 102	wake-up clears touch flag
May 01, 2012 release 103	support for Wiring 1.0
Jun 14, 2012 release 104	unified release for Arduino 23 & 1.0, chipKIT and Wiring
Jul 30, 2012 release 305	unified release numbering version 3xx
Aug 21, 2012 release 306	__AVR_ATmega1280__ added
Sep 01, 2012 release 307	__AVR_ATmega32U4__ added  


Full documentation with tutorial at
	http://embeddedcomputing.weebly.com/serial-lcd.html

Code at
	http://github.com/rei-vilo/Serial_LCD


Required
. Arduino IDE 0023
. NewSoftSerial release 11 beta
	http://arduiniana.org/2011/01/newsoftserial-11-beta/
. for Goldelox screens, hardware reset recommended with reset pin # (active low) be declared in myLCD.begin( # )
. I2C_Serial release 106


Bugs & ToDos
. RAW stability


Roadmap
	http://embeddedcomputing.weebly.com/serial-lcd.html
	

Serial Port Multiple Options

hardware : Arduino . Serial (pins 0 & 1)
         : chipKIT . Serial (pins 0 & 1) + Serial1 (pins 39 & 40)
software : Arduino . with library NewSoftSerial release 11
         : chipKIT . not available
i2c      : Arduino . (pins 4 & 5) with library I2C_Serial
         : chipKIT . (pins 4 & 5) with library I2C_Serial



SC16IS750 I2C slave bridge to serial
Arduino + chipKIT Library

I2C_Serial
Oct 06, 2011 release   1	initial release
Oct 10, 2011 release   2	Stream.h class based
Oct 25, 2011 release   3	I2C address selection through parameter default=0, 1, 2 or 12
Jan 28, 2012 release 103	new index
Mar 19, 2012 release 204	Arduino 1.0 compatible
Apr 19, 2012 release 104	I/O functions and reset
May 01, 2012 release 105	support for Wiring 1.0
Jun 04, 2012 release 106	unified release for Arduino 23, Arduino 1.0, chipKIT MPIDE and Wiring
Jul 30, 2012 release 306	unified release numbering version 3xx
Aug 21, 2012 release 307	__AVR_ATmega1280__ added
Sep 01, 2012 release 308	__AVR_ATmega32U4__ added  
Nov 29, 2012 release 309	Core library - IDE-based

