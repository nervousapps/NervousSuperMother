#include <Arduino.h>
#include <NervousSuperMother.h>

// Motherboard
NervousSuperMother * device = NervousSuperMother::getInstance();

void onPotentiometer(byte inputIndex, unsigned int value, int diffToPrevious) {
  Serial.print("Potentiometer ");
  Serial.print(inputIndex);
  Serial.print(" : ");
  Serial.print(value);
  Serial.print(" previous was ");
  Serial.println(diffToPrevious);
  char line = strcat("Potentiometer ", strcat(inputIndex, strcat(" : ", value)));
  device->updateLine(0, line);
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
}

void setup() {
  Serial.begin(9600);

  while (!Serial && millis() < 2500); // wait for serial monitor

  // Starting sequence
  Serial.println("Ready!");

  byte controls[6] = {0,1,2,3,4,5};
  device->init(controls);

  device->setHandlePotentiometerChange(0, onPotentiometer);
  device->setHandlePotentiometerChange(1, onPotentiometer);
  device->setHandlePotentiometerChange(2, onPotentiometer);
  device->setHandlePress(0, onButtonPress);
  device->setHandleLongPress(0, onButtonLongPress);
  device->setHandleDoublePress(0, onButtonDoublePress);
  device->setHandleEncoderChange(0, onEncoder);
  long encoderMaxValue[1] = {50};
  device->updateEncodeursMaxValue(encoderMaxValue);
  device->setHandleTrigger(0, onTrigger);
}

void loop() {
  device->update();
}
