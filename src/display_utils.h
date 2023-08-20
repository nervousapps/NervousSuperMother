#ifndef Display_Utils_h
#define Display_Utils_h

#include "Arduino.h"
#include <ArduinoJson.h>
#include "SamplePlayerAudioConnections.h"
#include "main_utils.h"
#include <NervousSuperMother.h>

#ifndef DEVICE
#define DEVICE
NervousSuperMother *device = NervousSuperMother::getInstance();
#endif

bool previously_inverted = true;

byte inverted_numbers[8][8] = {
    {B11111, B11011, B10011, B11011, B11011, B11011, B11011, B11111}, //  [0] 1 OP invers
    {B11111, B11011, B10101, B11101, B11011, B10111, B10001, B11111}, //  [1] 2 OP invers
    {B11111, B10001, B11101, B11011, B11101, B10101, B11011, B11111}, //  [2] 3 OP invers
    {B11111, B10111, B10111, B10101, B10001, B11101, B11101, B11111}, //  [3] 4 OP invers
    {B11111, B10001, B10111, B10011, B11101, B11101, B10011, B11111}, //  [4] 5 OP invers
    {B11111, B11001, B10111, B10011, B10101, B10101, B11011, B11111}, //  [5] 6 OP invers
    {B11111, B10001, B11101, B11101, B11101, B11101, B11101, B11111}, //  [6] 7 OP invers
    {B11111, B11011, B10101, B10101, B11011, B10101, B10101, B11011}, //  [7] 8 OP invers
};

FLASHMEM void customclear()
{
  // ensures all custom character slots are clear before new custom
  // characters can be defined.
  byte blank[8] =
      {
          B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00000};
  for (int i = 0; i < 8; i++)
  {
    lcd.createChar(i, blank);
  }
}

FLASHMEM void inverted_number_set(long sc_index)
{
  customclear();
  // arrays to form one set of custom characters

  setup_progressbar();
  lcd.createChar(7, inverted_numbers[sc_index]);
}

void write_special_char(long sc_index, bool inverted)
{
  if (inverted)
  {
    // Load custom char before setting cursor !!!
    if (!previously_inverted)
    {
      previously_inverted = true;
      inverted_number_set(sc_index);
    }
    lcd.setCursor(19, 0);
    lcd.write(7);
  }
  else
  {
    lcd.setCursor(19, 0);
    lcd.print(sc_index + 1);
    previously_inverted = false;
  }
}

#endif