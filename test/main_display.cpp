#include "Arduino.h"
#include <LiquidCrystal.h>

// Display
#define DISPLAY_RS	  31
#define DISPLAY_RW	  32
#define DISPLAY_E	    33
#define DISPLAY_DB4	  34
#define DISPLAY_DB5	  35
#define DISPLAY_DB6	  36
#define DISPLAY_DB7	  37


// initialize the library with the numbers of the interface pins
// LiquidCrystal lcd(RS, RW, Enable, D4, D5, D6, D7)
LiquidCrystal lcd(DISPLAY_RS, DISPLAY_RW, DISPLAY_E, DISPLAY_DB4, DISPLAY_DB5, DISPLAY_DB6, DISPLAY_DB7);

int LCD_NB_COLUMNS = 20;

int counter = 0;

void setup() {
  // set up the LCD's number of rows and columns:
  lcd.begin(20, 2);
  // Starting animation
  lcd.setCursor(0,0);
  lcd.print("!    SuperSynth    !");

  delay(2000);
}

void loop() {
  lcd.setCursor(0,0);
  lcd.print(String(counter));
  counter ++;
}
