#ifndef Display_h
#define Display_h

#include "Arduino.h"
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
// LiquidCrystal lcd(RS, Enable, D4, D5, D6, D7)
LiquidCrystal lcd(DISPLAY_RS, DISPLAY_E, DISPLAY_DB4, DISPLAY_DB5, DISPLAY_DB6, DISPLAY_DB7);

int LCD_NB_COLUMNS = 20;

// /* Caractères personnalisés */
// byte START_DIV_0_OF_1[8] = {
//   B01111,
//   B11000,
//   B10000,
//   B10000,
//   B10000,
//   B10000,
//   B11000,
//   B01111
// }; // Char début 0 / 1
//
// byte START_DIV_1_OF_1[8] = {
//   B01111,
//   B11000,
//   B10011,
//   B10111,
//   B10111,
//   B10011,
//   B11000,
//   B01111
// }; // Char début 1 / 1
//
// byte DIV_0_OF_2[8] = {
//   B11111,
//   B00000,
//   B00000,
//   B00000,
//   B00000,
//   B00000,
//   B00000,
//   B11111
// }; // Char milieu 0 / 2
//
// byte DIV_1_OF_2[8] = {
//   B11111,
//   B00000,
//   B11000,
//   B11000,
//   B11000,
//   B11000,
//   B00000,
//   B11111
// }; // Char milieu 1 / 2
//
// byte DIV_2_OF_2[8] = {
//   B11111,
//   B00000,
//   B11011,
//   B11011,
//   B11011,
//   B11011,
//   B00000,
//   B11111
// }; // Char milieu 2 / 2
//
// byte END_DIV_0_OF_1[8] = {
//   B11110,
//   B00011,
//   B00001,
//   B00001,
//   B00001,
//   B00001,
//   B00011,
//   B11110
// }; // Char fin 0 / 1
//
// byte END_DIV_1_OF_1[8] = {
//   B11110,
//   B00011,
//   B11001,
//   B11101,
//   B11101,
//   B11001,
//   B00011,
//   B11110
// }; // Char fin 1 / 1

byte SCROLLBAR_TOP[8] = {
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001
}; // scrollbar top

byte SCROLLBAR_STATE1[8] =  {
  B11111,
  B11111,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001
}; // scroll state 1

byte SCROLLBAR_STATE2[8] = {
  B10001,
  B10001,
  B11111,
  B11111,
  B10001,
  B10001,
  B10001,
  B10001
}; // scroll state 2

byte SCROLLBAR_STATE3[8] = {
  B10001,
  B10001,
  B10001,
  B10001,
  B11111,
  B11111,
  B10001,
  B10001
}; // scroll state 3

byte SCROLLBAR_BOTTOM[8] = {
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111,
  B11111  // scrollbar bottom
};

/**
 * Fonction de configuration de l'écran LCD pour la barre de progression.
 * Utilise les caractères personnalisés de 0 à 6 (7 reste disponible).
 */
FLASHMEM void setup_progressbar() {

  /* Enregistre les caractères personnalisés dans la mémoire de l'écran LCD */
  // set special chars for scrollbar
  lcd.createChar(0, SCROLLBAR_TOP);
  lcd.createChar(1, SCROLLBAR_STATE1);
  lcd.createChar(2, SCROLLBAR_STATE2);
  lcd.createChar(3, SCROLLBAR_STATE3);
  lcd.createChar(4, SCROLLBAR_BOTTOM);
  // lcd.createChar(5, START_DIV_0_OF_1);
  // lcd.createChar(6, START_DIV_1_OF_1);
  // lcd.createChar(7, DIV_0_OF_2);
  // lcd.createChar(8, DIV_1_OF_2);
  // lcd.createChar(9, DIV_2_OF_2);
  // lcd.createChar(10, END_DIV_0_OF_1);
  // lcd.createChar(11, END_DIV_1_OF_1);
}

FLASHMEM void setup_lcd(){
  // set up the LCD's number of rows and columns:
  lcd.begin(20, 2);
  setup_progressbar();
}

/**
 * Fonction dessinant la barre de progression.
 *
 * @param percent Le pourcentage à afficher.
 */
void draw_progressbar(byte percent) {
 /* Déplace le curseur sur la seconde ligne */
  lcd.setCursor(0, 1);

  /* Map la plage (0 ~ 100) vers la plage (0 ~ LCD_NB_COLUMNS * 2 - 2) */
  byte nb_columns = map(percent, 0, 100, 0, LCD_NB_COLUMNS * 2 - 2);
  // Chaque caractère affiche 2 barres verticales, mais le premier et dernier caractère n'en affiche qu'une.

  /* Dessine chaque caractère de la ligne */
  for (byte i = 0; i < LCD_NB_COLUMNS; ++i) {
    lcd.setCursor(0, 1);

    if (i == 0) { // Premiére case

      /* Affiche le char de début en fonction du nombre de colonnes */
      if (nb_columns > 0) {
        lcd.write(1); // Char début 1 / 1
        nb_columns -= 1;

      } else {
        lcd.write((byte) 0); // Char début 0 / 1
      }

    } else if (i == LCD_NB_COLUMNS -1) { // Derniére case

      /* Affiche le char de fin en fonction du nombre de colonnes */
      if (nb_columns > 0) {
        lcd.write(6); // Char fin 1 / 1

      } else {
        lcd.write(5); // Char fin 0 / 1
      }

    } else { // Autres cases

      /* Affiche le char adéquat en fonction du nombre de colonnes */
      if (nb_columns >= 2) {
        lcd.write(4); // Char div 2 / 2
        nb_columns -= 2;

      } else if (nb_columns == 1) {
        lcd.write(3); // Char div 1 / 2
        nb_columns -= 1;

      } else {
        lcd.write(2); // Char div 0 / 2
      }
    }
  }
}

#endif
