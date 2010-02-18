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

//1 Row Big Font Screen

void Screen5Init()
{
  
  lcd.init();
  
  lcd.commandWrite(0x80);                  //Line1
  lcd.printIn("Boost:");

  lcd.commandWrite(0x94);                  //Line2
  lcd.printIn("Max:");
 
  //Load the specific data:
  CGRAM_BigFont4();
  
  //Lets to the Conversion:
  DoCal = 1;
  
  //Fetch Typ K
  DoTypK = 1;

  //Select Serial Out Mode "Convertet":
  SerOut = 2;

  //print the Serial Header:
  HeaderPrint();
  
  RefResh = 10;
  val = 10;  
  
}

void Screen5Work()
{
   
    //and only after every X refreshes, so its faster on the serial & its easier to read

  if(val>=RefResh)
  {
     val = 0;

  
  lcd.commandWrite(0xC0+7); 
  if(CalBoost<0.0)
    {
      lcd.print(2);
      lcd.print(2);
    }
  else
    {
      lcd.printIn("  ");
    } 

  //bigNum(abs(CalBoost)*1000,0,9,1);
  
  bigNum4(abs(CalBoost)*1000,9,1);
  
  }
  else
  {
    val++;
  }
    
  //Check for new MaxLD
  if(CalBoost>MaxLD)
  {
    MaxLD  = CalBoost;
  }
  
  
  lcd.commandWrite(0xD4); 
  printfloat(MaxLD,2);                    //Max Boost
  
  
  
}


