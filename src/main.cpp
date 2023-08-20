#include <Arduino.h>
#include <Wire.h>
#include <MIDI.h>
#include <mtof.h>
#include <math.h>

#include <NervousSuperMother.h>

// Motherboard
#ifndef DEVICE
#define DEVICE
NervousSuperMother *device = NervousSuperMother::getInstance();
#endif

MIDI_CREATE_DEFAULT_INSTANCE();

#include "SamplePlayer.h"

elapsedMillis volumeTimeout;

void onVolChange(float value)
{
  volumeTimeout = 0;
  String line = "       VOLUME       ";
  Serial.println(line);
  device->updateLine(1, line);
  AudioNoInterrupts();
  main_amplifier_L.gain(value / 400.0);
  main_amplifier_R.gain(value / 400.0);
  main_amplifier_PL.gain(value / 400.0);
  main_amplifier_PR.gain(value / 400.0);
  AudioInterrupts();
  draw_progressbar(map(value, 0, 1024, 0, 100));
}

void setup()
{
  Serial.begin(9600);

  while (!Serial && millis() < 2500)
    ; // wait for serial monitor

  // Starting sequence
  Serial.println("Ready!");

  init_sd_card();

  // Configure the ADCs
  analogReadResolution(10);
  analogReadAveraging(16);
  // analogReference(EXTERNAL);

  // Init audio
  AudioMemory(200);

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
  byte controls[7] = {0, 1, 2, 3, 4, 5, 6};
  device->init(controls);

  // Set device volume pot handler
  device->setHandleVolChange(onVolChange);

  // Init MIDI
  MIDI.begin(MIDI_CHANNEL);

  // Starting animation
  lcd.setCursor(0, 0);
  lcd.print("! NervouSuperSynth !");
  for (int i = 0; i < 100; i++)
  {
    draw_progressbar(i);
    delay(2);
  }

  setupSampleplayer();
}

void loop()
{
  MIDI.read();
  device->update();
  if (volumeTimeout > 1000)
  {
    device->refreshDisplay(true);
  }
}
