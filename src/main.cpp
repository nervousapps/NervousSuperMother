#include <Arduino.h>
#include <Wire.h>
#include <MIDI.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Audio.h>
#include <mtof.h>
#include <math.h>

// GUItool: begin automatically generated code
AudioSynthSimpleDrum     drum1;          //xy=308,171
AudioSynthWaveform       waveform1;      //xy=309,282
AudioAmplifier           amp1;           //xy=526,155
AudioAmplifier           amp3;           //xy=526,266
AudioControlSGTL5000     sgtl5000_1;
AudioOutputI2S           i2s1;           //xy=686,170
AudioInputI2S            i2s2;           //xy=361,355
AudioOutputPT8211_2      pt8211_2_1;     //xy=703,281
AudioConnection          patchCord1(drum1, amp1);
AudioConnection          patchCord2(waveform1, amp3);
AudioConnection          patchCord3(amp1, 0, i2s1, 0);
AudioConnection          patchCord4(amp1, 0, i2s1, 1);
// AudioConnection          patchCord5(waveform1, 0, pt8211_2_1, 0);
// AudioConnection          patchCord6(waveform1, 0, pt8211_2_1, 1);
AudioConnection          inputPatchchCord1(i2s2, 0, pt8211_2_1, 0);
AudioConnection          inputPatchchCord2(i2s2, 1, pt8211_2_1, 1);
// GUItool: end automatically generated code

#include <NervousSuperMother.h>

// Motherboard
NervousSuperMother * device = NervousSuperMother::getInstance();

MIDI_CREATE_DEFAULT_INSTANCE();

void onMuxControl(byte inputIndex, unsigned int value, int diffToPrevious) {
  String line = "";
  switch(inputIndex){
    case SLIDE1:
    line = "SLIDE1";
    break;
    case SLIDE2:
    line = "SLIDE2";
    break;
    case SLIDE3:
    line = "SLIDE3";
    break;
    case SLIDE4:
    line = "SLIDE4";
    break;
    case SLIDE5:
    line = "SLIDE5";
    break;
    case SLIDE6:
    line = "SLIDE6";
    break;
    case SLIDE7:
    line = "SLIDE7";
    break;
    case SLIDE8:
    line = "SLIDE8";
    break;
    case SLIDE9:
    line = "SLIDE9";
    break;
    case SLIDE10:
    line = "SLIDE10";
    break;

    // Potentiometers
    case POT1:
    line = "POT1";
    break;
    case POT2:
    line = "POT2";
    break;
    case POT3:
    line = "POT3";
    break;
    case POT4:
    line = "POT4";
    break;
    case POT5:
    line = "POT5";
    break;
  }
  Serial.print(line + " ");
  Serial.print(inputIndex);
  Serial.print(" : ");
  Serial.print(value);
  Serial.print(" previous was ");
  Serial.println(diffToPrevious);
  line = line + " : " + String(value);
  device->updateLine(1, line);
}

void onSwitchControl(byte inputIndex, bool value) {
  Serial.print("SwitchControl :  ");
  Serial.print(inputIndex);
  Serial.print(" : ");
  Serial.print(value);
  String line = "SwitchControl " + String(inputIndex) + " : " + String(value);
  device->updateLine(1, line);
}

void onTrigger(byte inputIndex) {
  Serial.print("Trigger ! : ");
  Serial.println(inputIndex);
  String line = "Trigger ! : " + String(inputIndex);
  device->updateLine(1, line);
  drum1.noteOn();
}

float pitch_offset = 1;
float max_voltage_of_adc = 3.3;
float voltage_division_ratio = 0.333333333;
float notes_per_octave = 12;
float volts_per_octave = 1;
float mapping_upper_limit = (max_voltage_of_adc / voltage_division_ratio) * notes_per_octave * volts_per_octave;

void onCV(byte inputIndex, unsigned int value, int diffToPrevious) {
  float pitch = pitch_offset + map(value,1024,0, 0.0, mapping_upper_limit);
  float freq = mtof.toFrequency(pitch);
  waveform1.frequency(freq);
  Serial.print("CV : ");
  Serial.println(inputIndex);
  String line = "CV " + String(inputIndex) + " : " + String(value)+ " : " + String(pitch);
  device->updateLine(1, line);
  String line2 = "freq->" + String(freq) + " M" + String(mapping_upper_limit);
  device->updateLine(2, line2);
}

void onButtonPress(byte inputIndex) {
  Serial.print("Button short press ");
  Serial.println(inputIndex);
  String line = "Button short press " + String(inputIndex);
  device->updateLine(1, line);
}

void onButtonLongPress(byte inputIndex) {
  Serial.print("Button long press ");
  Serial.println(inputIndex);
  String line = "Button long press " + String(inputIndex);
  device->updateLine(1, line);
}

void onButtonDoublePress(byte inputIndex) {
  Serial.print("Button double press ");
  Serial.println(inputIndex);
  String line = "Button double press " + String(inputIndex);
  device->updateLine(1, line);
}

void onEncoder(byte inputIndex, long value) {
  Serial.print("Encoder ");
  Serial.print(inputIndex);
  Serial.print(" : ");
  Serial.println(value);
  String line = "Encoder " + String(inputIndex) + " : " + String(value);
  device->updateLine(2, line);
}

void OnNoteOff(byte channel, byte note, byte velocity) {
  String line = "MIDI " + String(channel) + " : " + String(note);
  Serial.println(line);
  device->updateLine(2, line);
}

void OnNoteOn(byte channel, byte note, byte velocity) {
  String line = "MIDI " + String(channel) + " : " + String(note);
  Serial.println(line);
  device->updateLine(2, line);
  waveform1.frequency(note);
}

void onVolChange(float value) {
  String line = "VOL : " + String(value);
  Serial.println(line);
  device->updateLine(2, line);
  // AudioNoInterrupts();
  amp1.gain(value/1000.0);
  amp3.gain(value/1000.0);
  // sgtl5000_1.lineOutLevel(value/1000.0);
  // AudioInterrupts();
  // draw_progressbar(value/10);
}

void setup() {
  Serial.begin(9600);

  // Set TX of Serial1 to 53 instead of 1 as encoder 1 uses that pin.
  // Serial1.setTX(53);

  while (!Serial && millis() < 2500); // wait for serial monitor

  // Starting sequence
  Serial.println("Ready!");

  // Configure the ADCs
  analogReadResolution(10);
  analogReadAveraging(16);
  // analogReference(EXTERNAL);

  // Init audio
  AudioMemory(100);

  AudioNoInterrupts();

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.32);
  sgtl5000_1.lineOutLevel(0.32);
  sgtl5000_1.dacVolume(1.0);
  sgtl5000_1.muteHeadphone();
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.audioPreProcessorEnable();
  sgtl5000_1.audioPostProcessorEnable();
  sgtl5000_1.surroundSoundEnable();
  sgtl5000_1.enhanceBassEnable();

  amp1.gain(0.5);
  amp3.gain(0.5);

  drum1.frequency(60);
  drum1.length(1500);
  drum1.secondMix(0.0);
  drum1.pitchMod(0.55);

  AudioInterrupts();

  waveform1.begin(1.0, 60, WAVEFORM_SAWTOOTH);

  // Init device NervousSuperMother
  byte controls[7] = {0,1,2,3,4,5,6};
  device->init(controls);

  // Set the handlers
  for (int i=0;i<MnumControls;i++){
    device->setHandleMuxControlChange(i, onMuxControl);
  }
  device->setHandlePress(0, onButtonPress);
  device->setHandleLongPress(0, onButtonLongPress);
  device->setHandleDoublePress(0, onButtonDoublePress);
  device->setHandlePress(1, onButtonPress);
  device->setHandleLongPress(1, onButtonLongPress);
  device->setHandleDoublePress(1, onButtonDoublePress);
  device->setHandleEncoderChange(0, onEncoder);
  device->setHandleEncoderChange(1, onEncoder);
  device->updateEncodeursMaxValue(0, -50);
  device->updateEncodeursMaxValue(1, 30);
  device->setHandleRisingTrigger(0, onTrigger);
  device->setHandleRisingTrigger(1, onTrigger);
  device->setHandleCVChange(0, onCV);
  device->setHandleCVChange(1, onCV);
  device->setHandleCVChange(2, onCV);
  device->setHandleCVChange(3, onCV);
  for (int i=0;i<ANALOG_CONTROL_PINS;i++){
    device->setHandleSwitchChange(i, onSwitchControl);
  }
  device->setHandleVolChange(onVolChange);

  // Init MIDI and set handlers
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOff(OnNoteOff);
  MIDI.setHandleNoteOn(OnNoteOn);

  // Starting animation
  String starting_message = "! NervouSuperSynth !";
  draw_starting_animation(starting_message, 25);
  delay(1000);

}

void loop() {
  MIDI.read();
  device->update();
}
