#ifndef HardwareControls_h
#define HardwareControls_h

#include "Arduino.h"
// include the ResponsiveAnalogRead library for analog smoothing
#include <ResponsiveAnalogRead.h>
// include the Bounce library for 'de-bouncing' switches -- removing electrical chatter as contacts settle
#include <Bounce.h>
// This optional setting causes Encoder to use more optimized code,
// It must be defined before Encoder.h is included.
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include "pins.h"

// ******POTENTIOMETERS CONSTANT VALUES********
const int POTARDS_PINS = 3; // number of Analog potentiometers PINS

const int ANALOG_CONTROL_PINS = POTARDS_PINS;
const int ANALOG_CONTROL[ANALOG_CONTROL_PINS] = {
  POT1,
  POT2,
  POT3
};

// ******BUTTONS CONSTANT VALUES********
const int BUTTON_PINS = 1; // number of Digital trigger PINS
const int BOUNCE_TIME = 5; // 5 ms is usually sufficient
const boolean toggled = true;

// define the pins and notes for digital events
const int BUTTONS[BUTTON_PINS] = {
  ENC1_SW
};

// ******TRIGGERS CONSTANT VALUES********
const int TRIGGER_PINS = 1;
const int TRIGGERS[TRIGGER_PINS] = {
  TRIG1
};

//******VARIABLES***********

//************INITIALIZE LIBRARY OBJECTS**************

// initialize the ReponsiveAnalogRead objects
ResponsiveAnalogRead analog_controls[]{
  {ANALOG_CONTROL[0],true},
  {ANALOG_CONTROL[1],true},
  {ANALOG_CONTROL[2],true}
};

// initialize the bounce objects
Bounce digital_button[] =   {
  Bounce(BUTTONS[0],BOUNCE_TIME)
};

Bounce digital_trigger[] =   {
  Bounce(TRIGGERS[0],BOUNCE_TIME)
};

const int NB_ENCODER = 1;
Encoder encoders_knob[] = {
  Encoder(ENC1_1, ENC1_2)
};

void setup_hardware_controls(){
  // loop to configure input pins and internal pullup resisters for digital section
  for (int i=0;i<BUTTON_PINS;i++){
    pinMode(BUTTONS[i], INPUT_PULLUP);
  }

  for (int i=0;i<TRIGGER_PINS;i++){
    pinMode(TRIGGERS[i], INPUT_PULLUP);
  }
};

#endif
