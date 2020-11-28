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

// ******ANALOG CONSTANT VALUES********
const int ANALOG_CONTROL_PINS = 18;
const int ANALOG_CONTROL[ANALOG_CONTROL_PINS] = {
  SLIDE1,
  SLIDE2,
  SLIDE3,
  SLIDE4,
  SLIDE5,
  SLIDE6,
  SLIDE7,
  SLIDE8,
  SLIDE9,
  SLIDE10,
  POT1,
  POT2,
  POT3,
  POT4,
  POT5,
  SW1,
  SW2,
  SW3
};

// ******BUTTONS CONSTANT VALUES********
const int BUTTON_PINS = 2; // number of Digital trigger PINS
const int BOUNCE_TIME = 5; // 5 ms is usually sufficient
const boolean toggled = true;

// define the pins and notes for digital events
const int BUTTONS[BUTTON_PINS] = {
  ENC1_SW,
  ENC2_SW
};

// ******TRIGGERS CONSTANT VALUES********
const int TRIGGER_PINS = 6;
const int TRIGGERS[TRIGGER_PINS] = {
  TRIG1,
  TRIG2,
  TRIG3,
  TRIG4,
  TRIG5,
  TRIG6
};

//******VARIABLES***********

//************INITIALIZE LIBRARY OBJECTS**************

// initialize the ReponsiveAnalogRead objects
ResponsiveAnalogRead analog_controls[]{
  {ANALOG_CONTROL[0],true},
  {ANALOG_CONTROL[1],true},
  {ANALOG_CONTROL[2],true},
  {ANALOG_CONTROL[3],true},
  {ANALOG_CONTROL[4],true},
  {ANALOG_CONTROL[5],true},
  {ANALOG_CONTROL[6],true},
  {ANALOG_CONTROL[7],true},
  {ANALOG_CONTROL[8],true},
  {ANALOG_CONTROL[9],true},
  {ANALOG_CONTROL[10],true},
  {ANALOG_CONTROL[11],true},
  {ANALOG_CONTROL[12],true},
  {ANALOG_CONTROL[13],true},
  {ANALOG_CONTROL[14],true},
  {ANALOG_CONTROL[15],true},
  {ANALOG_CONTROL[16],true},
  {ANALOG_CONTROL[17],true}
};

// initialize the bounce objects
Bounce digital_button[] =   {
  Bounce(BUTTONS[0],BOUNCE_TIME),
  Bounce(BUTTONS[1],BOUNCE_TIME)
};

Bounce digital_trigger[] =   {
  Bounce(TRIGGERS[0],BOUNCE_TIME),
  Bounce(TRIGGERS[1],BOUNCE_TIME),
  Bounce(TRIGGERS[2],BOUNCE_TIME),
  Bounce(TRIGGERS[3],BOUNCE_TIME),
  Bounce(TRIGGERS[4],BOUNCE_TIME),
  Bounce(TRIGGERS[5],BOUNCE_TIME)
};

const int NB_ENCODER = 2;
Encoder encoders_knob[] = {
  Encoder(ENC1_1, ENC1_2),
  Encoder(ENC2_1, ENC2_2)
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
