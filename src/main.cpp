#include <Arduino.h>
#include <Wire.h>
#include <MIDI.h>
#include <mtof.h>
#include <math.h>

#include <NervousSuperMother.h>

// Motherboard
#ifndef DEVICE
#define DEVICE
NervousSuperMother * device = NervousSuperMother::getInstance();
#endif

MIDI_CREATE_DEFAULT_INSTANCE();

#include "SamplePlayer.h"

float pitch_offset = 1;
float max_voltage_of_adc = 3.3;
float voltage_division_ratio = 0.333333333;
float notes_per_octave = 12;
float volts_per_octave = 1;
float mapping_upper_limit = (max_voltage_of_adc / voltage_division_ratio) * notes_per_octave * volts_per_octave;

void onCV(byte inputIndex, unsigned int value, int diffToPrevious) {
  // float pitch = pitch_offset + map(value,1024,0, 0.0, mapping_upper_limit);
  // float freq = mtof.toFrequency(pitch);
  Serial.print("CV : ");
  Serial.println(inputIndex);
  switch(inputIndex){
    case 0:
    playclip1.setStartPoint(map(value,1024,0, 0.0, 1.0));
    break;

    case 1:
    playclip1.setEndPoint(map(value,1024,0, 0.0, 1.0));
    break;

    case 2:
    playclip1.setSpeed(map(value,1024,0, 0.0, 5.0));
    break;
  }
}

void onVolChange(float value) {
  String line = "VOL : " + String(value);
  Serial.println(line);
  device->updateLine(1, line);
  AudioNoInterrupts();
  main_amplifier_L.gain(value/400.0);
  main_amplifier_R.gain(value/400.0);
  main_amplifier_PL.gain(value/400.0);
  main_amplifier_PR.gain(value/400.0);
  AudioInterrupts();
  draw_progressbar(map(value, 0, 1024, 0, 100));
  device->refreshDisplay(true);
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

void setup() {
  Serial.begin(9600);

  while (!Serial && millis() < 2500); // wait for serial monitor

  // Starting sequence
  Serial.println("Ready!");

  init_sd_card();

  // Configure the ADCs
  analogReadResolution(10);
  analogReadAveraging(16);
  // analogReference(EXTERNAL);

  // Init audio
  AudioMemory(500);

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

  AudioInterrupts();

  // Init device NervousSuperMother
  byte controls[7] = {0,1,2,3,4,5,6};
  device->init(controls);

  // Set device handlers
  device->setHandleCVChange(0, onCV);
  device->setHandleCVChange(1, onCV);
  device->setHandleCVChange(2, onCV);
  device->setHandleCVChange(3, onCV);
  device->setHandleVolChange(onVolChange);

  // Init MIDI
  MIDI.begin(16);

  // Starting animation
  lcd.setCursor(0,0);
  lcd.print("! NervouSuperSynth !");
  for(int i=0; i<100; i++){
    draw_progressbar(i);
    delay(2);
  }

  setupSampleplayer();

}

void loop() {
  MIDI.read();
  device->update();
} 
