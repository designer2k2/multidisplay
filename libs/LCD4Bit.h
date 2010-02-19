#ifndef LCD4Bit_h
#define LCD4Bit_h

#include <inttypes.h>

class LCD4Bit {
public:
  LCD4Bit(int num_lines);
  void commandWrite(int value);
  void init();

  void print(int value);
  void print_P(int value);
  void print_P(uint8_t value);
  void print_P(uint8_t value[], uint8_t len);

  void printIn(char value[]);
  void printIn_P(char value[]);

  void printInBlank();

  void clear();
  //non-core---------------
  void cursorTo(int line_num, int x);
  void leftScroll(int chars, int delay_time);

  void lcdCommandWriteAndPrintIn_P(uint8_t cmd, char *chars);
  //end of non-core--------

  //4bit only, therefore ideally private but may be needed by user
  void commandWriteNibble(int nibble);
private:
  void pulseEnablePin();
  void pushNibble(int nibble);
  void pushByte(int value);
};

#endif
