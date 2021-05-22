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

const byte analogResolution = 10;

ResponsiveAnalogRead volPot(A1, true);

// ******ANALOG CONSTANT VALUES********
const int ANALOG_CONTROL_PINS = 3;
const int ANALOG_CONTROL[ANALOG_CONTROL_PINS] = {
  SW1,
  SW2,
  SW3
};

const int MUX_CONTROL[MnumControls] = {
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
  POT5
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
const int TRIGGER_PINS = 2;
const int TRIGGERS[TRIGGER_PINS] = {
  TRIG1,
  TRIG2,
};

// ******CVs CONSTANT VALUES********
const int CV_PINS = 4;
const int CVs[CV_PINS] = {
  CV1,
  CV2,
  CV3,
  CV4
};

//******VARIABLES***********

//************INITIALIZE LIBRARY OBJECTS**************

// initialize the ReponsiveAnalogRead objects
ResponsiveAnalogRead analog_controls[]{
  {ANALOG_CONTROL[0],true},
  {ANALOG_CONTROL[1],true},
  {ANALOG_CONTROL[2],true}
};

// initialize the CVs
ResponsiveAnalogRead cv_controls[]{
  {CVs[0],true},
  {CVs[1],true},
  {CVs[2],true},
  {CVs[3],true}
};

// initialize the bounce objects
Bounce digital_button[] =   {
  Bounce(BUTTONS[0],BOUNCE_TIME),
  Bounce(BUTTONS[1],BOUNCE_TIME)
};

Bounce digital_trigger[] =   {
  Bounce(TRIGGERS[0],BOUNCE_TIME),
  Bounce(TRIGGERS[1],BOUNCE_TIME)
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

  // Init muxer pins
  pinMode(MS0, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);

};

#endif
