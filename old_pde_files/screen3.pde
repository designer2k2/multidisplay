//Bar Screen

void Screen3Init()
{
  
  lcd.init();
  
  lcd.commandWrite(0x80);                  //Line1
  lcd.printIn("LAM:");
  lcd.commandWrite(0xC0);                  //Line=2
  lcd.printIn("LD :");
  lcd.commandWrite(0x94);                  //Line=3
  lcd.printIn("THR:");
  lcd.commandWrite(0xD4);                  //Line=4
  lcd.printIn("RPM:");
  
  CGRAM_Bar();

  //Select Serial Out Mode Converted:
  SerOut = 2;

  //And calibration:
  DoCal = 1;  
  
  //Fetch Typ K
  DoTypK = 1;

  //print the Serial Header:
  HeaderPrint();
  
  
  RefResh = 10;
  val = 50; 

}

void Screen3Work()
{ 
  
  //First, show the Bars on the last 10 Chars:
  
  val1 = map(AnaIn[BoostPin], 0, 4096, 0, 100);
  val1 = constrain(val1, 0, 100);
  val2 = map(AnaIn[RPMPin], 0, RPMmax, 0, 100);
  val2 = constrain(val2, 0, 100);
  
  //Print the Values:
  

  lcd.commandWrite(0x80+5);       
  lcd.printIn(itoa(CalLambda, buf, 10));
  lcd.print(32);
  lcd.commandWrite(0xC0+4);    

  if(CalBoost<0.0)
    {
      lcd.printIn("-");
    }
  else
    {
      lcd.printIn(" ");
    } 
  printfloat(abs(CalBoost),2);              //Shows current Boost
  
  lcd.print(32);
  lcd.commandWrite(0x94+5);               
  lcd.printIn(itoa(CalThrottle, buf, 10));
  lcd.print(32);
  lcd.commandWrite(0xD4+5);               
  lcd.printIn(itoa((CalRPM/10)*10, buf, 10));    //gets rid of the last 10, thats not needed.
  lcd.print(32);
  
  
   //Now lets make the Bars:
  lcd.commandWrite(0x80+10);               
  DrawBar(10,CalLambda/2);
  lcd.commandWrite(0xC0+10);               
  DrawBar(10,val1);
  lcd.commandWrite(0x94+10);               
  DrawBar(10,CalThrottle);
  lcd.commandWrite(0xD4+10);               
  DrawBar(10,val2);
  
}

