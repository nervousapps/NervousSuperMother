#ifndef NervousSuperMother_h
#define NervousSuperMother_h

#include "hardware/HardwareControls.h"
#include "display/Display.h"

/*
 * NervousSuperMother(withDaddy)
 * v0.4.0 beta
 */
class NervousSuperMother
{

private:
  static NervousSuperMother *instance;
  NervousSuperMother();

  byte *inputs;

  byte ioNumber = 7;
  byte currentInput = 0;
  byte analogResolution = ANALOGRESOLUTION;

  // Switches
  bool *switches;
  bool *switchesPrevious;
  byte switchIndex;
  // Mux Controls
  unsigned int *muxcontrols;
  unsigned int *muxcontrolsPrevious;
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
  // CVs
  unsigned int *cvs;
  unsigned int *cvsPrevious;
  byte cvIndex;

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
  using SwitchChangeCallback = void (*)(byte, bool);
  SwitchChangeCallback *inputsSwitchesChangeCallback;
  using MuxControlChangeCallback = void (*)(byte, unsigned int, int);
  MuxControlChangeCallback *inputsMuxControlsChangeCallback;
  using EncoderChangeCallback = void (*)(byte, long);
  EncoderChangeCallback *inputsEncoderChangeCallback;
  using FallingTriggerCallback = void (*)(byte);
  FallingTriggerCallback *inputsFallingTriggerCallback;
  using RisingTriggerCallback = void (*)(byte);
  RisingTriggerCallback *inputsRisingTriggerCallback;
  using CVChangeCallback = void (*)(byte, unsigned int, int);
  CVChangeCallback *inputsCVChangeCallback;
  using volChangeCallback = void (*)(float);
  volChangeCallback inputsvolChangeCallback;

  using CustomDisplayRefresh = void (*)();
  CustomDisplayRefresh customDisplayRefresh;

  void readSwitch(byte inputIndex);
  void readMux();
  void readButton(byte inputIndex);
  void readEncoder(byte inputIndex);
  void readTrigger(byte inputIndex);
  void readCV(byte inputIndex);

  // Main clock
  elapsedMicros clockMain;
  const unsigned int intervalClockMain = 5000;
  // Display clock
  const unsigned int intervalDisplay = 1000;
  elapsedMicros clockDisplay;
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
  void resetHandlers();
  void updateOneChar(byte line_nb, char one_char, int index);
  void refreshDisplay(bool force = false);

  // Callbacks
  void setHandlePress(byte inputIndex, PressCallback fptr);
  void setHandleLongPress(byte inputIndex, LongPressCallback fptr);
  void setHandleDoublePress(byte inputIndex, DoublePressCallback fptr);
  void setHandleSwitchChange(byte inputIndex, SwitchChangeCallback fptr);
  void setHandleMuxControlChange(byte inputIndex, MuxControlChangeCallback fptr);
  void setHandleEncoderChange(byte inputIndex, EncoderChangeCallback fptr);
  void setHandleFallingTrigger(byte inputIndex, FallingTriggerCallback fptr);
  void setHandleRisingTrigger(byte inputIndex, RisingTriggerCallback fptr);
  void setHandleCVChange(byte inputIndex, CVChangeCallback fptr);
  void setHandleVolChange(volChangeCallback fptr);
  void setCustomDisplayRefresh(CustomDisplayRefresh fptr);
};

// Instance pre init
NervousSuperMother *NervousSuperMother::instance = nullptr;

/**
 * Constructor
 */
FLASHMEM NervousSuperMother::NervousSuperMother()
{
  this->inputs = new byte[this->ioNumber];
  for (byte i = 0; i < this->ioNumber; i++)
  {
    this->inputs[i] = 0;
  }

  // Switches
  this->switchIndex = 0;
  this->switches = new bool[ANALOG_CONTROL_PINS];
  this->switchesPrevious = new bool[ANALOG_CONTROL_PINS];
  this->inputsSwitchesChangeCallback = new SwitchChangeCallback[ANALOG_CONTROL_PINS];

  for (byte i = 0; i < ANALOG_CONTROL_PINS; i++)
  {
    this->switches[i] = 0;
    this->switchesPrevious[i] = 0;
    this->inputsSwitchesChangeCallback[i] = nullptr;
  }

  // Mux controls
  this->muxcontrols = new unsigned int[MnumControls];
  this->muxcontrolsPrevious = new unsigned int[MnumControls];
  this->inputsMuxControlsChangeCallback = new MuxControlChangeCallback[MnumControls];

  for (byte i = 0; i < MnumControls; i++)
  {
    this->muxcontrols[i] = 0;
    this->muxcontrolsPrevious[i] = 0;
    this->inputsMuxControlsChangeCallback[i] = nullptr;
  }

  // Buttons
  this->buttonIndex = 0;
  this->buttons = new bool[BUTTON_PINS];
  this->pushed = new bool[BUTTON_PINS];
  this->inputsPressCallback = new PressCallback[BUTTON_PINS];
  this->inputsLongPressCallback = new DoublePressCallback[BUTTON_PINS];
  this->inputsDoublePressCallback = new LongPressCallback[BUTTON_PINS];
  this->inputsPressTime = new elapsedMillis[BUTTON_PINS];

  for (byte i = 0; i < BUTTON_PINS; i++)
  {
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

  for (byte i = 0; i < NB_ENCODER; i++)
  {
    this->encoders[i] = 0;
    this->encodersPrevious[i] = -999;
    this->inputsEncoderChangeCallback[i] = nullptr;
    this->encodersMaxValue[i] = 0;
  }

  // Triggers
  this->triggerIndex = 0;
  this->inputsFallingTriggerCallback = new FallingTriggerCallback[TRIGGER_PINS];
  for (byte i = 0; i < TRIGGER_PINS; i++)
  {
    this->inputsFallingTriggerCallback[i] = nullptr;
  }
  this->inputsRisingTriggerCallback = new RisingTriggerCallback[TRIGGER_PINS];
  for (byte i = 0; i < TRIGGER_PINS; i++)
  {
    this->inputsRisingTriggerCallback[i] = nullptr;
  }

  // CVs
  this->cvIndex = 0;
  this->cvs = new unsigned int[CV_PINS];
  this->cvsPrevious = new unsigned int[CV_PINS];
  this->inputsCVChangeCallback = new CVChangeCallback[CV_PINS];

  for (byte i = 0; i < CV_PINS; i++)
  {
    this->cvs[i] = 0;
    this->cvsPrevious[i] = 0;
    this->inputsCVChangeCallback[i] = nullptr;
  }

  // Display
  this->display_line_1 = "";
  this->display_line_2 = "";
  this->previous_display_line_1 = "";
  this->previous_display_line_2 = "";
  this->customDisplayRefresh = nullptr;
}

/**
 * Singleton instance
 */
FLASHMEM inline NervousSuperMother *NervousSuperMother::getInstance()
{
  if (!instance)
    instance = new NervousSuperMother;
  return instance;
}

/**
 * Init
 */
FLASHMEM inline void NervousSuperMother::init(byte *inputs)
{
  for (byte i = 0; i < this->ioNumber; i++)
  {
    this->inputs[i] = inputs[i];
  }
  setup_hardware_controls();
  setup_lcd();
}

/**
 * Reset
 */
inline void NervousSuperMother::resetHandlers()
{
  // Reset switch handlers
  // for(byte i = 0; i < ANALOG_CONTROL_PINS; i++){
  //   this->switches[i] = 0;
  //   this->switchesPrevious[i] = 0;
  //   this->inputsSwitchesChangeCallback[i] = nullptr;
  // }

  // Reset muxers handlers
  for (byte i = 0; i < MnumControls; i++)
  {
    this->muxcontrols[i] = 0;
    this->muxcontrolsPrevious[i] = 0;
    this->inputsMuxControlsChangeCallback[i] = nullptr;
  }

  // Reset buttons handlers
  for (byte i = 0; i < BUTTON_PINS; i++)
  {
    this->buttons[i] = true;
    this->pushed[i] = false;
    this->inputsPressCallback[i] = nullptr;
    this->inputsLongPressCallback[i] = nullptr;
    this->inputsDoublePressCallback[i] = nullptr;
    this->inputsPressTime[i] = 0;
  }

  // Reset encoders handlers
  for (byte i = 0; i < NB_ENCODER; i++)
  {
    this->encoders[i] = 0;
    this->encodersPrevious[i] = -999;
    this->inputsEncoderChangeCallback[i] = nullptr;
    this->encodersMaxValue[i] = 0;
  }
}

/**
 * Update
 */
inline void NervousSuperMother::update()
{
  this->readMux();
  // Main clock
  if (this->clockMain >= this->intervalClockMain)
  {
    this->clockMain = 0;
  }
  if (this->clockMain > this->intervalClockMain / 2)
  {
    if (this->clockDisplay >= this->intervalDisplay)
    {
      this->refreshDisplay();
      this->clockDisplay = 0;
    }
  }
  else
  {
    // AudioNoInterrupts();
    // Inputs
    // At the end of the clock we iterate to next input
    if (this->clockInputs >= this->intervalInputs)
    {
      this->iterateInputs();
      this->clockInputs = 0;
    }
    else
    {
      // Reading the current input
      this->readCurrentInput();
    }
    // AudioInterrupts();
  }
}

/**
 * Iterate over the inputs
 */
inline void NervousSuperMother::iterateInputs()
{
  this->currentInput++;
  this->currentInput = this->currentInput % this->ioNumber;
}

/**
 * Read value of current inout
 */
inline void NervousSuperMother::readCurrentInput()
{
  switch (this->inputs[this->currentInput])
  {
  default:
  case 0:
    // Silence is golden
    break;

  case 1:
    if (this->switchIndex < ANALOG_CONTROL_PINS)
    {
      this->readSwitch(this->switchIndex);
      this->switchIndex++;
    }
    else
    {
      this->switchIndex = 0;
    }
    break;

  case 2:
    if (this->buttonIndex < BUTTON_PINS)
    {
      this->readButton(this->buttonIndex);
      this->buttonIndex++;
    }
    else
    {
      this->buttonIndex = 0;
    }
    break;

  case 3:
    if (this->encoderIndex < NB_ENCODER)
    {
      this->readEncoder(this->encoderIndex);
      this->encoderIndex++;
    }
    else
    {
      this->encoderIndex = 0;
    }
    break;

  case 4:
    if (this->triggerIndex < TRIGGER_PINS)
    {
      this->readTrigger(this->triggerIndex);
      this->triggerIndex++;
    }
    else
    {
      this->triggerIndex = 0;
    }
    break;

  case 5:
    if (this->cvIndex < CV_PINS)
    {
      this->readCV(this->cvIndex);
      this->cvIndex++;
    }
    else
    {
      this->cvIndex = 0;
    }
    break;

  case 6:
    volPot.update();
    if (volPot.hasChanged())
    {
      this->inputsvolChangeCallback(volPot.getValue());
    }
    break;
  }
}

/**
 * Get switch value
 * @param byte inputeIndex The index of the input
 */
inline void NervousSuperMother::readSwitch(byte inputIndex)
{
  analog_controls[inputIndex].update();
  if (analog_controls[inputIndex].hasChanged())
  {
    int val = analog_controls[inputIndex].getValue();

    if (val > 100)
    {
      this->switches[inputIndex] = true;
    }
    else
    {
      this->switches[inputIndex] = false;
    }

    if (this->switches[inputIndex] != this->switchesPrevious[inputIndex])
    {
      // Calling the switch callback if there is one
      if (this->inputsSwitchesChangeCallback[inputIndex] != nullptr)
      {
        this->inputsSwitchesChangeCallback[inputIndex](inputIndex, this->switches[inputIndex]);
      }
    }
    this->switchesPrevious[inputIndex] = this->switches[inputIndex];
  }
}

/**
 * Get muxer controls value
 */
inline void NervousSuperMother::readMux()
{
  static byte muxInput = 0;
  static int muxValues[MnumControls] = {};
  // Mux clock
  static unsigned long clockMux = 0;
  unsigned long intervalMux = micros();

  if (intervalMux - clockMux >= 1000)
  {
    clockMux = intervalMux;
    mux_read.update();
    if (mux_read.hasChanged())
    {
      int muxRead = mux_read.getValue();
      if (muxRead > (muxValues[muxInput] + 7) || muxRead < (muxValues[muxInput] - 7))
      {
        muxValues[muxInput] = muxRead;
        muxRead = (muxRead >> 3); // Change range to 0-127
        this->muxcontrols[muxInput] = muxRead;
        if (this->muxcontrols[muxInput] != this->muxcontrolsPrevious[muxInput])
        {
          // Calling the control callback if there is one
          if (this->inputsMuxControlsChangeCallback[muxInput] != nullptr)
          {
            this->inputsMuxControlsChangeCallback[muxInput](muxInput, this->muxcontrols[muxInput], this->muxcontrols[muxInput] - this->muxcontrolsPrevious[muxInput]);
          }
        }
        this->muxcontrolsPrevious[muxInput] = this->muxcontrols[muxInput];
      }
    }

    muxInput++;
    if (muxInput >= MnumControls)
      muxInput = 0;
    digitalWrite(MS0, muxInput & B0001);
    digitalWrite(MS1, muxInput & B0010);
    digitalWrite(MS2, muxInput & B0100);
    digitalWrite(MS3, muxInput & B1000);
  }
}

/**
 * Get input value
 * @param byte index The index of the input
 */
inline int NervousSuperMother::getInput(byte index)
{
  switch (this->inputs[index])
  {
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
    // Mux controls
    return this->muxcontrols[index];
    break;
  case 3:
    // Mux controls
    return this->switches[index];
    break;
  case 4:
    // Encoder
    // Device is not saving encoders values, only the latest change
    int value = this->encoders[index];
    this->encoders[index] = 0;
    return value;
    break;
  }
}

/**
 * Get min analog value according to resolution
 */
inline int NervousSuperMother::getAnalogMinValue()
{
  return 0;
}

/**
 * Get max analog value according to resolution
 */
inline int NervousSuperMother::getAnalogMaxValue()
{
  return (1 << this->analogResolution) - 1;
}

/**
 * Handle volume pot
 */
inline void NervousSuperMother::setHandleVolChange(volChangeCallback fptr)
{
  this->inputsvolChangeCallback = fptr;
}

/**
 * Handle switches
 */
inline void NervousSuperMother::setHandleSwitchChange(byte inputIndex, SwitchChangeCallback fptr)
{
  this->inputsSwitchesChangeCallback[inputIndex] = fptr;
}

/**
 * Handle Mux controls
 */
inline void NervousSuperMother::setHandleMuxControlChange(byte inputIndex, MuxControlChangeCallback fptr)
{
  this->inputsMuxControlsChangeCallback[inputIndex] = fptr;
}

/**
 * Get button value
 * @param byte inputeIndex The index of the input
 */
inline void NervousSuperMother::readButton(byte buttonIndex)
{
  // If there is a short or a long press callback on that input
  if (this->inputsPressCallback[buttonIndex] != nullptr ||
      this->inputsDoublePressCallback[buttonIndex] != nullptr ||
      this->inputsLongPressCallback[buttonIndex] != nullptr)
  {
    if (digital_button[buttonIndex].update())
    {
      if (digital_button[buttonIndex].fallingEdge())
      {
        this->pushed[buttonIndex] = true;
        if (this->inputsPressTime[buttonIndex] <= 400 && this->pushed[buttonIndex])
        {
          if (this->inputsDoublePressCallback[buttonIndex] != nullptr)
          {
            this->inputsDoublePressCallback[buttonIndex](buttonIndex);
          }
        }
        this->inputsPressTime[buttonIndex] = 0;
      }

      if (digital_button[buttonIndex].risingEdge())
      {
        if (this->inputsPressTime[buttonIndex] > 150 && this->inputsPressTime[buttonIndex] < 500)
        {
          if (this->inputsPressCallback[buttonIndex] != nullptr)
          {
            this->inputsPressCallback[buttonIndex](buttonIndex);
          }
        }
        this->pushed[buttonIndex] = false;
      }
    }
    else if (this->inputsPressTime[buttonIndex] >= 800 && this->pushed[buttonIndex])
    {
      if (this->inputsLongPressCallback[buttonIndex] != nullptr)
      {
        this->inputsLongPressCallback[buttonIndex](buttonIndex);
      }
      this->pushed[buttonIndex] = false;
    }
  }
}

/**
 * Handle press down on a button
 */
inline void NervousSuperMother::setHandlePress(byte inputIndex, PressCallback fptr)
{
  this->inputsPressCallback[inputIndex] = fptr;
}

/**
 * Handle press up on a button
 */
inline void NervousSuperMother::setHandleDoublePress(byte inputIndex, DoublePressCallback fptr)
{
  this->inputsDoublePressCallback[inputIndex] = fptr;
}

/**
 * Handle long press down on a button
 */
inline void NervousSuperMother::setHandleLongPress(byte inputIndex, LongPressCallback fptr)
{
  this->inputsLongPressCallback[inputIndex] = fptr;
}

/**
 * Get encoder value
 * @param byte inputeIndex The index of the input
 */
inline void NervousSuperMother::readEncoder(byte inputIndex)
{
  // Get rotary encoder value
  this->encoders[inputIndex] = encoders_knob[inputIndex].read();
  // Reverse
  if (this->encodersMaxValue[inputIndex] < 0)
  {
    if (this->encoders[inputIndex] < this->encodersMaxValue[inputIndex] - 2)
    {
      this->encoders[inputIndex] = 0;
      encoders_knob[inputIndex].write(0);
    }
    if (this->encoders[inputIndex] > 2)
    {
      this->encoders[inputIndex] = this->encodersMaxValue[inputIndex];
      encoders_knob[inputIndex].write(this->encodersMaxValue[inputIndex]);
    }
    // Normal
  }
  else
  {
    if (this->encoders[inputIndex] > this->encodersMaxValue[inputIndex] + 2)
    {
      this->encoders[inputIndex] = 0;
      encoders_knob[inputIndex].write(0);
    }
    if (this->encoders[inputIndex] < -2)
    {
      this->encoders[inputIndex] = this->encodersMaxValue[inputIndex];
      encoders_knob[inputIndex].write(this->encodersMaxValue[inputIndex]);
    }
  }
  if (this->encoders[inputIndex] % 4 == 0 && this->encoders[inputIndex] != this->encodersPrevious[inputIndex])
  {
    this->encodersPrevious[inputIndex] = this->encoders[inputIndex];
    // Calling the Encoder callback if there is one
    if (this->inputsEncoderChangeCallback[inputIndex] != nullptr)
    {
      this->inputsEncoderChangeCallback[inputIndex](inputIndex, abs(this->encoders[inputIndex]) / 4);
    }
  }
}

/**
 * Handle encoder change
 */
inline void NervousSuperMother::setHandleEncoderChange(byte inputIndex, EncoderChangeCallback fptr)
{
  this->inputsEncoderChangeCallback[inputIndex] = fptr;
}

/**
 * Update encoders max value
 * < 0 for reversed mode
 */
inline void NervousSuperMother::updateEncodeursMaxValue(byte inputIndex, long encoderMax)
{
  this->encodersMaxValue[inputIndex] = encoderMax * 4;
}

/**
 * Update encoders value
 */
inline void NervousSuperMother::updateEncodeursValue(byte inputIndex, long encoderValue)
{
  if (this->encodersMaxValue[inputIndex] < 0)
  {
    encoders_knob[inputIndex].write(-(encoderValue * 4));
  }
  else
  {
    encoders_knob[inputIndex].write(encoderValue * 4);
  }
}

/**
 * Get trigger
 * @param byte inputeIndex The index of the input
 */
inline void NervousSuperMother::readTrigger(byte inputIndex)
{
  digital_trigger[inputIndex].update();
  if (digital_trigger[inputIndex].fallingEdge())
  {
    if (this->inputsFallingTriggerCallback[inputIndex] != nullptr)
    {
      this->inputsFallingTriggerCallback[inputIndex](inputIndex);
    }
  }
  if (digital_trigger[inputIndex].risingEdge())
  {
    if (this->inputsRisingTriggerCallback[inputIndex] != nullptr)
    {
      this->inputsRisingTriggerCallback[inputIndex](inputIndex);
    }
  }
}

/**
 * Handle falling trigger
 */
inline void NervousSuperMother::setHandleFallingTrigger(byte inputIndex, FallingTriggerCallback fptr)
{
  this->inputsFallingTriggerCallback[inputIndex] = fptr;
}

/**
 * Handle rising trigger
 */
inline void NervousSuperMother::setHandleRisingTrigger(byte inputIndex, RisingTriggerCallback fptr)
{
  this->inputsRisingTriggerCallback[inputIndex] = fptr;
}

/**
 * Get CVx value
 * @param byte inputeIndex The index of the input
 */
inline void NervousSuperMother::readCV(byte inputIndex)
{
  cv_controls[inputIndex].update();
  if (cv_controls[inputIndex].hasChanged())
  {
    int value = cv_controls[inputIndex].getValue();
    if (value < 940)
    {
      this->cvs[inputIndex] = value;
    }
    if (this->cvs[inputIndex] != this->cvsPrevious[inputIndex])
    {
      // Calling the CV callback if there is one
      if (this->inputsCVChangeCallback[inputIndex] != nullptr)
      {
        this->inputsCVChangeCallback[inputIndex](inputIndex, this->cvs[inputIndex], this->cvs[inputIndex] - this->cvsPrevious[inputIndex]);
      }
    }
    this->cvsPrevious[inputIndex] = this->cvs[inputIndex];
  }
}

/**
 * Handle CVs
 */
inline void NervousSuperMother::setHandleCVChange(byte inputIndex, CVChangeCallback fptr)
{
  this->inputsCVChangeCallback[inputIndex] = fptr;
}

/**
 * Handle custom display refresh
 */
inline void NervousSuperMother::setCustomDisplayRefresh(CustomDisplayRefresh fptr)
{
  this->customDisplayRefresh = fptr;
}

/**
 * Update entire line to display
 */
inline void NervousSuperMother::updateLine(byte line_nb, String line)
{
  if (line_nb == 1)
  {
    this->display_line_1 = line;
    if (this->display_line_1.length() < 20)
    {
      for (byte i = this->display_line_1.length(); i < 20; i++)
      {
        this->display_line_1 = this->display_line_1 + " ";
      }
    }
    lcd.setCursor(0, 0);
    lcd.print(this->display_line_1);
  }
  else if (line_nb == 2)
  {
    this->display_line_2 = line;
    if (this->display_line_2.length() < 20)
    {
      for (byte i = this->display_line_2.length(); i < 20; i++)
      {
        this->display_line_2 = this->display_line_2 + " ";
      }
    }
    lcd.setCursor(0, 1);
    lcd.print(this->display_line_2);
  }
  else if (line_nb == -1)
  {
    lcd.setCursor(0, 0);
    lcd.print(this->display_line_1);
    lcd.setCursor(0, 1);
    lcd.print(this->display_line_2);
  }
}

/**
 * Update one char in line to display
 */
inline void NervousSuperMother::updateOneChar(byte line_nb, char one_char, int index)
{
  if (index < 20 && index >= 0)
  {
    if (line_nb == 1)
    {
      this->display_line_1[index] = one_char;
      lcd.setCursor(0, 0);
      lcd.print(this->display_line_1);
    }
    else if (line_nb == 2)
    {
      this->display_line_2[index] = one_char;
      lcd.setCursor(0, 1);
      lcd.print(this->display_line_2);
    }
  }
}

/**
 * Refresh the display
 */
inline void NervousSuperMother::refreshDisplay(bool force)
{
  if (force)
  {
    lcd.setCursor(0, 0);
    lcd.print(this->display_line_1);
    lcd.setCursor(0, 1);
    lcd.print(this->display_line_2);
  }
  else
  {
    if (this->display_line_1 != this->previous_display_line_1)
    {
      lcd.setCursor(0, 0);
      lcd.print(this->display_line_1);
      this->previous_display_line_1 = this->display_line_1;
    }
    if (this->display_line_2 != this->previous_display_line_2)
    {
      lcd.setCursor(0, 1);
      lcd.print(this->display_line_2);
      this->previous_display_line_2 = this->display_line_2;
    }
  }
  if (this->customDisplayRefresh != nullptr)
  {
    this->customDisplayRefresh();
  }
}

#endif
