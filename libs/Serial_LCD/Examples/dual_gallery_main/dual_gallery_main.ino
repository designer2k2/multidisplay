///
/// @mainpage	Dual Gallery with SoftwareSerial
/// @details	<#details#>
/// @n
/// @n
/// @n @a	Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author	Rei VILO
/// @author	Rei VILO
/// @date	29/11/12 11:22
/// @version	<#version#>
///
/// @copyright	© Rei VILO, 2012
/// @copyright	CC = BY NC SA
///
/// @see	ReadMe.txt for references
///


///
/// @file	dual_gallery.ino
/// @brief	Main sketch
/// @details	<#details#>
/// @n @a	Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author	Rei VILO
/// @author	Rei VILO
/// @date	29/11/12 11:22
/// @version	<#version#>
///
/// @copyright	© Rei VILO, 2012
/// @copyright	CC = BY NC SA
///
/// @see	ReadMe.txt for references
/// @n
///


///
/// @brief	Software port flag
/// @note	Uncomment for SoftwareSerial port, comment for HardwareSerial port
///
//#define SOFTWARE_PORT


// Core library for code-sense
#include "Arduino.h"

// Include application, user and local libraries
#include "proxySerial.h"
#include "Serial_LCD.h"
#include "GUI.h"
#include "Gallery.h"
#include "Wire.h"

#ifdef SOFTWARE_PORT
// Software ports
#include "SoftwareSerial.h"
#else
// Hardware ports
#endif

// Define variables and constants
#ifdef SOFTWARE_PORT
// Software ports
SoftwareSerial mySerial1(10, 11); // mega2560=(10, 11) Uno=(2, 3)
SoftwareSerial mySerial2(12, 13); // mega2560=(12, 13) Uno=(5, 6)
#else
// Hardware ports
#define mySerial1 Serial1
#define mySerial2 Serial2
#endif

ProxySerial myPort1(&mySerial1);
ProxySerial myPort2(&mySerial2);

Serial_LCD myLCD1(&myPort1);
Serial_LCD myLCD2(&myPort2);

button myButton;

Gallery myGallery1;
Gallery myGallery2;

uint8_t a = 0;
uint16_t x, y;
boolean flag1 = true;
boolean flag2 = true;


int availableMemory() {
    extern int  __bss_end;
    extern int  *__brkval;
    int free_memory;
    if((int)__brkval == 0) {
        free_memory = ((int)&free_memory) - ((int)&__bss_end);
    } else {
        free_memory = ((int)&free_memory) - ((int)__brkval);
    }
    return free_memory;
}


// Add setup code
void setup() {
    
    Serial.begin(9600);
    Serial.println("\n\n\n");
#ifdef SOFTWARE_PORT
    Serial.println("*** Dual gallery with software serial ports");
#else
    Serial.println("*** Dual gallery with hardware serial ports");
#endif
    
    Serial.print("> ");
    Serial.println(availableMemory(), DEC);
    
    Serial.println();
    Serial.println("Serial1.begin");
    mySerial1.begin(9600);
    
    Serial.print("> ");
    Serial.println(availableMemory(), DEC);
    
    Serial.println("Serial2.begin");
    mySerial2.begin(9600);
    
    Serial.print("> ");
    Serial.println(availableMemory(), DEC);
    
    if (flag1) {
    Serial.print("myLCD1.begin... ");
#ifdef SOFTWARE_PORT
    mySerial1.listen();
#endif
    myLCD1.begin(4);  // mega2560=(4) Uno=(4)
    Serial.println("done");
    
    Serial.print("> ");
    Serial.println(availableMemory(), DEC);
    }
    
    if (flag2) {
    Serial.print("myLCD2.begin... ");
#ifdef SOFTWARE_PORT
    mySerial2.listen();
#endif
    myLCD2.begin(7); // mega2560=(7) Uno=(7)
    Serial.println("done");
    
    Serial.print("> ");
    Serial.println(availableMemory(), DEC);
    }
    
    if (flag1) {
    Serial.print("myLCD1.gText... ");
#ifdef SOFTWARE_PORT
    mySerial1.listen();
#endif
    a = myLCD1.gText(100, 100, "screen 1", redColour, 2, 2);
    Serial.println(a, HEX);
    
    Serial.print("> ");
    Serial.println(availableMemory(), DEC);
    }
    
    if (flag2) {
    Serial.print("myLCD2.gText... ");
#ifdef SOFTWARE_PORT
    mySerial2.listen();
#endif
    a = myLCD2.gText(100, 100, "screen 2", greenColour, 2, 2);
    Serial.println(a, HEX);
    }
    
    // Gallery1 on screen1
    
    Serial.print("> ");
    Serial.println(availableMemory(), DEC);
    
    if (flag1) {
    Serial.println();
#ifdef SOFTWARE_PORT
    mySerial1.listen();
#endif
    Serial.print("myLCD1.readScreenGCI initSD... ");
    a =   myLCD1.initSD();
    Serial.println(a, HEX);
    
    Serial.print("myLCD1.readScreenGCI checkSD... ");
    a =   myLCD1.checkSD();
    Serial.println(a, HEX);
    
    Serial.print("myLCD1.readScreenGCI checkScreenType... ");
    a =   myLCD1.checkScreenType();
    Serial.println(a, HEX);
    
    Serial.print("myLCD1.readScreenGCI WhoAmI... ");
    myLCD1.gText( 0, 210, myLCD1.WhoAmI());
    Serial.println(myLCD1.WhoAmI());
    delay(500);
    
    Serial.print("> ");
    Serial.println(availableMemory(), DEC);
    
    Serial.print("myLCD1.readScreenGCI images1.gci... ");
    a = myLCD1.findFile("images1.gci");
    Serial.println(a, HEX);
    
    Serial.print("myLCD1.readScreenGCI images1.dat... ");
    a = myLCD1.findFile("images1.dat");
    Serial.println(a, HEX);
    
    Serial.print("myGallery1.begin... ");
    a = myGallery1.begin(&myLCD1, "images1");
    Serial.print(a, DEC);
    Serial.println(" images");
    
    Serial.print("> ");
    Serial.println(availableMemory(), DEC);
    
    if ( a==0 ) {
        Serial.println("myGallery1: no images available");
        flag1 = false;
    } else {
        Serial.print("myGallery1.showImage()... ");
        a = myGallery1.showImage();
        Serial.println(a, HEX);
    }
    }
    
    // Gallery2 on screen2
    if (flag2) {

    Serial.print("> ");
    Serial.println(availableMemory(), DEC);
    
    Serial.println();
#ifdef SOFTWARE_PORT
    mySerial2.listen();
#endif
    Serial.print("myLCD2.readScreenGCI initSD... ");
    a =   myLCD2.initSD();
    Serial.println(a, HEX);
    
    Serial.print("myLCD2.readScreenGCI checkSD... ");
    a =   myLCD2.checkSD();
    Serial.println(a, HEX);
    
    Serial.print("myLCD2.readScreenGCI checkScreenType... ");
    a =   myLCD2.checkScreenType();
    Serial.println(a, HEX);
    
    Serial.print("myLCD2.readScreenGCI WhoAmI... ");
    myLCD2.gText( 0, 210, myLCD2.WhoAmI());
    Serial.println(myLCD2.WhoAmI());
    delay(500);
    
    Serial.print("> ");
    Serial.println(availableMemory(), DEC);
    
    Serial.print("myLCD2.readScreenGCI images2.gci... ");
    a = myLCD2.findFile("images2.Gci");
    Serial.println(a, HEX);
    
    Serial.print("myLCD2.readScreenGCI images2.dat... ");
    a = myLCD2.findFile("images2.Dat");
    Serial.println(a, HEX);
    
    Serial.print("myGallery2.begin... ");
    a = myGallery2.begin(&myLCD2, "images2");
    Serial.print(a, DEC);
    Serial.println(" images");
    
    Serial.print("> ");
    Serial.println(availableMemory(), DEC);
        
    if ( a==0 ) {
        Serial.println("myGallery2: no images available");
        flag2 = false;
    } else {
        Serial.print("myGallery2.showImage()... ");
        a = myGallery2.showImage();
        Serial.println(a, HEX);
    }
    }
    
    // Activate touch
    
    Serial.print("> ");
    Serial.println(availableMemory(), DEC);
    
    if (flag1) {
    Serial.println();
    Serial.print("myLCD1.setTouch... ");
#ifdef SOFTWARE_PORT
    mySerial1.listen();
#endif
    a = myLCD1.setTouch(true);
    Serial.println(a, HEX);
    
    Serial.print("> ");
    Serial.println(availableMemory(), DEC);
    
//    myButton.dStringDefine(&myLCD1, myLCD1.maxX()-60, myLCD1.maxY()-40, 60, 40, "Quit", whiteColour, redColour);
//    myButton.enable();
//    myButton.draw();
    
    Serial.print("> ");
    Serial.println(availableMemory(), DEC);
    
    Serial.print("mySerial1.available... ");
    Serial.println(mySerial1.available(), DEC);
    }
    
    if (flag2) {
    Serial.print("myLCD2.setTouch... ");
#ifdef SOFTWARE_PORT
    mySerial2.listen();
#endif
    a = myLCD2.setTouch(true);
    Serial.println(a, HEX);
    
    Serial.print("> ");
    Serial.println(availableMemory(), DEC);
    
    Serial.print("mySerial2.available... ");
    Serial.println(mySerial2.available(), DEC);
    }
}


// Add loop code
void loop() {
    Serial.print("> ");
    Serial.println(availableMemory(), DEC);
    
    Serial.println();
    // Touch acquisition sequence
    
    if (flag1) {
#ifdef SOFTWARE_PORT
        mySerial1.listen();
#endif
        Serial.print("mySerial1.available... ");
        Serial.println(mySerial1.available(), DEC);
        if (myLCD1.getTouchActivity()) {
            Serial.println("myLCD1.getTouchActivity");
            a = myLCD1.gText(120, 100, "end", greenColour, 2, 2);
            Serial.println("*** myLCD1.off");
            myLCD1.off();
            flag1 = false;
        }
    }
    
    if (flag2) {
#ifdef SOFTWARE_PORT
        mySerial2.listen();
#endif
        Serial.print("mySerial2.available... ");
        Serial.println(mySerial2.available(), DEC);
        if (myLCD2.getTouchActivity()) {
            Serial.println("myLCD2.getTouchActivity");
            a = myLCD2.gText(120, 100, "end", redColour, 2, 2);
            Serial.println("*** myLCD2.off");
            myLCD2.off();
            flag2 = false;
        }
    }
    
    // End of dual gallery
    
    if ((flag1==false) && (flag2==false)) {
        Serial.println("*** End of dual gallery");
        while (true);
    }
    
    
    delay(2000);
    
    
    // Image display sequence
    
    if (flag1) {
#ifdef SOFTWARE_PORT
        mySerial1.listen();
#endif
        Serial.print("myGallery1.showNext(");
        a = myGallery1.showNext();
        Serial.print(myGallery1.index(), DEC);
        Serial.print(")... ");
        Serial.println(a, HEX);
    }
    
    if (flag2) {
#ifdef SOFTWARE_PORT
        mySerial2.listen();
#endif
        Serial.print("myGallery2.showNext(");
        a = myGallery2.showNext();
        Serial.print(myGallery2.index(), DEC);
        Serial.print(")... ");
        Serial.println(a, HEX);
    }
    
}

