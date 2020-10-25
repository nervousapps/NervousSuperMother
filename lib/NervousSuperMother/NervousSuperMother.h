#ifndef NervousSuperMother_h
#define NervousSuperMother_h

#include "hardware/HardwareControls.h"
#include "display/Display.h"

/*
* NervousSuperMother
* v0.0.1 beta
*/
class NervousSuperMother{

private:
  static NervousSuperMother *instance;
  NervousSuperMother();

  byte *inputs;

  byte ioNumber = 6;
  byte currentInput = 0;

  byte analogResolution = 10;

  // Potentiometers
  unsigned int *potentiometers;
  unsigned int *potentiometersPrevious;
  byte potardIndex;
  // For smoothing purposes
  unsigned int *potentiometersTemp;
  byte *potentiometersReadings;
  // Buttons
  bool *buttons;
  byte buttonIndex;
  bool *pushed;
  // Encoders
  long *encoders;
  long *encodersPrevious;
  byte encoderIndex;
  long *encodersMaxValue;
  // Triggers
  byte triggerIndex;

  // Display
  String display_line_1;
  String display_line_2;
  String previous_display_line_1;
  String previous_display_line_2;

  // Callbacks
  using PressCallback = void (*)(byte);
  PressCallback *inputsPressCallback;
  using LongPressCallback = void (*)(byte);
  LongPressCallback *inputsLongPressCallback;
  using DoublePressCallback = void (*)(byte);
  DoublePressCallback *inputsDoublePressCallback;
  elapsedMillis *inputsPressTime;
  using PotentiometerChangeCallback = void (*)(byte, unsigned int, int);
  PotentiometerChangeCallback *inputsPotentiometerChangeCallback;
  using EncoderChangeCallback = void (*)(byte, long);
  EncoderChangeCallback *inputsEncoderChangeCallback;
  using TriggerCallback = void (*)(byte);
  TriggerCallback *inputsTriggerCallback;


  void readPotentiometer(byte inputIndex);
  void readButton(byte inputIndex);
  void readEncoder(byte inputIndex);
  void readTrigger(byte inputIndex);
  void refreshDisplay();

  // Main clock
  elapsedMicros clockMain;
  const unsigned int intervalClockMain = 5000;

  // Inputs clock
  const unsigned int intervalInputs = 100;
  elapsedMicros clockInputs;

public:
  static NervousSuperMother *getInstance();
  void init(byte *inputs);
  void update();
  int getInput(byte index);
  int getAnalogMaxValue();
  int getAnalogMinValue();
  void iterateInputs();
  void readCurrentInput();
  void updateEncodeursMaxValue(byte index, long encoderMax);
  void updateEncodeursValue(byte inputIndex, long encoderValue);
  void updateLine(byte line_nb, String line);

  // Callbacks
  void setHandlePress(byte inputIndex, PressCallback fptr);
  void setHandleDoublePress(byte inputIndex, LongPressCallback fptr);
  void setHandleLongPress(byte inputIndex, DoublePressCallback fptr);
  void setHandlePotentiometerChange(byte inputIndex, PotentiometerChangeCallback fptr);
  void setHandleEncoderChange(byte inputIndex, EncoderChangeCallback fptr);
  void setHandleTrigger(byte inputIndex, TriggerCallback fptr);
};

// Instance pre init
NervousSuperMother * NervousSuperMother::instance = nullptr;

/**
* Constructor
*/
inline NervousSuperMother::NervousSuperMother(){
  this->inputs = new byte[this->ioNumber];
  for(byte i = 0; i < this->ioNumber; i++){
    this->inputs[i] = 0;
  }

  // Potentiometers
  this->potardIndex = 0;
  this->potentiometers = new unsigned int[POTARDS_PINS];
  this->potentiometersPrevious = new unsigned int[POTARDS_PINS];
  this->potentiometersTemp = new unsigned int[POTARDS_PINS];
  this->potentiometersReadings = new byte[POTARDS_PINS];
  this->inputsPotentiometerChangeCallback = new PotentiometerChangeCallback[POTARDS_PINS];

  for(byte i = 0; i < POTARDS_PINS; i++){
    this->potentiometers[i] = 0;
    this->potentiometersPrevious[i] = 0;
    this->potentiometersTemp[i] = 0;
    this->potentiometersReadings[i] = 0;
    this->inputsPotentiometerChangeCallback[i] = nullptr;
  }

  // Buttons
  this->buttonIndex = 0;
  this->buttons = new bool[BUTTON_PINS];
  this->pushed = new bool[BUTTON_PINS];
  this->inputsPressCallback = new PressCallback[BUTTON_PINS];
  this->inputsLongPressCallback = new DoublePressCallback[BUTTON_PINS];
  this->inputsDoublePressCallback = new LongPressCallback[BUTTON_PINS];
  this->inputsPressTime = new elapsedMillis[BUTTON_PINS];

  for(byte i = 0; i < BUTTON_PINS; i++){
    this->buttons[i] = true;
    this->pushed[i] = false;
    this->inputsPressCallback[i] = nullptr;
    this->inputsLongPressCallback[i] = nullptr;
    this->inputsDoublePressCallback[i] = nullptr;
    this->inputsPressTime[i] = 0;
  }

  // Encoders
  this->encoderIndex = 0;
  this->encoders = new long[NB_ENCODER];
  this->encodersPrevious = new long[NB_ENCODER];
  this->inputsEncoderChangeCallback = new EncoderChangeCallback[NB_ENCODER];
  this->encodersMaxValue = new long[NB_ENCODER];

  for(byte i = 0; i < NB_ENCODER; i++){
    this->encoders[i] = 0;
    this->encodersPrevious[i] = -999;
    this->inputsEncoderChangeCallback[i] = nullptr;
    this->encodersMaxValue[i] = 0;
  }

  // Triggers
  this->triggerIndex = 0;
  this->inputsTriggerCallback = new TriggerCallback[TRIGGER_PINS];
  for(byte i = 0; i < TRIGGER_PINS; i++){
    this->inputsTriggerCallback[i] = nullptr;
  }

  // Display
  this->display_line_1 = "";
  this->display_line_2 = "";
  this->previous_display_line_1 = "";
  this->previous_display_line_2 = "";
}

/**
* Singleton instance
*/
inline NervousSuperMother *NervousSuperMother::getInstance()    {
  if (!instance)
  instance = new NervousSuperMother;
  return instance;
}

/**
* Init
*/
inline void NervousSuperMother::init(byte *inputs){
  for(byte i = 0; i < this->ioNumber; i++){
    this->inputs[i] = inputs[i];
  }
  setup_hardware_controls();
  setup_lcd();
}

/**
* Update
*/
inline void NervousSuperMother::update(){
  // Main clock
  if (this->clockMain >= this->intervalClockMain) {
    this->clockMain = 0;
  }
  if (this->clockMain > this->intervalClockMain / 2) {
    return;
  }else{
    // Inputs
    // At the end of the clock we iterate to next input
    if (this->clockInputs >= this->intervalInputs) {
      this->iterateInputs();
      this->clockInputs = 0;
    }else{
      // Reading the current input
      this->readCurrentInput();
    }
  }
}

/**
* Iterate over the inputs
*/
inline void NervousSuperMother::iterateInputs(){
  this->currentInput++;
  this->currentInput = this->currentInput % this->ioNumber;
}

/**
* Read value of current inout
*/
inline void NervousSuperMother::readCurrentInput(){
  switch(this->inputs[this->currentInput]){
    default:
    case 0:
    // Silence is golden
    break;

    case 1:
    if(this->potardIndex < POTARDS_PINS) {
      this->readPotentiometer(this->potardIndex);
      this->potardIndex ++;
    } else {
      this->potardIndex = 0;
    }
    break;
    case 2:
    if(this->buttonIndex < BUTTON_PINS) {
      this->readButton(this->buttonIndex);
      this->buttonIndex ++;
    }else {
      this->buttonIndex = 0;
    }
    break;
    case 3:
    if(this->encoderIndex < NB_ENCODER) {
      this->readEncoder(this->encoderIndex);
      this->encoderIndex ++;
    }else {
      this->encoderIndex = 0;
    }
    break;
    case 4:
    if(this->triggerIndex < TRIGGER_PINS) {
      this->readTrigger(this->triggerIndex);
      this->triggerIndex ++;
    }else {
      this->triggerIndex = 0;
    }
    break;
    case 5:
    this->refreshDisplay();
    break;
  }
}

/**
* Get potentiometer value
* @param byte inputeIndex The index of the input
*/
inline void NervousSuperMother::readPotentiometer(byte inputIndex){
  analog_controls[inputIndex].update();
  if(analog_controls[inputIndex].hasChanged()){
    this->potentiometers[inputIndex] = analog_controls[inputIndex].getValue();

    if(this->potentiometers[inputIndex] != this->potentiometersPrevious[inputIndex]){
      // Calling the potentiometer callback if there is one
      if(this->inputsPotentiometerChangeCallback[inputIndex] != nullptr){
        this->inputsPotentiometerChangeCallback[inputIndex](inputIndex, this->potentiometers[inputIndex], this->potentiometers[inputIndex] - this->potentiometersPrevious[inputIndex] );
      }
    }
    this->potentiometersPrevious[inputIndex] = this->potentiometers[inputIndex];
  }

}

/**
* Get input value
* @param byte index The index of the input
*/
inline int NervousSuperMother::getInput(byte index){
  switch(this->inputs[index]){
    default:
    case 0:
    // Empty
    return 0;
    break;
    case 1:
    // Buttons
    return !this->buttons[index];
    break;
    case 2:
    // Potentiometer
    return this->potentiometers[index];
    break;
    case 3:
    // Encoder
    // Device is not saving the encoders values, only the latest change
    // int value = this->encoders[index];
    // this->encoders[index] = 0;
    // return value;
    break;
  }
}

/**
* Get max analog value according to resolution
*/
inline int NervousSuperMother::getAnalogMinValue(){
  return 0;
}

/**
* Get max analog value according to resolution
*/
inline int NervousSuperMother::getAnalogMaxValue(){
  return (1 << this->analogResolution) - 1;
}

/**
* Handle potentiometer
*/
inline void NervousSuperMother::setHandlePotentiometerChange(byte inputIndex, PotentiometerChangeCallback fptr){
  this->inputsPotentiometerChangeCallback[inputIndex] = fptr;
}

/**
* Get button value
* @param byte inputeIndex The index of the input
*/
inline void NervousSuperMother::readButton(byte buttonIndex) {
  // If there is a short or a long press callback on that input
  if(this->inputsPressCallback[buttonIndex] != nullptr ||
    this->inputsDoublePressCallback[buttonIndex] != nullptr ||
    this->inputsLongPressCallback[buttonIndex] != nullptr) {
      if(digital_button[buttonIndex].update()){
        if(digital_button[buttonIndex].fallingEdge()){
          this->pushed[buttonIndex] = true;
          if(this->inputsPressTime[buttonIndex] <= 400 && this->pushed[buttonIndex]){
            this->inputsDoublePressCallback[buttonIndex](buttonIndex);
          }
          this->inputsPressTime[buttonIndex] = 0;
        }

        if(digital_button[buttonIndex].risingEdge()){
          if(this->inputsPressTime[buttonIndex] > 150 && this->inputsPressTime[buttonIndex] < 500){
            this->inputsPressCallback[buttonIndex](buttonIndex);
          }
          this->pushed[buttonIndex] = false;
        }
      }else if(this->inputsPressTime[buttonIndex] >= 800 && this->pushed[buttonIndex]){
        this->inputsLongPressCallback[buttonIndex](buttonIndex);
        this->pushed[buttonIndex] = false;
      }
    }
  }

  /**
  * Handle press down on a button
  */
  inline void NervousSuperMother::setHandlePress(byte inputIndex, PressCallback fptr){
    this->inputsPressCallback[inputIndex] = fptr;
  }

  /**
  * Handle press up on a button
  */
  inline void NervousSuperMother::setHandleDoublePress(byte inputIndex, DoublePressCallback fptr){
    this->inputsDoublePressCallback[inputIndex] = fptr;
  }

  /**
  * Handle long press down on a button
  */
  inline void NervousSuperMother::setHandleLongPress(byte inputIndex, LongPressCallback fptr){
    this->inputsLongPressCallback[inputIndex] = fptr;
  }

  /**
  * Get encoder value
  * @param byte inputeIndex The index of the input
  */
  inline void NervousSuperMother::readEncoder(byte inputIndex) {
    // Get rotary encoder value
    // Reverse
    if(this->encodersMaxValue[inputIndex] < 0){
      this->encoders[inputIndex] = encoders_knob[inputIndex].read();
      if(this->encoders[inputIndex] < this->encodersMaxValue[inputIndex]){
        this->encoders[inputIndex] = 0;
        encoders_knob[inputIndex].write(0);
      }
      if(this->encoders[inputIndex] > 0){
        this->encoders[inputIndex] = this->encodersMaxValue[inputIndex];
        encoders_knob[inputIndex].write(this->encodersMaxValue[inputIndex]);
      }
      if (this->encoders[inputIndex]%4 == 0 && this->encoders[inputIndex] != this->encodersPrevious[inputIndex]) {
        this->encodersPrevious[inputIndex] = this->encoders[inputIndex];
        // Calling the Encoder callback if there is one
        if(this->inputsEncoderChangeCallback[inputIndex] != nullptr){
          this->inputsEncoderChangeCallback[inputIndex](inputIndex, abs(this->encoders[inputIndex]/4));
        }
      }
    // Normal
    }else {
      this->encoders[inputIndex] = encoders_knob[inputIndex].read();
      if(this->encoders[inputIndex] > this->encodersMaxValue[inputIndex]){
        this->encoders[inputIndex] = 0;
        encoders_knob[inputIndex].write(0);
      }
      if(this->encoders[inputIndex] < 0){
        this->encoders[inputIndex] = this->encodersMaxValue[inputIndex];
        encoders_knob[inputIndex].write(this->encodersMaxValue[inputIndex]);
      }
      if (this->encoders[inputIndex]%4 == 0 && this->encoders[inputIndex] != this->encodersPrevious[inputIndex]) {
        this->encodersPrevious[inputIndex] = this->encoders[inputIndex];
        // Calling the Encoder callback if there is one
        if(this->inputsEncoderChangeCallback[inputIndex] != nullptr){
          this->inputsEncoderChangeCallback[inputIndex](inputIndex, this->encoders[inputIndex]/4);
        }
      }
    }
  }

  /**
  * Handle encoder change
  */
  inline void NervousSuperMother::setHandleEncoderChange(byte inputIndex, EncoderChangeCallback fptr) {
    this->inputsEncoderChangeCallback[inputIndex] = fptr;
  }

  /**
  * Update encoders max value
  */
  inline void NervousSuperMother::updateEncodeursMaxValue(byte inputIndex, long encoderMax) {
    this->encodersMaxValue[inputIndex] = encoderMax*4;
  }

  /**
  * Update encoders value
  */
  inline void NervousSuperMother::updateEncodeursValue(byte inputIndex, long encoderValue) {
    encoders_knob[inputIndex].write(encoderValue*4);
  }

  /**
  * Get trigger
  * @param byte inputeIndex The index of the input
  */
  inline void NervousSuperMother::readTrigger(byte inputIndex){
    digital_trigger[inputIndex].update();
    if (digital_trigger[inputIndex].fallingEdge()) {
      if(this->inputsTriggerCallback[inputIndex] != nullptr){
        this->inputsTriggerCallback[inputIndex](inputIndex);
      }
    }
  }

  /**
  * Handle trigger
  */
  inline void NervousSuperMother::setHandleTrigger(byte inputIndex, TriggerCallback fptr){
    this->inputsTriggerCallback[inputIndex] = fptr;
  }

  /**
  * Update line to display
  */
  inline void NervousSuperMother::updateLine(byte line_nb, String line) {
    if(line_nb == 1){
      this->display_line_1 = line;
    }else if(line_nb == 2){
      this->display_line_2 = line;
    }
  }

  /**
  * Refresh the display
  */
  inline void NervousSuperMother::refreshDisplay() {
    if(this->display_line_1 != this->previous_display_line_1 || this->display_line_2 != this->previous_display_line_2){
      byte j = 0;
      for(byte i = 0; i < this->display_line_1.length(); i++){
        if(this->previous_display_line_1[i] != this->display_line_1[i]){
          lcd.setCursor(i, 0);
          lcd.print(this->display_line_1[i]);
        }
        j = i+1;
      }
      if(j < 20){
        for(byte i = j; i < 20; i++){
          lcd.setCursor(i, 0);
          lcd.print(" ");
        }
      }
      j = 0;
      for(byte i = 0; i < this->display_line_2.length(); i++){
        if(this->previous_display_line_2[i] != this->display_line_2[i]){
          lcd.setCursor(i, 1);
          lcd.print(this->display_line_2[i]);
        }
        j = i+1;
      }
      if(j < 20){
        for(byte i = j; i < 20; i++){
          lcd.setCursor(i, 1);
          lcd.print(" ");
        }
      }
      this->previous_display_line_1 = this->display_line_1;
      this->previous_display_line_2 = this->display_line_2;
    }
  }

  #endif
