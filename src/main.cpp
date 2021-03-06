#include <Arduino.h>
#include <MIDI.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <NervousSuperMother.h>

// Motherboard
NervousSuperMother * device = NervousSuperMother::getInstance();

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

void onPotentiometer(byte inputIndex, unsigned int value, int diffToPrevious) {
  Serial.print("Potentiometer ");
  Serial.print(inputIndex);
  Serial.print(" : ");
  Serial.print(value);
  Serial.print(" previous was ");
  Serial.println(diffToPrevious);
  String line = "Potard " + String(inputIndex) + " : " + String(value);
  device->updateLine(1, line);
}

void onTrigger(byte inputIndex) {
  Serial.print("Trigger ! : ");
  Serial.println(inputIndex);
}

void onButtonPress(byte inputIndex) {
  Serial.print("Button short press ");
  Serial.println(inputIndex);
}

void onButtonLongPress(byte inputIndex) {
  Serial.print("Button long press ");
  Serial.println(inputIndex);
}

void onButtonDoublePress(byte inputIndex) {
  Serial.print("Button double press ");
  Serial.println(inputIndex);
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
  device->updateLine(2, line);
}

void OnNoteOn(byte channel, byte note, byte velocity) {
  String line = "MIDI " + String(channel) + " : " + String(note);
  device->updateLine(2, line);
}

void setup() {
  Serial.begin(9600);

  while (!Serial && millis() < 2500); // wait for serial monitor

  // Starting sequence
  Serial.println("Ready!");

  // Configure the ADCs
  analogReadResolution(7);
  analogReadAveraging(4);
  analogReference(EXTERNAL);

  // Init MIDI and set handlers
  MIDI.begin();
  MIDI.setHandleNoteOff(OnNoteOff);
  MIDI.setHandleNoteOn(OnNoteOn);

  // Init device NervousSuperMother
  byte controls[3] = {0,1,4};
  device->init(controls);

  // Set the handlers
  for (int i=0;i<ANALOG_CONTROL_PINS;i++){
    device->setHandlePotentiometerChange(i, onPotentiometer);
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
  device->setHandleTrigger(0, onTrigger);
  device->setHandleTrigger(1, onTrigger);
  device->setHandleTrigger(2, onTrigger);
  device->setHandleTrigger(3, onTrigger);
  device->setHandleTrigger(4, onTrigger);
  device->setHandleTrigger(5, onTrigger);

  // Starting animation
  lcd.setCursor(0,0);
  lcd.print("!    SuperSynth    !");
  for(int i=0; i<100; i++){
    draw_progressbar(i);
    delay(2);
  }

}

void loop() {
  MIDI.read();
  device->update();
}
