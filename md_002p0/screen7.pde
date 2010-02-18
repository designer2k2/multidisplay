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

//Scope Mode Screen

void Screen7Init()
{
  
  lcd.init();
  
  //Load the specific data:
  CGRAM_VertBar();
  
  //Lets not to the Conversion:
  DoCal = 0;
  
  //Fetch Typ K
  DoTypK = 0;

  //Select Serial Out Mode "Nothing":
  SerOut = 0;

  //print the Serial Header:
  HeaderPrint();
  
  //The Constrains:
  val1 = 0;
  val2 = 5000;
  
  //The Data Selector
  val4 = 1; 
  
  RefResh = 10;
  val = 10;   
  
}


void Screen7Work()
{
   
  //new Constrains if needed
  
  int ScopeVal;
  
  
  
  switch(val4){
    case 1:
     ScopeVal = AnaIn[BoostPin];
     break;
    case 2:
     ScopeVal = CalThrottle;
     break;
    case 3:
     ScopeVal = CalLMM;
     break;
    case 4:
     ScopeVal = CalRPM;
     break;
    default:
     break;
    }
  
  
  if(ScopeVal<val2)
  {
     val2 = ScopeVal;
  }
  
  if(ScopeVal>val1)
  {
    val1 = ScopeVal;
  }
  
  //Get the Data:  
  val3 = map(ScopeVal, val2, val1, 0, 33);
  //val3 = constrain(val3, 0, 32);
     
  //Show only the rightmost bar: (live screen)
  DrawVertBar(val3,19);
  
 //and show it only after every X refreshes, so its faster on the serial & its easier to read
 if(val>=RefResh)
  {
     val = 0;
    
     //Feed it into the Buffer (on the right side)
     ScopeInt[19] = val3;
    
     ScopeMode();
     
  }
  else
  {
    val++;
  }


  //Draw the Caption:  
  lcd.commandWrite(0x80);                  //Line1
  lcd.printIn(itoa(val4, buf, 10));
  lcd.printIn(":");
  lcd.printIn(itoa(RefResh, buf, 10));  

  
  Serial.println(time);
 
}


