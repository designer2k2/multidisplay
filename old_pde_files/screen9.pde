//VDO Screen

void Screen9Init()
{
   lcd.init();
   
  lcd.commandWrite(0x80);                  //Line1
  lcd.printIn("VDO Screen:");
  lcd.commandWrite(0xC0);                  //Line=2
  lcd.printIn("T1:       P1:");
  lcd.commandWrite(0x94);                  //Line=3
  lcd.printIn("T2:       P2:");
  lcd.commandWrite(0xD4);                  //Line=4
  lcd.printIn("T3:       P3:");

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

void Screen9Work()
{ 
   
  //This just print´s all the Thermocouple values:
  //like screen2 it only updates every X repeat´s
  
  
  
    if(val>=RefResh)
  {
     val = 0;
    
    lcd.commandWrite(0xC0+4);                  //T1
    lcd.printIn(itoa(VDOTemp1, buf, 10));
    Print2Blanks();
    lcd.commandWrite(0xC0+14);                 //P1
    printfloat(float(VDOPres1/10.0),1);
    Print2Blanks();
    lcd.commandWrite(0x94+4);                  //T2
    lcd.printIn(itoa(VDOTemp2, buf, 10));  
    Print2Blanks();
    lcd.commandWrite(0x94+14);                 //P2
    printfloat(float(VDOPres2/10.0),1);
    Print2Blanks();
    lcd.commandWrite(0xD4+4);                  //T3
    lcd.printIn(itoa(VDOTemp3, buf, 10));
    Print2Blanks();
    lcd.commandWrite(0xD4+14);                 //P3
    printfloat(float(VDOPres3/10.0),1);
    Print2Blanks();
        
  }
  else
  {
    val++;
  }
  
  //delay(50);
  
}

