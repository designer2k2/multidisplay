//Bigfont Code
//
//BIG parts from http://opengauge.googlecode.com/svn/trunk/mpguino/mpguino.pde

//-------------------------------------------------------------------------------------------------------
void CGRAM_BigFont2()
{
  
  //255 is the "Full Block" Code, and 254 is blank
  int data1[] = {31,31,31,0,0,0,0,0};    //Small Block on top
  int data2[] = {0,0,0,0,0,31,31,31};    //Small Block on bottom
  int data3[] = {31,31,31,0,0,0,31,31};  //Small Block on top and bottom
 // int data4[] = {28,30,31,31,31,31,30,28};  //Full Block roundet right
 // int data5[] = {7,15,31,31,31,31,15,7};  //Full Block roundet left
  int data6[] = {0,0,0,14,14,14,12,8};  //Dot, for Decimal. 
  
  LcdUploadUdef5x8(1,data1);
  LcdUploadUdef5x8(2,data2);
  LcdUploadUdef5x8(3,data3);
 // LcdUploadUdef5x8(4,data4);
 // LcdUploadUdef5x8(5,data5);
  LcdUploadUdef5x8(6,data6);
    
}
//-------------------------------------------------------------------------------------------------------

void CGRAM_BigFont4()
{
  
  //255 is the "Full Block" Code, and 254 is blank
  int data1[] = {0,0,0,0,3,15,15,31};    //Small Block on bottom left
  int data2[] = {0,0,0,0,31,31,31,31};    //Small Block on bottom right
  int data3[] = {0,0,0,0,24,30,30,31};  //Small Block on bottom full
  int data4[] = {31,15,15,3,0,0,0,0};  //Small Block on top left
  int data5[] = {31,30,30,24,0,0,0,0};  //Small Block on top right
  int data6[] = {31,31,31,31,0,0,0,0};  //Small Block on top full
  int data7[] = {14,14,14,14,12,8,0,0};  //Dot, for Decimal. 
  
  LcdUploadUdef5x8(1,data1);
  LcdUploadUdef5x8(2,data2);
  LcdUploadUdef5x8(3,data3);
  LcdUploadUdef5x8(4,data4);
  LcdUploadUdef5x8(5,data5);
  LcdUploadUdef5x8(6,data6);
  LcdUploadUdef5x8(7,data7);
    
}
//-------------------------------------------------------------------------------------------------------


// Array index into parts of big numbers. Numbers consist of 6 custom characters in 2 lines
//              0             1          2        3          4             5         6       7               8       9    
//char bn12[]={255,1,255,0, 1,255,254,0, 1,3,4,0, 1,3,4,0, 255,2,2,0,     255,3,1,0, 5,3,1,0, 1,1,4,0,       5,3,4,0, 5,3,4,0};
//char bn22[]={255,2,255,0, 2,255,2,0,   5,2,2,0, 2,2,4,0, 254,255,254,0, 2,2,4,0,   5,2,4,0, 254,255,254,0, 5,2,4,0, 254,254,255,0};

char bn12[]={255,1,255,0, 1,255,254,0, 1,3,255,0, 1,3,255,0, 255,2,2,0,     255,3,1,0, 255,3,1,0, 1,1,255,0,       255,3,255,0, 255,3,255,0};
char bn22[]={255,2,255,0, 2,255,2,0,   255,2,2,0, 2,2,255,0, 254,255,254,0, 2,2,255,0,   255,2,255,0, 254,255,254,0, 255,2,255,0, 254,254,255,0};


// Array index into parts of big numbers. Numbers consist of 12 custom characters in 4 lines
//              0               1            2              3                4             5             6              7              8              9    
char bn14[]={1,2,3,0,       2,3,254,0,     1,2,3,0,       1,2,3,0,       2,254,254,0,   2,2,2,0,       1,2,3,0,       2,2,2,0,       1,2,3,0,       1,2,3,0};
char bn24[]={255,254,255,0, 254,255,254,0, 1,2,255,0,     254,2,255,0,   255,2,2,0,     255,2,2,0,     255,2,3,0,     254,2,255,0,   255,2,255,0,   255,254,255,0};
char bn34[]={255,254,255,0, 254,255,254,0, 255,254,254,0, 254,254,255,0, 254,255,254,0, 254,254,255,0, 255,254,255,0, 254,255,254,0, 255,254,255,0, 4,6,255,0};
char bn44[]={4,6,5,0,       6,6,6,0,       4,6,6,0,       4,6,5,0,       254,6,254,0,   6,6,5,0,       4,6,5,0,       254,6,254,0,   4,6,5,0,       254,254,6,0};

//-------------------------------------------------------------------------------------------------------

void printOneNumber2(uint8_t digit, byte leftAdjust, int LineOffset)
{
  // LineOffset = 0, means 1 Line, LineOffset = 20 means 2 Line.
  // leftAdjust = 0, means 1 Pos, leftAdjust = 1 is second Pos.

  // Line 1 of the one digit number
  lcd.commandWrite(0x80+LineOffset+leftAdjust*3+1*leftAdjust);                  //Line1 
  
  lcd.print(bn12[digit*3+digit*1]);
  lcd.print(bn12[digit*3+1+digit*1]);
  lcd.print(bn12[digit*3+2+digit*1]);

  // Line 2 of the one-digit number
  lcd.commandWrite(0xC0+LineOffset+leftAdjust*3+1*leftAdjust);              // Line 2
  lcd.print(bn22[digit*3+digit*1]);
  lcd.print(bn22[digit*3+1+digit*1]);
  lcd.print(bn22[digit*3+2+digit*1]);

}

//-------------------------------------------------------------------------------------------------------

void printOneNumber4(uint8_t digit, byte leftAdjust)
{
  // leftAdjust = 0, means 1 Pos, leftAdjust = 1 is second Pos.

  // Line 1 of the one digit number
  lcd.commandWrite(0x80+leftAdjust*3+1*leftAdjust);                  //Line 1 
  lcd.print(bn14[digit*3]);
  lcd.print(bn14[digit*3+1]);
  lcd.print(bn14[digit*3+2]);

  // Line 2 of the one-digit number
  lcd.commandWrite(0xC0+leftAdjust*3+1*leftAdjust);              // Line 2
  lcd.print(bn24[digit*3]);
  lcd.print(bn24[digit*3+1]);
  lcd.print(bn24[digit*3+2]);

  // Line 3 of the one digit number
  lcd.commandWrite(0x94+leftAdjust*3+1*leftAdjust);                  //Line 3
  lcd.print(bn34[digit*3]);
  lcd.print(bn34[digit*3+1]);
  lcd.print(bn34[digit*3+2]);

  // Line 4 of the one-digit number
  lcd.commandWrite(0xD4+leftAdjust*3+1*leftAdjust);              // Line 4
  lcd.print(bn44[digit*3]);
  lcd.print(bn44[digit*3+1]);
  lcd.print(bn44[digit*3+2]);

}

//-------------------------------------------------------------------------------------------------------

void bigNum (unsigned long t, int LineOffset, byte leftAdjust, int d){     
 
// LineOffset = 0, means 1 Line, LineOffset = 20 means 2 Line.
// d = 0, means no dp!
  
//  unsigned long t = 98550ul;//number in thousandths 
//  unsigned long t = 9855ul;//number in thousandths 
  char  dp = 254; 
  char  dp2 = 254; 
  
 
  char * r = "009.99"; //default to 999
  if(t<=99500){ 
    r=format(t/10); //0098.6 
    dp=6; 
  }else if(t<=999500){ 
    r=format(t/100); 
  }   
  if(t<=9950)
  {
    dp = 254;
    dp2 = 6;
    r=format(t); //009.86
  }
  
  if(d==0)
  {
   dp = 254;
   dp2 = 254;
  }
  
  lcd.commandWrite(0x80+LineOffset+leftAdjust); 
  lcd.printIn(bn12+(r[2]-'0')*4); 
  lcd.printIn(" "); 
  lcd.printIn(bn12+(r[4]-'0')*4); 
  lcd.printIn(" "); 
  lcd.printIn(bn12+(r[5]-'0')*4); 
 
  lcd.commandWrite(0xC0+LineOffset+leftAdjust);
  lcd.printIn(bn22+(r[2]-'0')*4); 
  lcd.print(dp2); 
  lcd.printIn(bn22+(r[4]-'0')*4); 
  lcd.print(dp); 
  lcd.printIn(bn22+(r[5]-'0')*4); 

} 

//-------------------------------------------------------------------------------------------------------

void bigNum4 (unsigned long t, byte leftAdjust, int d){     
 
// LineOffset = 0, means 1 Line, LineOffset = 20 means 2 Line.
// d = 0, means no dp!
  
//  unsigned long t = 98550ul;//number in thousandths 
//  unsigned long t = 9855ul;//number in thousandths 
  char  dp = 254; 
  char  dp2 = 254; 
  
 
  char * r = "009.99"; //default to 999
  if(t<=99500){ 
    r=format(t/10); //0098.6 
    dp=7; 
  }else if(t<=999500){ 
    r=format(t/100); 
  }   
  if(t<=9950)
  {
    dp = 254;
    dp2 = 7;
    r=format(t); //009.86
  }
  
  if(d==0)
  {
   dp = 254;
   dp2 = 254;
  }
  
  lcd.commandWrite(0x80+leftAdjust); 
  lcd.printIn(bn14+(r[2]-'0')*4); 
  lcd.printIn(" "); 
  lcd.printIn(bn14+(r[4]-'0')*4); 
  lcd.printIn(" "); 
  lcd.printIn(bn14+(r[5]-'0')*4); 
 
  lcd.commandWrite(0xC0+leftAdjust); 
  lcd.printIn(bn24+(r[2]-'0')*4); 
  lcd.printIn(" "); 
  lcd.printIn(bn24+(r[4]-'0')*4); 
  lcd.printIn(" "); 
  lcd.printIn(bn24+(r[5]-'0')*4);

  lcd.commandWrite(0x94+leftAdjust); 
  lcd.printIn(bn34+(r[2]-'0')*4); 
  lcd.printIn(" "); 
  lcd.printIn(bn34+(r[4]-'0')*4); 
  lcd.printIn(" "); 
  lcd.printIn(bn34+(r[5]-'0')*4);
 
  lcd.commandWrite(0xD4+leftAdjust);
  lcd.printIn(bn44+(r[2]-'0')*4); 
  lcd.print(dp2); 
  lcd.printIn(bn44+(r[4]-'0')*4); 
  lcd.print(dp); 
  lcd.printIn(bn44+(r[5]-'0')*4); 

} 



//-------------------------------------------------------------------------------------------------------
//Displays a 4 Digit Number (RPM, Thermocouple Temp) over 2 Lines
//only Full Digits are shown!
//no leading 0 or commas.
//it clears all in front

void bigNum24(int Value,int LineOffset)
{
  
  //how many digits?
  
  int Dig;
  
  if(Value>=999)
  {
    Dig = 4;
  }
  else
  {
    if(Value>=99)
    {
      Dig = 3;
    }
    else
    {
      if(Value>=9)
      {
        Dig = 2;
      }
      else
      {
        Dig = 1;
      }
    }
  }
  
  //depending from the Digits, i need to print it different.
  
  int D1;
  int D2;
  int D3;
  int D4; 
    
  switch (Dig) {
    case 1:
      //Thats easy...
      printOneNumber2(Value,0,LineOffset+9);
      
      //And clear digit 1+2+3:
      lcd.commandWrite(0x80+LineOffset+0);                  //Line1 
      blanks3();
      blanks3();
      blanks3();      
      
      lcd.commandWrite(0xC0+LineOffset+0);                  //Line1 
      blanks3();
      blanks3();
      blanks3(); 
      
      break;
    case 2:
      //Not so easy anymore.
      D2 = abs(Value/10);  //First Value
      D1 = Value - D2*10;     //Last Value
      
      printOneNumber2(D2,0,LineOffset+6);
      printOneNumber2(D1,0,LineOffset+9);
      
      //And clear digit 1+2:
      lcd.commandWrite(0x80+LineOffset+0);                  //Line1 
      blanks3();
      blanks3();
      
      
      lcd.commandWrite(0xC0+LineOffset+0);                  //Line1 
      blanks3();
      blanks3();    
      
      break;
    case 3:
      //Not so easy anymore.
      D3 = abs(Value/100);  //First Value
      D2 = abs((Value - D3*100)/10);
      D1 = abs((Value - D3*100 - D2*10));     //Last Value
            
      printOneNumber2(D3,0,LineOffset+3);
      printOneNumber2(D2,0,LineOffset+6);
      printOneNumber2(D1,0,LineOffset+9);
      
      //And clear digit 1:
      lcd.commandWrite(0x80+LineOffset+0);                  //Line1 
      blanks3();
      
      lcd.commandWrite(0xC0+LineOffset+0);                  //Line1 
      blanks3();
      
      
      break;
    case 4:
      //Not so easy anymore.
      D4 = abs(Value/1000); //First Value
      D3 = abs((Value - D4*1000)/100);  
      D2 = abs((Value - D4*1000 - D3*100)/10);
      D1 = abs((Value - D4*1000 - D3*100 - D2*10));     //Last Value
            
      printOneNumber2(D4,0,LineOffset+0);     
      printOneNumber2(D3,0,LineOffset+3);
      printOneNumber2(D2,0,LineOffset+6);
      printOneNumber2(D1,0,LineOffset+9);
    

      break;
    }
  
  
}

void blanks3()
{
      lcd.print(254);
      lcd.print(254);
      lcd.print(254);   
}


//-------------------------------------------------------------------------------------------------------

char fBuff[7];//used by format    

char* format(unsigned long num){
  byte dp = 3;

  while(num > 999999){
    num /= 10;
    dp++;
    if( dp == 5 ) break; // We'll lose the top numbers like an odometer
  }
  if(dp == 5) dp = 99; // We don't need a decimal point here.

// Round off the non-printed value.
  if((num % 10) > 4)
    num += 10;
  num /= 10;
  byte x = 6;
  while(x > 0){
    x--;
    if(x==dp){ //time to poke in the decimal point?{
      fBuff[x]='.';
    }else{
      fBuff[x]= '0' + (num % 10);//poke the ascii character for the digit.
      num /= 10;
    } 
  }
  fBuff[6] = 0;
  return fBuff;
}

