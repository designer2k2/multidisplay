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

//Main Screen

void Screen1Init()
{
  
  lcd.init();
  
  lcd.commandWrite(0x80);                  //Line1
  lcd.printIn("EGT:");

  lcd.commandWrite(0x94);                  //Line=3
  lcd.printIn("LD:        Max:");

  //Load the specific data:
  CGRAM_Bar();
  CGRAM_Dot();
  
  //Lets to the Conversion:
  DoCal = 1;
  
  //Fetch Typ K
  DoTypK = 1;

  //Select Serial Out Mode "Convertet":
  SerOut = 2;

  //print the Serial Header:
  HeaderPrint();
  
}

void Screen1Work()
{
 
  //Calibrate it:
  val = CalLambda/10;
  val3 = map(AnaIn[BoostPin], 0, 4096, 0, 200);
  //val3 = constrain(val3, 0, 200);
  
  lcd.commandWrite(0x80+5);               //Line 1, position 5! 

  lcd.printIn(itoa((CalAGT[0]+CalAGT[1])/2, buf, 10));  //AGT1+AGT2/2 thats becouse off my 2 Probes
  
  lcd.print(32);                           //Print a " " at the end, to clear in case its needed
 
 
  lcd.commandWrite(0x80+14);              //Line 3, 
    
  printfloat(CalCaseTemp,1);
  
  lcd.print(223);                         //Print a " " at the end, to clear in case its needed 
  lcd.printIn("C");                       //Print a " " at the end, to clear in case its needed
  
  lcd.commandWrite(0x94+3); 
  if(CalBoost<0.0)
    {
      lcd.printIn("-");
    }
  else
    {
      lcd.printIn(" ");
    } 
  printfloat(abs(CalBoost),2);              //Shows current Boost
  
  //Check for new MaxLD, and then save the point where to make the square
  if(CalBoost>MaxLD)
  {
    MaxLD  = CalBoost;
    MaxLDt = val3/10;
  }
  
  lcd.commandWrite(0x94+16); 
  printfloat(MaxLD,2);                    //Max Boost
  
  //Lets draw the dot:
    lcd.commandWrite(0xC0);              // Line 2
    for (int i=0; i <= 19; i++){
    if(i==val)
      {
        lcd.print(8);        //thats the Dot 
      }
    else
      {
        if(i <= LambdaLowerLimit)    //from 0-4 a minus then nothing
        {
          lcd.print(6);
        }
        else
        {
          if(i <= LambdaUpperLimit) //from 5-15 ist nothing and then a plus
          {
           lcd.print(5);
          }
         else
         {
           lcd.print(7);
         }
        }
      }
  }

  //Now lets make the Bar:
  lcd.commandWrite(0xD4);                // Line 4
  DrawBar(20,val3);
  
  //And draw a Dot at the max pos & 0 Pos
  
  lcd.commandWrite(0xD4+MaxLDt);                // Line 4
  lcd.print(255);  
  
  lcd.commandWrite(0xD4+LDcalPoint);                // Line 4
  lcd.print(255);  
  
  
}


