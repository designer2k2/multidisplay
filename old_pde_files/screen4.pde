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
   
  //This just print´s all the Thermocouple values:
  //like screen2 it only updates every X repeat´s
  
  
  
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

