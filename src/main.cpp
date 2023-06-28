#include <Arduino.h>
#include <Wire.h>
#include <MIDI.h>
#include <mtof.h>
#include <math.h>

#include <NervousSuperMother.h>

// Motherboard
NervousSuperMother * device = NervousSuperMother::getInstance();

MIDI_CREATE_DEFAULT_INSTANCE();

#include "SamplePlayer.h"


void onSwitchControl(byte inputIndex, bool value) {
  Serial.print("SwitchControl :  ");
  Serial.print(inputIndex);
  Serial.print(" : ");
  Serial.print(value);
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
  Serial.print("CV : ");
  Serial.println(inputIndex);
  switch(inputIndex){
    case 0:
    playclip1.setStartPoint(float(value)/float(1000));
    break;

    case 1:
    playclip1.setEndPoint(float(value)/float(1000));
    break;

    case 2:
    playclip1.setSpeed(float(value)/float(300));
    break;
  }
}

void onVolChange(float value) {
  String line = "VOL : " + String(value);
  Serial.println(line);
  // device->updateLine(2, line);
  // AudioNoInterrupts();
  audio_amp[sample_number].gain(value/1000.0);
  // sgtl5000_1.lineOutLevel(value/1000.0);
  // AudioInterrupts();
  // draw_progressbar(value/10);
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

  AudioInterrupts();

  // Init device NervousSuperMother
  byte controls[7] = {0,1,2,3,4,5,6};
  device->init(controls);

  // Set the handlers
  device->setHandleCVChange(0, onCV);
  device->setHandleCVChange(1, onCV);
  device->setHandleCVChange(2, onCV);
  device->setHandleCVChange(3, onCV);
  for (int i=0;i<ANALOG_CONTROL_PINS;i++){
    device->setHandleSwitchChange(i, onSwitchControl);
  }
  device->setHandleVolChange(onVolChange);

  // Init MIDI
  MIDI.begin(MIDI_CHANNEL_OMNI);

  // Starting animation
  String starting_message = "! NervouSuperSynth !";
  for(int i=0; i<20; i++){
    lcd.setCursor(i,0);
    lcd.print(starting_message[i]);
    lcd.setCursor(i,1);
    lcd.print("=");
    if(i<19){
      lcd.setCursor(i+1,1);
      lcd.print(">");
    }
    delay(25);
  }
  delay(100);

  setupSampleplayer();

}

void loop() {
  MIDI.read();
  device->update();
}
