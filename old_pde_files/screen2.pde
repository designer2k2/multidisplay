//Debug Screen

void Screen2Init()
{
  
  lcd.init();
  
  lcd.commandWrite(0x80);                  //Line1
  lcd.printIn("A1:       A2:");
  lcd.commandWrite(0xC0);                  //Line=2
  lcd.printIn("A3:       A4:");
  lcd.commandWrite(0x94);                  //Line=3
  lcd.printIn("A5:       A6:");
  lcd.commandWrite(0xD4);                  //Line=4
  lcd.printIn("A7:       A8:");
  
  RefResh = 2;
  val = 2;  
  
  //Select Serial Out Mode RAW:
  SerOut = 1;

  //And calibration:
  DoCal = 1;
  
  //Fetch Typ K
  DoTypK = 1;
  
  //print the Serial Header:
  HeaderPrint();
  
  
  //select A (0 Offset)
  val1 = 0;

}

void Screen2InitB()
{
  
  lcd.init();
  
  lcd.commandWrite(0x80);                  //Line1
  lcd.printIn("B1:       B2:");
  lcd.commandWrite(0xC0);                  //Line=2
  lcd.printIn("B3:       B4:");
  lcd.commandWrite(0x94);                  //Line=3
  lcd.printIn("B5:       B6:");
  lcd.commandWrite(0xD4);                  //Line=4
  lcd.printIn("B7:       B8:");
  
  RefResh = 2;
  val = 2;  
  
  //print the Serial Header:
  HeaderPrint();

  //select B (8 Offset)
  val1 = 8;

}


void Print2Blanks()
{
  lcd.print(32);
  lcd.print(32);
}

void Screen2Work()
{
  //This screen only shows the current Value from all 8 Inputs in RAW
  //and only after every X refreshes, so its faster on the serial & its easier to read

  if(val>=RefResh)
  {
     val = 0;
    
    lcd.commandWrite(0x80+4);                  //A1
    lcd.printIn(itoa((AnaIn[1+val1]), buf, 10));
    Print2Blanks();
    lcd.commandWrite(0x80+14);                 //A2
    lcd.printIn(itoa((AnaIn[2+val1]), buf, 10));
    Print2Blanks();
    lcd.commandWrite(0xC0+4);                  //A3
    lcd.printIn(itoa((AnaIn[3+val1]), buf, 10));
    Print2Blanks();
    lcd.commandWrite(0xC0+14);                 //A4
    lcd.printIn(itoa((AnaIn[4+val1]), buf, 10));
    Print2Blanks();
    lcd.commandWrite(0x94+4);                  //A5
    lcd.printIn(itoa((AnaIn[5+val1]), buf, 10));
    Print2Blanks();
    lcd.commandWrite(0x94+14);                 //A6
    lcd.printIn(itoa((AnaIn[6+val1]), buf, 10));
    Print2Blanks();
    lcd.commandWrite(0xD4+4);                  //A7
    lcd.printIn(itoa((AnaIn[7+val1]), buf, 10));
    Print2Blanks();
    lcd.commandWrite(0xD4+14);                 //A8
    lcd.printIn(itoa((AnaIn[8+val1]), buf, 10));
    Print2Blanks();
  }
  else
  {
    val++;
  }
  
}

