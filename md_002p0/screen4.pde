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

//Typ K Screen
void Screen4Init()
{
   lcd.init();
   
  lcd.commandWrite(0x80);                  //Line1
  lcd.printIn("T1:       T2:");
  lcd.commandWrite(0xC0);                  //Line=2
  lcd.printIn("T3:       T4:");
  lcd.commandWrite(0x94);                  //Line=3
  lcd.printIn("T5:       T6:");
  lcd.commandWrite(0xD4);                  //Line=4
  lcd.printIn("T7:       T8:");

  RefResh = 2;
  val = 2;  
  
  //Lets to the Conversion:
  DoCal = 1;
  
  //Fetch Typ K
  DoTypK = 1;

  //Select Serial Out Mode "Convertet":
  SerOut = 2;
  
  //print the Serial Header:
  HeaderPrint();

}

void Screen4Work()
{ 
   
  //This just printÂ´s all the Thermocouple values:
  //like screen2 it only updates every X repeatÂ´s
  
  
  
    if(val>=RefResh)
  {
     val = 0;
    
    lcd.commandWrite(0x80+4);                  //T1
    lcd.printIn(itoa((CalAGT[0]), buf, 10));
    Print2Blanks();
    lcd.commandWrite(0x80+14);                 //T2
    lcd.printIn(itoa((CalAGT[1]), buf, 10));
    Print2Blanks();
    lcd.commandWrite(0xC0+4);                  //T3
    lcd.printIn(itoa((CalAGT[2]), buf, 10));
    Print2Blanks();
    lcd.commandWrite(0xC0+14);                 //T4
    lcd.printIn(itoa((CalAGT[3]), buf, 10));
    Print2Blanks();
    lcd.commandWrite(0x94+4);                  //T5
    lcd.printIn(itoa((CalAGT[4]), buf, 10));
    Print2Blanks();
    lcd.commandWrite(0x94+14);                 //T6
    lcd.printIn(itoa((CalAGT[5]), buf, 10));
    Print2Blanks();
    lcd.commandWrite(0xD4+4);                  //T7
    lcd.printIn(itoa((CalAGT[6]), buf, 10));
    Print2Blanks();
    lcd.commandWrite(0xD4+14);                 //T8
    lcd.printIn(itoa((CalAGT[7]), buf, 10));
    Print2Blanks();
    
  }
  else
  {
    val++;
  }
  
  
  //delay(50);
  
}


