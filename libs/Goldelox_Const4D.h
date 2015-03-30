// undef windows definitions that are also 4DGL definitions
#ifdef TRANSPARENT
    #undef TRANSPARENT
#endif
#ifdef OPAQUE
    #undef OPAQUE
#endif
#ifdef BAUD_110
    #undef BAUD_110
#endif
#ifdef BAUD_300
    #undef BAUD_300
#endif
#ifdef BAUD_600
    #undef BAUD_600
#endif
#ifdef BAUD_1200
    #undef BAUD_1200
#endif
#ifdef BAUD_2400
    #undef BAUD_2400
#endif
#ifdef BAUD_4800
    #undef BAUD_4800
#endif
#ifdef BAUD_9600
    #undef BAUD_9600
#endif
#ifdef BAUD_14400
    #undef BAUD_14400
#endif
#ifdef BAUD_19200
    #undef BAUD_19200
#endif
#ifdef BAUD_38400
    #undef BAUD_38400
#endif
#ifdef BAUD_57600
    #undef BAUD_57600
#endif
#ifdef BAUD_115200
    #undef BAUD_115200
#endif

#ifdef TEXT
    #undef TEXT
#endif

/*
    //----------------------------------
    //   4DGL - GOLDELOX-GFX2 PLATFORM
    //----------------------------------


*/
/*
    System Primitives for 4DGL GOLDELOX GFX2 Platform
*/


//==============================================//
// initialization table additional commands     //
//==============================================//
#define INIT_TABLE_END          0xFF // end of initialization table marker
#define INIT_TABLE_DELAY        0xFE // insert delay value
#define INIT_TABLE_16BIT_INDEX_REG_MODE  0xFD // 16 bit display index register
#define INIT_TABLE_8BIT_INDEX_REG_MODE  0xFC // 8 bit display index register
#define INIT_TABLE_16BIT_DUMMY_READ  0xFB // 16 bit display dummy read
#define INIT_TABLE_PORTRAIT     0xFA // initial screen mode requirement
#define INIT_TABLE_LANDSCAPE    0xF9
#define INIT_TABLE_PORTRAIT_R   0xF8
#define INIT_TABLE_LANDSCAPE_R  0xF7

//==============================================//
// putch, putstr, putnum and print redirection  //
// constants                                    //
//==============================================//
#define APPEND                  0x0000
#define COM0                    0xFF04
#define TEXT                    0xFF08
#define MDA                     0xFF10

//==============================================//
// general number formatting constants          //
// for 'print' and 'putnum'                     //
//==============================================//
    // string processing constants
#define STR                     0x0080 // display as string
#define CHR                     0x0081 // display as single char



  // binary, no leading zeroes
#define BIN4d                     0x0002 // binary, 16 digits, no leading zeroes
#define BIN1                    0x0102 // binary, 1 digit, no leading zeroes
#define BIN2                    0x0202 // binary, 2 digits, no leading zeroes
#define BIN3                    0x0302 // binary, 3 digits, no leading zeroes
#define BIN4                    0x0402 // binary, 4 digits, no leading zeroes
#define BIN5                    0x0502 // binary, 5 digits, no leading zeroes
#define BIN6                    0x0602 // binary, 6 digits, no leading zeroes
#define BIN7                    0x0702 // binary, 7 digits, no leading zeroes
#define BIN8                    0x0802 // binary, 8 digits, no leading zeroes
#define BIN9                    0x0902 // binary, 9 digits, no leading zeroes
#define BIN10                   0x0A02 // binary, 10 digits, no leading zeroes
#define BIN11                   0x0B02 // binary, 11 digits, no leading zeroes
#define BIN12                   0x0C02 // binary, 12 digits, no leading zeroes
#define BIN13                   0x0D02 // binary, 13 digits, no leading zeroes
#define BIN14                   0x0E02 // binary, 14 digits, no leading zeroes
#define BIN15                   0x0F02 // binary, 15 digits, no leading zeroes
#define BIN16                   0x0002 // binary, 16 digits, no leading zeroes

    // binary, with leading zeroes
#define BINZ                    0x1002 // binary, 16 digits, leading zeroes
#define BIN1Z                   0x1102 // binary, 1 digit, leading zeroes
#define BIN2Z                   0x1202 // binary, 2 digits, leading zeroes
#define BIN3Z                   0x1302 // binary, 3 digits, leading zeroes
#define BIN4Z                   0x1402 // binary, 4 digits, leading zeroes
#define BIN5Z                   0x1502 // binary, 5 digits, leading zeroes
#define BIN6Z                   0x1602 // binary, 6 digits, leading zeroes
#define BIN7Z                   0x1702 // binary, 7 digits, leading zeroes
#define BIN8Z                   0x1802 // binary, 8 digits, leading zeroes
#define BIN9Z                   0x1902 // binary, 9 digits, leading zeroes
#define BIN10Z                  0x1A02 // binary, 10 digits, leading zeroes
#define BIN11Z                  0x1B02 // binary, 11 digits, leading zeroes
#define BIN12Z                  0x1C02 // binary, 12 digits, leading zeroes
#define BIN13Z                  0x1D02 // binary, 13 digits, leading zeroes
#define BIN14Z                  0x1E02 // binary, 14 digits, leading zeroes
#define BIN15Z                  0x1F02 // binary, 15 digits, leading zeroes
#define BIN16Z                  0x1002 // binary, 16 digits, leading zeroes

    // binary, with leading blanked
#define BINZB                   0x2002 // binary, 16 digits, leading blanks
#define BIN1ZB                  0x2102 // binary, 1 digit, leading blanks
#define BIN2ZB                  0x2202 // binary, 2 digits, leading blanks
#define BIN3ZB                  0x2302 // binary, 3 digits, leading blanks
#define BIN4ZB                  0x2402 // binary, 4 digits, leading blanks
#define BIN5ZB                  0x2502 // binary, 5 digits, leading blanks
#define BIN6ZB                  0x2602 // binary, 6 digits, leading blanks
#define BIN7ZB                  0x2702 // binary, 7 digits, leading blanks
#define BIN8ZB                  0x2802 // binary, 8 digits, leading blanks
#define BIN9ZB                  0x2902 // binary, 9 digits, leading blanks
#define BIN10ZB                 0x2A02 // binary, 10 digits, leading blanks
#define BIN11ZB                 0x2B02 // binary, 11 digits, leading blanks
#define BIN12ZB                 0x2C02 // binary, 12 digits, leading blanks
#define BIN13ZB                 0x2D02 // binary, 13 digits, leading blanks
#define BIN14ZB                 0x2E02 // binary, 14 digits, leading blanks
#define BIN15ZB                 0x2F02 // binary, 15 digits, leading blanks
#define BIN16ZB                 0x2002 // binary, 16 digits, leading blanks

    // signed decimal, no leading zeroes
#define DEC4d                   0x050A // signed decimal, 5 digits, no leading zeroes
#define DEC1                    0x010A // signed decimal, 1 digit, no leading zeroes
#define DEC2                    0x020A // signed decimal, 2 digits, no leading zeroes
#define DEC3                    0x030A // signed decimal, 3 digits, no leading zeroes
#define DEC4                    0x040A // signed decimal, 4 digits, no leading zeroes
#define DEC5                    0x050A // signed decimal, 5 digits, no leading zeroes

    // signed decimal, with leading zeroes
#define DECZ                    0x150A // signed decimal, 5 digits, leading zeroes
#define DEC1Z                   0x110A // signed decimal, 1 digit, leading zeroes
#define DEC2Z                   0x120A // signed decimal, 2 digits, leading zeroes
#define DEC3Z                   0x130A // signed decimal, 3 digits, leading zeroes
#define DEC4Z                   0x140A // signed decimal, 4 digits, leading zeroes
#define DEC5Z                   0x150A // signed decimal, 5 digits, leading zeroes

  // signed decimal, leading zeroes blanked
#define DECZB                   0x250A // signed decimal, 5 digits, leading blanks
#define DEC1ZB                  0x210A // signed decimal, 1 digit, leading blanks
#define DEC2ZB                  0x220A // signed decimal, 2 digits, leading blanks
#define DEC3ZB                  0x230A // signed decimal, 3 digits, leading blanks
#define DEC4ZB                  0x240A // signed decimal, 4 digits, leading blanks
#define DEC5ZB                  0x250A // signed decimal, 5 digits, leading blanks

    // unsigned decimal, no leading zeroes
#define UDEC                    0x450A // unsigned decimal, 5 digits, no leading zeroes
#define UDEC1                   0x410A // unsigned decimal, 1 digit, no leading zeroes
#define UDEC2                   0x420A // unsigned decimal, 2 digits, no leading zeroes
#define UDEC3                   0x430A // unsigned decimal, 3 digits, no leading zeroes
#define UDEC4                   0x440A // unsigned decimal, 4 digits, no leading zeroes
#define UDEC5                   0x450A // unsigned decimal, 5 digits, no leading zeroes

    // unsigned decimal, with leading zero's
#define UDECZ                   0x550A // unsigned decimal, 5 digits, leading zeroes
#define UDEC1Z                  0x510A // unsigned decimal, 1 digit, leading zeroes
#define UDEC2Z                  0x520A // unsigned decimal, 2 digits, leading zeroes
#define UDEC3Z                  0x530A // unsigned decimal, 3 digits, leading zeroes
#define UDEC4Z                  0x540A // unsigned decimal, 4 digits, leading zeroes
#define UDEC5Z                  0x550A // unsigned decimal, 5 digits, leading zeroes

    // unsigned decimal, leading zeroes blanked
#define UDECZB                  0x650A // unsigned decimal, 5 digits, leading blanks
#define UDEC1ZB                 0x610A // unsigned decimal, 1 digit, leading blanks
#define UDEC2ZB                 0x620A // unsigned decimal, 2 digits, leading blanks
#define UDEC3ZB                 0x630A // unsigned decimal, 3 digits, leading blanks
#define UDEC4ZB                 0x640A // unsigned decimal, 4 digits, leading blanks
#define UDEC5ZB                 0x650A // unsigned decimal, 5 digits, leading blanks

    // hex, with leading zero's
#define HEX4d                   0x1410 // hex, 4 digits, leading zeroes
#define HEX1                    0x1110 // hex, 1 digit, leading zeroes
#define HEX2                    0x1210 // hex, 2 digits, leading zeroes
#define HEX3                    0x1310 // hex, 3 digits, leading zeroes
#define HEX4                    0x1410 // hex, 4 digits, leading zeroes

    // hex, no leading zero's
#define HEXZ                    0x0410 // hex, 4 digits, no leading zeroes
#define HEX1Z                   0x0110 // hex, 1 digit, no leading zeroes
#define HEX2Z                   0x0210 // hex, 2 digits, no leading zeroes
#define HEX3Z                   0x0310 // hex, 3 digits, no leading zeroes
#define HEX4Z                   0x0410 // hex, 4 digits, no leading zeroes

    // hex, leading zero's blanked
#define HEXZB                   0x2410 // hex, 4 digits, leading blanks
#define HEX1ZB                  0x2110 // hex, 1 digit, leading blanks
#define HEX2ZB                  0x2210 // hex, 2 digits, leading blanks
#define HEX3ZB                  0x2310 // hex, 3 digits, leading blanks
#define HEX4ZB                  0x2410 // hex, 4 digits, leading blanks

// baud divisor rates
#define MIDI                    95
#define BAUD_110                27271
#define BAUD_300                9999
#define BAUD_600                4999
#define BAUD_1200               2499
#define BAUD_2400               1249
#define BAUD_4800               624
#define BAUD_9600               312
#define BAUD_14400              207
#define BAUD_19200              155
#define BAUD_31250              95
#define BAUD_38400              77
#define BAUD_56000              53
#define BAUD_57600              51
#define BAUD_115200             25
#define BAUD_128000             22
#define BAUD_256000             11
#define BAUD_300000             10
#define BAUD_375000             8
#define BAUD_500000             6
#define BAUD_600000             4


//==============================================//
// generic constants                            //
//==============================================//
#define ENABLE                  1
#define DISABLE                 0
#define ON                      1
#define OFF                     0
#define TRUE                    1
#define FALSE                   0

//==============================================//
// joystick() function return constants         //
//==============================================//
#define RELEASED                0
#define UP                      1
#define LEFT                    2
#define DOWN                    3
#define RIGHT                   4
#define FIRE                    5

//==============================================//
// spi_Init(...)  mode arguments                //
//==============================================//
#define SPI_SLOW                2 // 750khz
#define SPI_MED                 1 // 3mhz
#define SPI_FAST                0 // 12mhz
#define RXMODE_0                0 // receive sample pos
#define RXMODE_1                1 // receive sample pos
#define CKMODE_0                0 // transmit sample pos
#define CKMODE_1                1 // transmit sample pos
#define CKMODE_2                2 // transmit sample pos
#define CKMODE_3                3 // transmit sample pos

//==============================================//
//Pin related constants                         //
//==============================================//
#define IO1                     0
#define IO2                     1
#define LO                      0
#define HI                      1
#define OUTPUT4d                0
#define INPUT4d                 1
#define ANALOGUE_8              2
#define ANALOGUE_10             3
#define ONEWIRE                 4
#define SOUND                   5

//==============================================//
//gfx_Set() related constants                   //
//==============================================//
#define PEN_SIZE                0 // not necessary to use (legacy mode)
#define BACKGROUND_COLOUR       1 // screen background colour
#define OBJECT_COLOUR           2 // line / circle / rectangle generic colour
#define CLIPPING                3 // clipping ON / OFF
#define TRANSPARENT_COLOUR      4 // (only on displays with specific hware feature)
#define TRANSPARENCY            5 // 0 = OFF, 1 = ON (only on displays with specific hware feature)
#define FRAME_DELAY             6 // legacy mode, see pokeB(IMAGE_DELAY, n);
#define SCREEN_MODE             7 // LANDSCAPE, LANDSCAPE_R, PORTRAIT, PORTRAIT_R
#define OUTLINE_COLOUR          8 // if not BLACK (0) , used for outline around circles and rectanglse
#define CONTRAST                9 // set contrast ratuio in uOLED modules
#define LINE_PATTERN            10 // used for patterned lines, 16bit value (0 = no pattern)
#define COLOUR_MODE             11 // select 8 or 16 bit colour mode


#define SOLID                   0 // PEN_SIZE argument  (gfx_Set legacy mode)
#define OUTLINE                 1 // PEN_SIZE argument  (gfx_Set legacy mode)

#define LANDSCAPE               0 // SCREEN_MODE (north)
#define LANDSCAPE_R             1 // SCREEN_MODE (south)
#define PORTRAIT                2 // SCREEN_MODE (west)
#define PORTRAIT_R              3 // SCREEN_MODE (east)

#define COLOUR8                 1 // COLOUR_MODE 8 bit colour mode
#define COLOUR16                0 // COLOUR_MODE 16 bit colour mode

//==============================================//
//txt_Set() related constants                   //
//==============================================//
#define TEXT_COLOUR             0 // text foreground colr
#define TEXT_HIGHLIGHT          1 // text background colr
#define FONT_ID                 2 // default 0, else points to data statement font
#define TEXT_WIDTH              3 // text width multiplier
#define TEXT_HEIGHT             4 // text height multiplier
#define TEXT_XGAP               5 // horizontal text gap (default 1)
#define TEXT_YGAP               6 // vertical text gap (default 1)
#define TEXT_PRINTDELAY         7 // for 'teletype' like effect when pronting
#define TEXT_OPACITY            8 // text mode flag, TRANSPARENT or OPAQUE
#define TEXT_BOLD               9 // embolden text (auto reset) - legacy
#define TEXT_ITALIC             10 // italicize text (auto reset)
#define TEXT_INVERSE            11 // invert text (auto reset)
#define TEXT_UNDERLINED         12 // underline text (auto reset)
#define TEXT_ATTRIBUTES         13 // controls BOLD/ITALIC/INVERSE/UNDERLINE simultaneously

//==============================================//
//txt_Set() related arguments                   //
//==============================================//
#define SYSTEM                  0 // internal system font
#define MEDIA                   7 // font is at uSD_add


#define TRANSPARENT             0 // TEXT_OPACITY  transparent  text
#define OPAQUE                  1 // TEXT_OPACITY  opaque text

#define BOLD                    16 // TEXT_ATTRIBUTES bold text (auto reset)
#define ITALIC                  32 // TEXT_ATTRIBUTES italic text (auto reset)
#define INVERSE                 64 // TEXT_ATTRIBUTES inverse text (auto reset)
#define UNDERLINED              128 // TEXT_ATTRIBUTES underlined text (auto reset)


//==============================================//
// Memory MAP                                   //
// system BYTE variables accesible with         //
// peekB and pokeB                              //
//==============================================//
#define VX1                     128 // display hardware x1 pos
#define VY1                     129 // display hardware y1 pos
#define VX2                     130 // display hardware x2 pos
#define VY2                     131 // display hardware y2 pos
#define SYS_X_MAX               132 // display hardware X res-1
#define SYS_Y_MAX               133 // display hardware Y res-1
#define WRITE_GRAM_REG          134 // device register address for write operation
#define READ_GRAM_REG           135 // device register address for write operation
#define IMAGE_WIDTH             136 // width of currently loaded image
#define IMAGE_HEIGHT            137 // height of currently loaded image
#define IMAGE_DELAY             138 // 0 if image, else inter frame delay for movie
#define IMAGE_MODE              139 // bit 4 determines colr mode, other bits reserved
#define CLIP_LEFT_POS           140 // left clipping point (set with gfx_ClipWindow(...)
#define CLIP_TOP_POS            141 // top clipping point (set with gfx_ClipWindow(...)
#define CLIP_RIGHT_POS          142 // right clipping point (set with gfx_ClipWindow(...)
#define CLIP_BOTTOM_POS         143 // bottom clipping point (set with gfx_ClipWindow(...)
#define CLIP_LEFT               144 // current clip value (reads as full size if clipping turned off)
#define CLIP_TOP                145 // current clip value (reads as full size if clipping turned off)
#define CLIP_RIGHT              146 // current clip value (reads as full size if clipping turned off)
#define CLIP_BOTTOM             147 // current clip value (reads full size if clipping turned off)
#define FONT_TYPE               148 // font type, 0 = system font, else pointer to user or media font
#define FONT_MAX                149 // number of chars in current font
#define FONT_OFFSET             150 // starting offset (normally 0x20)
#define FONT_WIDTH              151 // current font width
#define FONT_HEIGHT             152 // current font height
#define TEXT_XMAG               153 // text width magnification
#define TEXT_YMAG               154 // text height magnification
#define TEXT_MARGIN             155 // left column for carriage return
#define TEXT_DELAY              156 // print delay
#define TEXT_X_GAP              157 // text horizontal gap size
#define TEXT_Y_GAP              158 // text vertical gap size
#define GFX_XMAX                159 // current display width-1 determined by portrait / landscape swapping
#define GFX_YMAX                160 // current display width-1 determined by portrait / landscape swapping
#define GFX_SCREENMODE          161 // holds current screen mode
    //


//==============================================//
// system WORD variables accesible with peekW   //
// and pokeW or pointer access                  //
//==============================================//
#define VM_OVERFLOW             83 // 16bit overflow of 32bit results (see OVF() funtion)
#define VM_COLOUR               84 // general working var for colour
#define VM_RETVAL               85 // last value returned from any function
#define GFX_BACK_COLOUR         86 // screen background colour
#define GFX_OBJECT_COLOUR       87 // graphics object colour
#define GFX_TEXT_COLOUR         88 // text forground colour
#define GFX_TEXT_BGCOLOUR       89 // text background colour
#define GFX_OUTLINE_COLOUR      90 // screen background colour
#define GFX_LINE_PATTERN        91 // line draw 16bit linear pattern
#define IMG_PIXEL_COUNT         92 // pixel count of current object (may be altered by clipping)
#define IMG_FRAME_COUNT         93 // count of frames in currently loaded video
#define MEDIA_HEAD              94 // media sector head position
#define SYS_OSTREAM             95 // system output redirection
#define GFX_LEFT                96 // virtual left point for current image
#define GFX_TOP                 97 // virtual top point for current image
#define GFX_RIGHT               98 // virtual right point for current image
#define GFX_BOTTOM              99 // virtual bottom point for current image
#define GFX_X1                  100 // clipped left point for current image
#define GFX_Y1                  101 // clipped top point for current image
#define GFX_X2                  102 // clipped right point for current image
#define GFX_Y2                  103 // clipped bottom point for current image
#define GFX_X_ORG               104 // current Y origin
#define GFX_Y_ORG               105 // current Y origin
#define RANDOM_LO               106 // random number generator LO word
#define RANDOM_HI               107 // random number generator LO word
#define MEDIA_ADDR_LO           108 // uSD/FLASH absolute byte address LO
#define MEDIA_ADDR_HI           109 // uSD/FLASH absolute byte address HI
#define SECTOR_ADDR_LO          110 // uSD/FLASH sector address LO
#define SECTOR_ADDR_HI          111 // uSD/FLASH sector address HI
#define SYSTEM_TIMER_LO         112 // 1msec 32 bit free running timer LO word
#define SYSTEM_TIMER_HI         113 // 1msec 32 bit free running timer HI word
#define TIMER04d                114 // user timer0
#define TIMER14d                115 // user timer1
#define TIMER24d                116 // user timer2
#define TIMER34d                117 // user timer3 (10msec resolution)
#define INCVAL                  118 // incrementer size
#define TEMP_MEDIA_ADDRLO       119 // temp stroage while printing font from media storage
#define TEMP_MEDIA_ADDRHI       120 // temp stroage while printing font from media storage
#define GFX_TRANSPARENTCOLOUR   121 // image transparency colour
#define GFX_STRINGMETRIX        122 // low byte = string width, hi byte = string height
#define GFX_TEMPSTORE1          123 // low byte = last char printed, hi byte = frame timer over-ride
    // reserved                 124             // internal use
    // reserved                 125             // internal use
#define SYS_FLAGS1              126 // system control flags word 1
#define SYS_FLAGS2              127 // system control flags word 2

//==============================================//
// bit constants for SYS_FLAGS1                 //
//==============================================//
#define _STREAMLOCK             0x0001 // Used internally
#define _PENSIZE                0x0002 // object, 0=solid,1=outline
#define _OPACITY                0x0004 // text, 0=transparent,1=opaque
#define _OUTLINED               0x0008 // box/circle outline 0=off,1=on
#define _BOLD                   0x0010 // text, 0=normal,1=bold (auto reset)
#define _ITALIC                 0x0020 // Text, 0=normal,1=italic (auto reset)
#define _INVERSE                0x0040 // Text, 0=normal,1=inverse (auto reset)
#define _UNDERLINED             0x0080 // Text, 0=normal,1=underlined (auto reset)
#define _CLIPPING               0x0100 // Used internally, 0-clipping off, 1-clipping on
#define _STRMODE                0x0200 // Used internally
#define _SERMODE                0x0400 // Used internally
#define _TXTMODE                0x0800 // Used internally
#define _MEDIAMODE              0x1000 // Used internally
#define _PATTERNED              0x2000 // Used internally
#define _COLOUR8                0x4000 // Display mode, 0=16bit, 1=8bit
#define _MEDIAFONT              0x8000 // 0=internal font, 1=media font

//==============================================//
// bit constants for SYS_FLAGS2                 //
//==============================================//
#define _MEDIA_INSTALLED        0x0001 // SD of FLASH device is installed
#define _MEDIA_TYPE             0x0002 // 0=SD, 1=FLASH media device detected
#define _MEDIA_READ             0x0004 // 1=MEDIA read in progress
#define _MEDIA_WRITE            0x0008 // 1=MEDIA write in progress
#define _OW_PIN                 0x0010 // 0=IO1, 1=IO2 (Dallas OW Pin)
#define _PTR_TYPE               0x0020 // Used internally
#define _TEMP1                  0x0040 // Used internally
#define _TEMP2                  0x0080 // Used internally
#define _RUNMODE                0x0100 // 1=running pcode from media
#define _SIGNED                 0x0200 // 0=number printed '-' prepend
#define _RUNFLAG                0x0400 // 1=EVE processor is running    SYSTEM
#define _SINGLESTEP             0x0800 // 1=set breakpoint for debugger
#define _COMMINT                0x1000 // 1=buffered comms active
#define _DUMMY16                0x2000 // 1=display needs 16bit dummy
#define _DISP16                 0x4000 // 1=display is 16bit interface
#define _PROPFONT               0x8000 // 1=current font is proportional


//==============================================//
// EVE User Resources                           //
//==============================================//
#define USR_SP                  128 // EVE user defined stack pointer
#define USRVARS                 129 // EVE user variables VARS[255]
#define USRSTACK                384 // EVE machine stack STACK[128]

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/*
    16 bit RGB (565) Colour Chart
    Original work by 4D Forum Member: skadoo
*/



#define ALICEBLUE               0xF7DF
#define ANTIQUEWHITE            0xFF5A
#define AQUA                    0x07FF
#define AQUAMARINE              0x7FFA
#define AZURE                   0xF7FF
#define BEIGE                   0xF7BB
#define BISQUE                  0xFF38
#define BLACK                   0x0000
#define BLANCHEDALMOND          0xFF59
#define BLUE                    0x001F
#define BLUEVIOLET              0x895C
#define BROWN                   0xA145
#define BURLYWOOD               0xDDD0
#define CADETBLUE               0x5CF4
#define CHARTREUSE              0x7FE0
#define CHOCOLATE               0xD343
#define CORAL                   0xFBEA
#define CORNFLOWERBLUE          0x64BD
#define CORNSILK                0xFFDB
#define CRIMSON                 0xD8A7
#define CYAN                    0x07FF
#define DARKBLUE                0x0011
#define DARKCYAN                0x0451
#define DARKGOLDENROD           0xBC21
#define DARKGRAY                0xAD55
#define DARKGREEN               0x0320
#define DARKKHAKI               0xBDAD
#define DARKMAGENTA             0x8811
#define DARKOLIVEGREEN          0x5345
#define DARKORANGE              0xFC60
#define DARKORCHID              0x9999
#define DARKRED                 0x8800
#define DARKSALMON              0xECAF
#define DARKSEAGREEN            0x8DF1
#define DARKSLATEBLUE           0x49F1
#define DARKSLATEGRAY           0x2A69
#define DARKTURQUOISE           0x067A
#define DARKVIOLET              0x901A
#define DEEPPINK                0xF8B2
#define DEEPSKYBLUE             0x05FF
#define DIMGRAY                 0x6B4D
#define DODGERBLUE              0x1C9F
#define FIREBRICK               0xB104
#define FLORALWHITE             0xFFDE
#define FORESTGREEN             0x2444
#define FUCHSIA                 0xF81F
#define GAINSBORO               0xDEFB
#define GHOSTWHITE              0xFFDF
#define GOLD                    0xFEA0
#define GOLDENROD               0xDD24
#define GRAY                    0x8410
#define GREEN                   0x0400
#define GREENYELLOW             0xAFE5
#define HONEYDEW                0xF7FE
#define HOTPINK                 0xFB56
#define INDIANRED               0xCAEB
#define INDIGO                  0x4810
#define IVORY                   0xFFFE
#define KHAKI                   0xF731
#define LAVENDER                0xE73F
#define LAVENDERBLUSH           0xFF9E
#define LAWNGREEN               0x7FE0
#define LEMONCHIFFON            0xFFD9
#define LIGHTBLUE               0xAEDC
#define LIGHTCORAL              0xF410
#define LIGHTCYAN               0xE7FF
#define LIGHTGOLD               0xFFDA
#define LIGHTGREEN              0x9772
#define LIGHTGREY               0xD69A
#define LIGHTPINK               0xFDB8
#define LIGHTSALMON             0xFD0F
#define LIGHTSEAGREEN           0x2595
#define LIGHTSKYBLUE            0x867F
#define LIGHTSLATEGRAY          0x7453
#define LIGHTSTEELBLUE          0xB63B
#define LIGHTYELLOW             0xFFFC
#define LIME                    0x07E0
#define LIMEGREEN               0x3666
#define LINEN                   0xFF9C
#define MAGENTA                 0xF81F
#define MAROON                  0x8000
#define MEDIUMAQUAMARINE        0x6675
#define MEDIUMBLUE              0x0019
#define MEDIUMORCHID            0xBABA
#define MEDIUMPURPLE            0x939B
#define MEDIUMSEAGREEN          0x3D8E
#define MEDIUMSLATEBLUE         0x7B5D
#define MEDIUMSPRINGGREEN       0x07D3
#define MEDIUMTURQUOISE         0x4E99
#define MEDIUMVIOLETRED         0xC0B0
#define MIDNIGHTBLUE            0x18CE
#define MINTCREAM               0xF7FF
#define MISTYROSE               0xFF3C
#define MOCCASIN                0xFF36
#define NAVAJOWHITE             0xFEF5
#define NAVY                    0x0010
#define OLDLACE                 0xFFBC
#define OLIVE                   0x8400
#define OLIVEDRAB               0x6C64
#define ORANGE                  0xFD20
#define ORANGERED               0xFA20
#define ORCHID                  0xDB9A
#define PALEGOLDENROD           0xEF55
#define PALEGREEN               0x9FD3
#define PALETURQUOISE           0xAF7D
#define PALEVIOLETRED           0xDB92
#define PAPAYAWHIP              0xFF7A
#define PEACHPUFF               0xFED7
#define PERU                    0xCC27
#define PINK2                   0xFE19
#define PLUM                    0xDD1B
#define POWDERBLUE              0xB71C
#define PURPLE                  0x8010
#define RED                     0xF800
#define ROSYBROWN               0xBC71
#define ROYALBLUE               0x435C
#define SADDLEBROWN             0x8A22
#define SALMON                  0xFC0E
#define SANDYBROWN              0xF52C
#define SEAGREEN                0x2C4A
#define SEASHELL                0xFFBD
#define SIENNA                  0xA285
#define SILVER                  0xC618
#define SKYBLUE                 0x867D
#define SLATEBLUE               0x6AD9
#define SLATEGRAY               0x7412
#define SNOW                    0xFFDF
#define SPRINGGREEN             0x07EF
#define STEELBLUE               0x4416
#define TAN                     0xD5B1
#define TEAL                    0x0410
#define THISTLE                 0xDDFB
#define TOMATO                  0xFB08
#define TURQUOISE               0x471A
#define VIOLET                  0xEC1D
#define WHEAT                   0xF6F6
#define WHITE                   0xFFFF
#define WHITESMOKE              0xF7BE
#define YELLOW                  0xFFE0
#define YELLOWGREEN             0x9E66










