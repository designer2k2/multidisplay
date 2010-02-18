//2 Row Big Font Screen

void Screen6Init()
{
  
  lcd.init();
  
  lcd.commandWrite(0x80);                  //Line1
  lcd.printIn("Boost:");

  lcd.commandWrite(0x94);                  //Line3
  lcd.printIn("Temp:");
 
  //Load the specific data:
  CGRAM_BigFont2();
  
  //Lets to the Conversion:
  DoCal = 1;
  
  //Fetch Typ K
  DoTypK = 1;

  //Select Serial Out Mode "Convertet":
  SerOut = 2;

  //print the Serial Header:
  HeaderPrint();
  
  RefResh = 5;
  val = 10;    
  val1 = 1;
  
}

void Screen6InitB()
{
  
  lcd.init();
  
  lcd.commandWrite(0x80);                  //Line1
  lcd.printIn("T1:   ");

  lcd.commandWrite(0x94);                  //Line3
  lcd.printIn("T2:   ");
 
   //Clear the - from the screen A
  lcd.commandWrite(0x80+7); 
  lcd.printIn("  "); 	  
 
  //Load the specific data:
  CGRAM_BigFont2();
  
  //print the Serial Header:
  HeaderPrint();
  
  RefResh = 5;
  val = 10;    
  
}

void Screen6Work()
{
   
   //and only after every X refreshes, so its faster on the serial & its easier to read
 
   if(val>=RefResh)
  {
     val = 0;
  
 
  switch(val1){
   case 1:
     
	  //The Boost:
	  
	  lcd.commandWrite(0x80+7); 
	  if(CalBoost<0.0)
		{
		  lcd.print(2);
		  lcd.print(2);
		 }
	  else
		{
		  lcd.printIn("  ");
		} 

	  bigNum(abs(CalBoost)*1000,0,9,1);
	   
		
	  //The Temp:
	 
	 
	  bigNum(abs(CalCaseTemp)*1000,20,9,1);
	   
  
  break;
  case 2:
     //T1:
  
     bigNum24(CalAGT[0],0+8);
   
     //T2:
 
     //bigNum24(CalAGT[1],20);
     
     bigNum24(CalAGT[1],20+8);
  
     break;
 case 3:
  val1 = 1;
  break;
  }
  
  
      }
  else
  {
    val++;
  }
  
  
  
}

