/*
   Goldelox_Serial_4DLib.cpp - Library for 4D Systems Serial Environment.
 */

#include "Goldelox_Serial_4DLib.h"

#if (ARDUINO >= 100)
	#include "Arduino.h" // for Arduino 1.0
#else
	#include "WProgram.h" // for Arduino 23
#endif

Goldelox_Serial_4DLib::Goldelox_Serial_4DLib(Stream * virtualPort) { 
        _virtualPort = virtualPort; 
	_virtualPort->flush();
}

//*********************************************************************************************//
//**********************************Intrinsic 4D Routines**************************************//
//*********************************************************************************************//

void Goldelox_Serial_4DLib::WriteChars(char * charsout)
{
  unsigned char wk ;
  do
  {
    wk = *charsout++ ;
    _virtualPort->write(wk) ;
  } while (wk) ;
}

void Goldelox_Serial_4DLib::WriteBytes(char * Source, int Size)
{
 	unsigned char wk ;
	int i ;
	for (i = 0; i < Size; i++)
	{
		wk = *Source++ ;
                _virtualPort->write(wk) ;
	}
}

void Goldelox_Serial_4DLib::WriteWords(word * Source, int Size)
{
  word wk ;
  int i ;
  for (i = 0; i < Size; i++)
  {
    wk = *Source++ ;
    _virtualPort->write(wk >> 8) ;
    _virtualPort->write(wk) ;
  }
}

void Goldelox_Serial_4DLib::getbytes(char * data, int size)
{
  int read ;
  unsigned long sttime ;
  int readc ;
  readc  = 0 ;
  sttime = millis() ;
  while ((readc != size) && (millis() - sttime < TimeLimit4D))
  {
    if(_virtualPort->available()) 
    {
      data[readc++] = _virtualPort->read() ;
    }
  }
  if (readc != size)
  {
    Error4D = Err4D_Timeout ;
    if (Callback4D != NULL)
      Callback4D(Error4D, Error4D_Inv) ;
  }
}

void Goldelox_Serial_4DLib::GetAck(void)
{
  int read ;
  unsigned char readx ;
  unsigned long sttime ;
  Error4D = Err4D_OK ;
  sttime  = millis();
  read    = 0 ;
  while ((read != 1) && (millis() - sttime < TimeLimit4D))
  {
    if(_virtualPort->available() != 0) 
    {
      readx = _virtualPort->read() ;
      read = 1 ;
    }
  }
  if (read == 0)
  {
    Error4D = Err4D_Timeout ;
    if (Callback4D != NULL)
      Callback4D(Error4D, Error4D_Inv) ;
  }
  else if (readx != 6)
  {
    Error4D     = Err4D_NAK ;
    Error4D_Inv = readx ;
    if (Callback4D != NULL)
      Callback4D(Error4D, Error4D_Inv) ;
  }
}

word Goldelox_Serial_4DLib::GetWord(void)
{
  unsigned char readx[2] ;
  int readc ;
  unsigned long sttime ;
  
  if (Error4D != Err4D_OK)
    return 0 ;
  sttime   = millis();
  readc    = 0 ;
  while ((readc != 2) && (millis() - sttime < TimeLimit4D))
  {
    if(_virtualPort->available()) 
    {
      readx[readc++] = _virtualPort->read() ;
    }
  }
  
  if (readc != 2)
  {
    Error4D  = Err4D_Timeout ;
    if (Callback4D != NULL)
      Callback4D(Error4D, Error4D_Inv) ;
  return 0 ;
  }
  else
    return readx[0] << 8 | readx[1] ;
}

void Goldelox_Serial_4DLib::getString(char * outStr, int strLen)
{
  unsigned char readx[2] ;
  int readc ;
  unsigned long sttime ;
  
  if (Error4D != Err4D_OK)
  {
    outStr[0] = 0 ;
    return ;
  }
  sttime   = millis();
  readc    = 0 ;
  while ((readc != strLen) && (millis() - sttime < TimeLimit4D))
  {
    if(_virtualPort->available()) 
    {
      outStr[readc++] = _virtualPort->read() ;
    }
  }
  
  if (readc != strLen)
  {
    Error4D  = Err4D_Timeout ;
    if (Callback4D != NULL)
      Callback4D(Error4D, Error4D_Inv) ;
  }
  outStr[readc] = 0 ;
}

word Goldelox_Serial_4DLib::GetAckResp(void)
{
	GetAck() ;
	return GetWord() ;
}

word Goldelox_Serial_4DLib::GetAckRes2Words(word * word1, word * word2)
{
	int Result ;
	GetAck() ;
	Result = GetWord() ;
	*word1 = GetWord() ;
	*word2 = GetWord() ;
	return Result ;
}

void Goldelox_Serial_4DLib::GetAck2Words(word * word1, word * word2)
{
	GetAck() ;
	*word1 = GetWord() ;
	*word2 = GetWord() ;
}

word Goldelox_Serial_4DLib::GetAckResStr(char * OutStr)
{
	int Result ;
	GetAck() ;
	Result = GetWord() ;
	getString(OutStr, Result) ;
	return Result ;
}
/*
word Goldelox_Serial_4DLib::GetAckResData(t4DByteArray OutData, word size)
{
	int Result ;
	GetAck() ;
	Result = GetWord() ;
	getbytes(OutData, size) ;
	return Result ;
}
*/
void Goldelox_Serial_4DLib::SetThisBaudrate(int Newrate)
{
  int br ;
  _virtualPort->flush() ;
//  _virtualPort->end() ;
  switch(Newrate)
  {
 /*   case BAUD_110    : br = 110 ;
      break ;
    case BAUD_300    : br = 300 ;
      break ;
    case BAUD_600    : br = 600 ;
      break ;
    case BAUD_1200   : br = 1200 ;
      break ;
    case BAUD_2400   : br = 2400 ;
      break ;
    case BAUD_4800   : br = 4800 ;
      break ;*/
    case BAUD_9600   : br = 9600 ;
      break ;
//   case BAUD_14400  : br = 14400 ;
//      break ;
    case BAUD_19200  : br = 19200 ;
      break ;
    case BAUD_31250  : br = 31250 ;
      break ;
    case BAUD_38400  : br = 38400 ;
      break ;
    case BAUD_56000  : br = 56000 ;
      break ;
    case BAUD_57600  : br = 57600 ;
      break ;
    case BAUD_115200 : br = 115200 ;
      break ;
    case BAUD_128000 : br = 133928 ; // actual rate is not 128000 ;
      break ;
    case BAUD_256000 : br = 281250 ; // actual rate is not  256000 ;
      break ;
    case BAUD_300000 : br = 312500 ; // actual rate is not  300000 ;
      break ;
    case BAUD_375000 : br = 401785 ; // actual rate is not  375000 ;
      break ;
    case BAUD_500000 : br = 562500 ; // actual rate is not  500000 ;
      break ;
    case BAUD_600000 : br = 703125 ; // actual rate is not  600000 ;
      break ;
  }
//  _virtualPort->begin(br) ;
  delay(50) ; // Display sleeps for 100
  _virtualPort->flush() ;
}

//*********************************************************************************************//
//**********************************Compound 4D Routines***************************************//
//*********************************************************************************************//

word Goldelox_Serial_4DLib::charheight(char  TestChar)
{
  _virtualPort->print((char)(F_charheight >> 8)) ;
  _virtualPort->print((char)(F_charheight)) ;
  _virtualPort->print((char)(TestChar)) ;
  return GetAckResp() ;
}

word Goldelox_Serial_4DLib::charwidth(char  TestChar)
{
  _virtualPort->print((char)(F_charwidth >> 8)) ;
  _virtualPort->print((char)(F_charwidth)) ;
  _virtualPort->print((char)(TestChar)) ;
  return GetAckResp() ;
}

void Goldelox_Serial_4DLib::gfx_BGcolour(word  Color)
{
  _virtualPort->print((char)(F_gfx_BGcolour >> 8)) ;
  _virtualPort->print((char)(F_gfx_BGcolour)) ;
  _virtualPort->print((char)(Color >> 8)) ;
  _virtualPort->print((char)(Color)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::gfx_ChangeColour(word  OldColor, word  NewColor)
{
  _virtualPort->print((char)(F_gfx_ChangeColour >> 8)) ;
  _virtualPort->print((char)(F_gfx_ChangeColour)) ;
  _virtualPort->print((char)(OldColor >> 8)) ;
  _virtualPort->print((char)(OldColor)) ;
  _virtualPort->print((char)(NewColor >> 8)) ;
  _virtualPort->print((char)(NewColor)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::gfx_Circle(word  X, word  Y, word  Radius, word  Color)
{
  _virtualPort->print((char)(F_gfx_Circle >> 8)) ;
  _virtualPort->print((char)(F_gfx_Circle)) ;
  _virtualPort->print((char)(X >> 8)) ;
  _virtualPort->print((char)(X)) ;
  _virtualPort->print((char)(Y >> 8)) ;
  _virtualPort->print((char)(Y)) ;
  _virtualPort->print((char)(Radius >> 8)) ;
  _virtualPort->print((char)(Radius)) ;
  _virtualPort->print((char)(Color >> 8)) ;
  _virtualPort->print((char)(Color)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::gfx_CircleFilled(word  X, word  Y, word  Radius, word  Color)
{
  _virtualPort->print((char)(F_gfx_CircleFilled >> 8)) ;
  _virtualPort->print((char)(F_gfx_CircleFilled)) ;
  _virtualPort->print((char)(X >> 8)) ;
  _virtualPort->print((char)(X)) ;
  _virtualPort->print((char)(Y >> 8)) ;
  _virtualPort->print((char)(Y)) ;
  _virtualPort->print((char)(Radius >> 8)) ;
  _virtualPort->print((char)(Radius)) ;
  _virtualPort->print((char)(Color >> 8)) ;
  _virtualPort->print((char)(Color)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::gfx_Clipping(word  OnOff)
{
  _virtualPort->print((char)(F_gfx_Clipping >> 8)) ;
  _virtualPort->print((char)(F_gfx_Clipping)) ;
  _virtualPort->print((char)(OnOff >> 8)) ;
  _virtualPort->print((char)(OnOff)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::gfx_ClipWindow(word  X1, word  Y1, word  X2, word  Y2)
{
  _virtualPort->print((char)(F_gfx_ClipWindow >> 8)) ;
  _virtualPort->print((char)(F_gfx_ClipWindow)) ;
  _virtualPort->print((char)(X1 >> 8)) ;
  _virtualPort->print((char)(X1)) ;
  _virtualPort->print((char)(Y1 >> 8)) ;
  _virtualPort->print((char)(Y1)) ;
  _virtualPort->print((char)(X2 >> 8)) ;
  _virtualPort->print((char)(X2)) ;
  _virtualPort->print((char)(Y2 >> 8)) ;
  _virtualPort->print((char)(Y2)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::gfx_Cls()
{
  _virtualPort->print((char)(F_gfx_Cls >> 8)) ;
  _virtualPort->print((char)(F_gfx_Cls)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::gfx_Contrast(word  Contrast)
{
  _virtualPort->print((char)(F_gfx_Contrast >> 8)) ;
  _virtualPort->print((char)(F_gfx_Contrast)) ;
  _virtualPort->print((char)(Contrast >> 8)) ;
  _virtualPort->print((char)(Contrast)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::gfx_FrameDelay(word  Msec)
{
  _virtualPort->print((char)(F_gfx_FrameDelay >> 8)) ;
  _virtualPort->print((char)(F_gfx_FrameDelay)) ;
  _virtualPort->print((char)(Msec >> 8)) ;
  _virtualPort->print((char)(Msec)) ;
  GetAck() ;
}

word Goldelox_Serial_4DLib::gfx_GetPixel(word  X, word  Y)
{
  _virtualPort->print((char)(F_gfx_GetPixel >> 8 )) ;
  _virtualPort->print((char)(F_gfx_GetPixel )) ;
  _virtualPort->print((char)(X >> 8 )) ;
  _virtualPort->print((char)(X )) ;
  _virtualPort->print((char)(Y >> 8 )) ;
  _virtualPort->print((char)(Y )) ;
  return GetAckResp() ;
}

void Goldelox_Serial_4DLib::gfx_Line(word  X1, word  Y1, word  X2, word  Y2, word  Color)
{
  _virtualPort->print((char)(F_gfx_Line >> 8 )) ;
  _virtualPort->print((char)(F_gfx_Line )) ;
  _virtualPort->print((char)(X1 >> 8 )) ;
  _virtualPort->print((char)(X1 )) ;
  _virtualPort->print((char)(Y1 >> 8 )) ;
  _virtualPort->print((char)(Y1 )) ;
  _virtualPort->print((char)(X2 >> 8 )) ;
  _virtualPort->print((char)(X2 )) ;
  _virtualPort->print((char)(Y2 >> 8 )) ;
  _virtualPort->print((char)(Y2 )) ;
  _virtualPort->print((char)(Color >> 8 )) ;
  _virtualPort->print((char)(Color )) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::gfx_LinePattern(word  Pattern)
{
  _virtualPort->print((char)(F_gfx_LinePattern >> 8 )) ;
  _virtualPort->print((char)(F_gfx_LinePattern )) ;
  _virtualPort->print((char)(Pattern >> 8 )) ;
  _virtualPort->print((char)(Pattern )) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::gfx_LineTo(word  X, word  Y)
{
  _virtualPort->print((char)(F_gfx_LineTo >> 8 )) ;
  _virtualPort->print((char)(F_gfx_LineTo )) ;
  _virtualPort->print((char)(X >> 8 )) ;
  _virtualPort->print((char)(X )) ;
  _virtualPort->print((char)(Y >> 8 )) ;
  _virtualPort->print((char)(Y )) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::gfx_MoveTo(word  X, word  Y)
{
  _virtualPort->print((char)(F_gfx_MoveTo >> 8 )) ;
  _virtualPort->print((char)(F_gfx_MoveTo )) ;
  _virtualPort->print((char)(X >> 8 )) ;
  _virtualPort->print((char)(X )) ;
  _virtualPort->print((char)(Y >> 8 )) ;
  _virtualPort->print((char)(Y )) ;
  GetAck() ;
}

word Goldelox_Serial_4DLib::gfx_Orbit(word  Angle, word  Distance, word *  Xdest, word *  Ydest)
{
  _virtualPort->print((char)(F_gfx_Orbit >> 8 )) ;
  _virtualPort->print((char)(F_gfx_Orbit )) ;
  _virtualPort->print((char)(Angle >> 8 )) ;
  _virtualPort->print((char)(Angle )) ;
  _virtualPort->print((char)(Distance >> 8 )) ;
  _virtualPort->print((char)(Distance )) ;
  GetAck2Words(Xdest,Ydest) ;
  return 0 ;
}

void Goldelox_Serial_4DLib::gfx_OutlineColour(word  Color)
{
  _virtualPort->print((char)(F_gfx_OutlineColour >> 8 )) ;
  _virtualPort->print((char)(F_gfx_OutlineColour )) ;
  _virtualPort->print((char)(Color >> 8 )) ;
  _virtualPort->print((char)(Color )) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::gfx_Polygon(word  n, t4DWordArray  Xvalues, t4DWordArray  Yvalues, word  Color)
{
  _virtualPort->print((char)(F_gfx_Polygon >> 8 )) ;
  _virtualPort->print((char)(F_gfx_Polygon )) ;
  _virtualPort->print((char)(n >> 8 )) ;
  _virtualPort->print((char)(n )) ;
  WriteWords(Xvalues, n) ;
  WriteWords(Yvalues, n) ;
  _virtualPort->print((char)(Color >> 8 )) ;
  _virtualPort->print((char)(Color )) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::gfx_Polyline(word  n, t4DWordArray  Xvalues, t4DWordArray  Yvalues, word  Color)
{
  _virtualPort->print((char)(F_gfx_Polyline >> 8 )) ;
  _virtualPort->print((char)(F_gfx_Polyline )) ;
  _virtualPort->print((char)(n >> 8 )) ;
  _virtualPort->print((char)(n )) ;
  WriteWords(Xvalues, n) ;
  WriteWords(Yvalues, n) ;
  _virtualPort->print((char)(Color >> 8 )) ;
  _virtualPort->print((char)(Color )) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::gfx_PutPixel(word  X, word  Y, word  Color)
{
  _virtualPort->print((char)(F_gfx_PutPixel >> 8 )) ;
  _virtualPort->print((char)(F_gfx_PutPixel )) ;
  _virtualPort->print((char)(X >> 8 )) ;
  _virtualPort->print((char)(X )) ;
  _virtualPort->print((char)(Y >> 8 )) ;
  _virtualPort->print((char)(Y )) ;
  _virtualPort->print((char)(Color >> 8 )) ;
  _virtualPort->print((char)(Color )) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::gfx_Rectangle(word  X1, word  Y1, word  X2, word  Y2, word  Color)
{
  _virtualPort->print((char)(F_gfx_Rectangle >> 8 )) ;
  _virtualPort->print((char)(F_gfx_Rectangle )) ;
  _virtualPort->print((char)(X1 >> 8 )) ;
  _virtualPort->print((char)(X1 )) ;
  _virtualPort->print((char)(Y1 >> 8 )) ;
  _virtualPort->print((char)(Y1 )) ;
  _virtualPort->print((char)(X2 >> 8 )) ;
  _virtualPort->print((char)(X2 )) ;
  _virtualPort->print((char)(Y2 >> 8 )) ;
  _virtualPort->print((char)(Y2 )) ;
  _virtualPort->print((char)(Color >> 8 )) ;
  _virtualPort->print((char)(Color )) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::gfx_RectangleFilled(word  X1, word  Y1, word  X2, word  Y2, word  Color)
{
  _virtualPort->print((char)(F_gfx_RectangleFilled >> 8 )) ;
  _virtualPort->print((char)(F_gfx_RectangleFilled )) ;
  _virtualPort->print((char)(X1 >> 8 )) ;
  _virtualPort->print((char)(X1 )) ;
  _virtualPort->print((char)(Y1 >> 8 )) ;
  _virtualPort->print((char)(Y1 )) ;
  _virtualPort->print((char)(X2 >> 8 )) ;
  _virtualPort->print((char)(X2 )) ;
  _virtualPort->print((char)(Y2 >> 8 )) ;
  _virtualPort->print((char)(Y2 )) ;
  _virtualPort->print((char)(Color >> 8 )) ;
  _virtualPort->print((char)(Color )) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::gfx_ScreenMode(word  ScreenMode)
{
  _virtualPort->print((char)(F_gfx_ScreenMode >> 8 )) ;
  _virtualPort->print((char)(F_gfx_ScreenMode )) ;
  _virtualPort->print((char)(ScreenMode >> 8 )) ;
  _virtualPort->print((char)(ScreenMode )) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::gfx_Set(word  Func, word  Value)
{
  _virtualPort->print((char)(F_gfx_Set >> 8 )) ;
  _virtualPort->print((char)(F_gfx_Set )) ;
  _virtualPort->print((char)(Func >> 8 )) ;
  _virtualPort->print((char)(Func )) ;
  _virtualPort->print((char)(Value >> 8 )) ;
  _virtualPort->print((char)(Value )) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::gfx_SetClipRegion()
{
  _virtualPort->print((char)(F_gfx_SetClipRegion >> 8)) ;
  _virtualPort->print((char)(F_gfx_SetClipRegion)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::gfx_Transparency(word  OnOff)
{
  _virtualPort->print((char)(F_gfx_Transparency >> 8)) ;
  _virtualPort->print((char)(F_gfx_Transparency)) ;
  _virtualPort->print((char)(OnOff >> 8)) ;
  _virtualPort->print((char)(OnOff)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::gfx_TransparentColour(word  Color)
{
  _virtualPort->print((char)(F_gfx_TransparentColour >> 8)) ;
  _virtualPort->print((char)(F_gfx_TransparentColour)) ;
  _virtualPort->print((char)(Color >> 8)) ;
  _virtualPort->print((char)(Color)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::gfx_Triangle(word  X1, word  Y1, word  X2, word  Y2, word  X3, word  Y3, word  Color)
{
  _virtualPort->print((char)(F_gfx_Triangle >> 8)) ;
  _virtualPort->print((char)(F_gfx_Triangle)) ;
  _virtualPort->print((char)(X1 >> 8)) ;
  _virtualPort->print((char)(X1)) ;
  _virtualPort->print((char)(Y1 >> 8)) ;
  _virtualPort->print((char)(Y1)) ;
  _virtualPort->print((char)(X2 >> 8)) ;
  _virtualPort->print((char)(X2)) ;
  _virtualPort->print((char)(Y2 >> 8)) ;
  _virtualPort->print((char)(Y2)) ;
  _virtualPort->print((char)(X3 >> 8)) ;
  _virtualPort->print((char)(X3)) ;
  _virtualPort->print((char)(Y3 >> 8)) ;
  _virtualPort->print((char)(Y3)) ;
  _virtualPort->print((char)(Color >> 8)) ;
  _virtualPort->print((char)(Color)) ;
  GetAck() ;
}

word Goldelox_Serial_4DLib::media_Flush()
{
  _virtualPort->print((char)(F_media_Flush >> 8)) ;
  _virtualPort->print((char)(F_media_Flush)) ;
  return GetAckResp() ;
}

void Goldelox_Serial_4DLib::media_Image(word  X, word  Y)
{
  _virtualPort->print((char)(F_media_Image >> 8)) ;
  _virtualPort->print((char)(F_media_Image)) ;
  _virtualPort->print((char)(X >> 8)) ;
  _virtualPort->print((char)(X)) ;
  _virtualPort->print((char)(Y >> 8)) ;
  _virtualPort->print((char)(Y)) ;
  GetAck() ;
}

word Goldelox_Serial_4DLib::media_Init()
{
  _virtualPort->print((char)(F_media_Init >> 8)) ;
  _virtualPort->print((char)(F_media_Init)) ;
  return GetAckResp() ;
}

word Goldelox_Serial_4DLib::media_ReadByte()
{
  _virtualPort->print((char)(F_media_ReadByte >> 8)) ;
  _virtualPort->print((char)(F_media_ReadByte)) ;
  return GetAckResp() ;
}

word Goldelox_Serial_4DLib::media_ReadWord()
{
  _virtualPort->print((char)(F_media_ReadWord >> 8)) ;
  _virtualPort->print((char)(F_media_ReadWord)) ;
  return GetAckResp() ;
}

void Goldelox_Serial_4DLib::media_SetAdd(word  HiWord, word  LoWord)
{
  _virtualPort->print((char)(F_media_SetAdd >> 8)) ;
  _virtualPort->print((char)(F_media_SetAdd)) ;
  _virtualPort->print((char)(HiWord >> 8)) ;
  _virtualPort->print((char)(HiWord)) ;
  _virtualPort->print((char)(LoWord >> 8)) ;
  _virtualPort->print((char)(LoWord)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::media_SetSector(word  HiWord, word  LoWord)
{
  _virtualPort->print((char)(F_media_SetSector >> 8)) ;
  _virtualPort->print((char)(F_media_SetSector)) ;
  _virtualPort->print((char)(HiWord >> 8)) ;
  _virtualPort->print((char)(HiWord)) ;
  _virtualPort->print((char)(LoWord >> 8)) ;
  _virtualPort->print((char)(LoWord)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::media_Video(word  X, word  Y)
{
  _virtualPort->print((char)(F_media_Video >> 8)) ;
  _virtualPort->print((char)(F_media_Video)) ;
  _virtualPort->print((char)(X >> 8)) ;
  _virtualPort->print((char)(X)) ;
  _virtualPort->print((char)(Y >> 8)) ;
  _virtualPort->print((char)(Y)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::media_VideoFrame(word  X, word  Y, word  Framenumber)
{
  _virtualPort->print((char)(F_media_VideoFrame >> 8)) ;
  _virtualPort->print((char)(F_media_VideoFrame)) ;
  _virtualPort->print((char)(X >> 8)) ;
  _virtualPort->print((char)(X)) ;
  _virtualPort->print((char)(Y >> 8)) ;
  _virtualPort->print((char)(Y)) ;
  _virtualPort->print((char)(Framenumber >> 8)) ;
  _virtualPort->print((char)(Framenumber)) ;
  GetAck() ;
}

word Goldelox_Serial_4DLib::media_WriteByte(word  Byte)
{
  _virtualPort->print((char)(F_media_WriteByte >> 8)) ;
  _virtualPort->print((char)(F_media_WriteByte)) ;
  _virtualPort->print((char)(Byte >> 8)) ;
  _virtualPort->print((char)(Byte)) ;
  return GetAckResp() ;
}

word Goldelox_Serial_4DLib::media_WriteWord(word  Word)
{
  _virtualPort->print((char)(F_media_WriteWord >> 8)) ;
  _virtualPort->print((char)(F_media_WriteWord)) ;
  _virtualPort->print((char)(Word >> 8)) ;
  _virtualPort->print((char)(Word)) ;
  return GetAckResp() ;
}

void Goldelox_Serial_4DLib::putCH(word  WordChar)
{
  _virtualPort->print((char)(F_putCH >> 8)) ;
  _virtualPort->print((char)(F_putCH)) ;
  _virtualPort->print((char)(WordChar >> 8)) ;
  _virtualPort->print((char)(WordChar)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::putstr(char *  InString)
{
  _virtualPort->print((char)(F_putstr >> 8)) ;
  _virtualPort->print((char)(F_putstr)) ;
  WriteChars(InString) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::txt_Attributes(word  Attribs)
{
  _virtualPort->print((char)(F_txt_Attributes >> 8)) ;
  _virtualPort->print((char)(F_txt_Attributes)) ;
  _virtualPort->print((char)(Attribs >> 8)) ;
  _virtualPort->print((char)(Attribs)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::txt_BGcolour(word  Color)
{
  _virtualPort->print((char)(F_txt_BGcolour >> 8)) ;
  _virtualPort->print((char)(F_txt_BGcolour)) ;
  _virtualPort->print((char)(Color >> 8)) ;
  _virtualPort->print((char)(Color)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::txt_Bold(word  Bold)
{
  _virtualPort->print((char)(F_txt_Bold >> 8)) ;
  _virtualPort->print((char)(F_txt_Bold)) ;
  _virtualPort->print((char)(Bold >> 8)) ;
  _virtualPort->print((char)(Bold)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::txt_FGcolour(word  Color)
{
  _virtualPort->print((char)(F_txt_FGcolour >> 8)) ;
  _virtualPort->print((char)(F_txt_FGcolour)) ;
  _virtualPort->print((char)(Color >> 8)) ;
  _virtualPort->print((char)(Color)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::txt_FontID(word  FontNumber)
{
  _virtualPort->print((char)(F_txt_FontID >> 8)) ;
  _virtualPort->print((char)(F_txt_FontID)) ;
  _virtualPort->print((char)(FontNumber >> 8)) ;
  _virtualPort->print((char)(FontNumber)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::txt_Height(word  Multiplier)
{
  _virtualPort->print((char)(F_txt_Height >> 8)) ;
  _virtualPort->print((char)(F_txt_Height)) ;
  _virtualPort->print((char)(Multiplier >> 8)) ;
  _virtualPort->print((char)(Multiplier)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::txt_Inverse(word  Inverse)
{
  _virtualPort->print((char)(F_txt_Inverse >> 8)) ;
  _virtualPort->print((char)(F_txt_Inverse)) ;
  _virtualPort->print((char)(Inverse >> 8)) ;
  _virtualPort->print((char)(Inverse)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::txt_Italic(word  Italic)
{
  _virtualPort->print((char)(F_txt_Italic >> 8)) ;
  _virtualPort->print((char)(F_txt_Italic)) ;
  _virtualPort->print((char)(Italic >> 8)) ;
  _virtualPort->print((char)(Italic)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::txt_MoveCursor(word  Line, word  Column)
{
  _virtualPort->print((char)(F_txt_MoveCursor >> 8)) ;
  _virtualPort->print((char)(F_txt_MoveCursor)) ;
  _virtualPort->print((char)(Line >> 8)) ;
  _virtualPort->print((char)(Line)) ;
  _virtualPort->print((char)(Column >> 8)) ;
  _virtualPort->print((char)(Column)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::txt_Opacity(word  TransparentOpaque)
{
  _virtualPort->print((char)(F_txt_Opacity >> 8)) ;
  _virtualPort->print((char)(F_txt_Opacity)) ;
  _virtualPort->print((char)(TransparentOpaque >> 8)) ;
  _virtualPort->print((char)(TransparentOpaque)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::txt_Set(word  Func, word  Value)
{
  _virtualPort->print((char)(F_txt_Set >> 8)) ;
  _virtualPort->print((char)(F_txt_Set)) ;
  _virtualPort->print((char)(Func >> 8)) ;
  _virtualPort->print((char)(Func)) ;
  _virtualPort->print((char)(Value >> 8)) ;
  _virtualPort->print((char)(Value)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::txt_Underline(word  Underline)
{
  _virtualPort->print((char)(F_txt_Underline >> 8)) ;
  _virtualPort->print((char)(F_txt_Underline)) ;
  _virtualPort->print((char)(Underline >> 8)) ;
  _virtualPort->print((char)(Underline)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::txt_Width(word  Multiplier)
{
  _virtualPort->print((char)(F_txt_Width >> 8)) ;
  _virtualPort->print((char)(F_txt_Width)) ;
  _virtualPort->print((char)(Multiplier >> 8)) ;
  _virtualPort->print((char)(Multiplier)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::txt_Xgap(word  Pixels)
{
  _virtualPort->print((char)(F_txt_Xgap >> 8)) ;
  _virtualPort->print((char)(F_txt_Xgap)) ;
  _virtualPort->print((char)(Pixels >> 8)) ;
  _virtualPort->print((char)(Pixels)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::txt_Ygap(word  Pixels)
{
  _virtualPort->print((char)(F_txt_Ygap >> 8)) ;
  _virtualPort->print((char)(F_txt_Ygap)) ;
  _virtualPort->print((char)(Pixels >> 8)) ;
  _virtualPort->print((char)(Pixels)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::BeeP(word  Note, word  Duration)
{
  _virtualPort->print((char)(F_BeeP >> 8)) ;
  _virtualPort->print((char)(F_BeeP)) ;
  _virtualPort->print((char)(Note >> 8)) ;
  _virtualPort->print((char)(Note)) ;
  _virtualPort->print((char)(Duration >> 8)) ;
  _virtualPort->print((char)(Duration)) ;
  GetAck() ;
}

word Goldelox_Serial_4DLib::sys_GetModel(char *  ModelStr)
{
  _virtualPort->print((char)(F_sys_GetModel >> 8)) ;
  _virtualPort->print((char)(F_sys_GetModel)) ;
  return GetAckResStr(ModelStr) ;
}

word Goldelox_Serial_4DLib::sys_GetVersion()
{
  _virtualPort->print((char)(F_sys_GetVersion >> 8)) ;
  _virtualPort->print((char)(F_sys_GetVersion)) ;
  return GetAckResp() ;
}

word Goldelox_Serial_4DLib::sys_GetPmmC()
{
  _virtualPort->print((char)(F_sys_GetPmmC >> 8)) ;
  _virtualPort->print((char)(F_sys_GetPmmC)) ;
  return GetAckResp() ;
}

void Goldelox_Serial_4DLib::blitComtoDisplay(word  X, word  Y, word  Width, word  Height, t4DByteArray  Pixels)
{
  _virtualPort->print((char)(F_blitComtoDisplay >> 8)) ;
  _virtualPort->print((char)(F_blitComtoDisplay)) ;
  _virtualPort->print((char)(X >> 8)) ;
  _virtualPort->print((char)(X)) ;
  _virtualPort->print((char)(Y >> 8)) ;
  _virtualPort->print((char)(Y)) ;
  _virtualPort->print((char)(Width >> 8)) ;
  _virtualPort->print((char)(Width)) ;
  _virtualPort->print((char)(Height >> 8)) ;
  _virtualPort->print((char)(Height)) ;
  WriteBytes(Pixels, Width*Height*2) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::setDisplayBaud(word  Newrate)
{
  _virtualPort->print((char)(F_setbaudWait >> 8)) ;
  _virtualPort->print((char)(F_setbaudWait)) ;
  _virtualPort->print((char)(Newrate >> 8)) ;
  _virtualPort->print((char)(Newrate)) ;
}

void Goldelox_Serial_4DLib::setbaudWait(word  Newrate)
{
  _virtualPort->print((char)(F_setbaudWait >> 8)) ;
  _virtualPort->print((char)(F_setbaudWait)) ;
  _virtualPort->print((char)(Newrate >> 8)) ;
  _virtualPort->print((char)(Newrate)) ;
  SetThisBaudrate(Newrate) ; // change this systems baud rate to match new display rate, ACK is 100ms away
  GetAck() ;
}

word Goldelox_Serial_4DLib::peekW(word  Address)
{
  _virtualPort->print((char)(F_peekW >> 8)) ;
  _virtualPort->print((char)(F_peekW)) ;
  _virtualPort->print((char)(Address >> 8)) ;
  _virtualPort->print((char)(Address)) ;
  return GetAckResp() ;
}

void Goldelox_Serial_4DLib::pokeW(word  Address, word  WordValue)
{
  _virtualPort->print((char)(F_pokeW >> 8)) ;
  _virtualPort->print((char)(F_pokeW)) ;
  _virtualPort->print((char)(Address >> 8)) ;
  _virtualPort->print((char)(Address)) ;
  _virtualPort->print((char)(WordValue >> 8)) ;
  _virtualPort->print((char)(WordValue)) ;
  GetAck() ;
}

word Goldelox_Serial_4DLib::peekB(word  Address)
{
  _virtualPort->print((char)(F_peekB >> 8)) ;
  _virtualPort->print((char)(F_peekB)) ;
  _virtualPort->print((char)(Address >> 8)) ;
  _virtualPort->print((char)(Address)) ;
  return GetAckResp() ;
}

void Goldelox_Serial_4DLib::pokeB(word  Address, word  ByteValue)
{
  _virtualPort->print((char)(F_pokeB >> 8)) ;
  _virtualPort->print((char)(F_pokeB)) ;
  _virtualPort->print((char)(Address >> 8)) ;
  _virtualPort->print((char)(Address)) ;
  _virtualPort->print((char)(ByteValue >> 8)) ;
  _virtualPort->print((char)(ByteValue)) ;
  GetAck() ;
}

word Goldelox_Serial_4DLib::joystick()
{
  _virtualPort->print((char)(F_joystick >> 8)) ;
  _virtualPort->print((char)(F_joystick)) ;
  return GetAckResp() ;
}

void Goldelox_Serial_4DLib::SSTimeout(word  Seconds)
{
  _virtualPort->print((char)(F_SSTimeout >> 8)) ;
  _virtualPort->print((char)(F_SSTimeout)) ;
  _virtualPort->print((char)(Seconds >> 8)) ;
  _virtualPort->print((char)(Seconds)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::SSSpeed(word  Speed)
{
  _virtualPort->print((char)(F_SSSpeed >> 8)) ;
  _virtualPort->print((char)(F_SSSpeed)) ;
  _virtualPort->print((char)(Speed >> 8)) ;
  _virtualPort->print((char)(Speed)) ;
  GetAck() ;
}

void Goldelox_Serial_4DLib::SSMode(word  Parm)
{
  _virtualPort->print((char)(F_SSMode >> 8)) ;
  _virtualPort->print((char)(F_SSMode)) ;
  _virtualPort->print((char)(Parm >> 8)) ;
  _virtualPort->print((char)(Parm)) ;
  GetAck() ;
}
