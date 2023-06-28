#ifndef Sample_Player_h
#define Sample_Player_h

#include "Arduino.h"
#include <ArduinoJson.h>
#include "SamplePlayerAudioConnections.h"
#include "main_utils.h"
#include <NervousSuperMother.h>

File root;
const char* sampleplayerBaseDir = "/SAMPLES/";

volatile boolean directory = true;
volatile boolean sample = false;
int sampleplayerStateEnc0 = 0;
int sampleplayerStateEnc1 = 1;
int sample_number = 0;
int enc0_value = 0;
const String sampleEditorMenu[1] = {"Volume"};

int numFile = 0;
struct Directories{
  const String directory;
  const String files[150];
  int numberFile;
};

char editableBank[2][100] = {NULL};

int numDirectory = 0;
int numDirectorymax = 0;
Directories directoriesList[20];

volatile boolean sampleVolCtrl = false;

String splitString(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void load_sd_sample(){
  // Retry 3 times to load sample
  for(int j = 0; j<10; j++){
    recordclip.startRecording();
    inputwav.play(editableBank[sample_number]);
    device->updateLine(2, "!  Loading sample  !");
    delay(0.5);
    while(inputwav.isPlaying()){
      // Starting animation
      draw_starting_animation("", 0.1);
    }
    recordclip.stopRecording();
    if (recordclip.getClip() == NULL || recordclip.getClipLength() < 10){
      device->updateLine(2, "!Fail to load sample!");
    }else{
      playclip1.noteOff();
      playclip1.setClip(recordclip.getClip(), recordclip.getClipLength());
      device->updateLine(2, "!  Sample loaded  !");
      delay(0.5);
      break;
    }
  }
}

void sampleplay(byte inputIndex){
  if(editableBank[inputIndex] != NULL){
    audio_playclip[inputIndex].play();
    device->updateLine(2, "PLAYING SAMPLE " + String(inputIndex));
  }
}

void setSpeed1(byte inputIndex, unsigned int value, int diffToPrevious){
  float valflo = float(value)/float(32);
  playclip1.setSpeed(valflo);
  device->updateLine(2, "SPEED 1 : " + String(valflo));
}

void setStart1(byte inputIndex, unsigned int value, int diffToPrevious){
  float valflo = float(value)/float(127);
  playclip1.setStartPoint(valflo);
  device->updateLine(2, "START 1 : " + String(valflo));
}

void setEnd1(byte inputIndex, unsigned int value, int diffToPrevious){
  float valflo = float(value)/float(127);
  playclip1.setEndPoint(valflo);
  device->updateLine(2, "END 1 : " + String(valflo));
}

void encoderHandler0(byte inputIndex, long value){
  switch(sampleplayerStateEnc0){
    case 0:
    enc0_value = value;
    lcd.setCursor(0, 0);
    lcd.print("                   " + String(sample_number+1));
    break;

    case 1:
    audio_amp[sample_number].gain(value);
    lcd.setCursor(0, 0);
    lcd.print("Volume " + String(sample_number) + " : " + String(value));
    break;
  }
}

void simplePressHandler0(byte inputIndex){
  device->updateLine(1, "Switch enc " + String(inputIndex));
  switch(sampleplayerStateEnc0){
    case 0:
    sample_number = enc0_value;
    device->updateEncodeursMaxValue(0, sizeof(sampleEditorMenu));
    device->updateEncodeursValue(0, 0);
    sampleplayerStateEnc0 = 1;
    break;

    case 1:
    
    break;
  }
}

void doublePressHandler0(byte inputIndex){
  switch(sampleplayerStateEnc0){
    case 1:
    sampleplayerStateEnc0 = 0;
    device->updateEncodeursMaxValue(0, sizeof(audio_playclip));// + sizeof(audio_playwav));
    device->updateEncodeursValue(0, 0);
    break;
  }
}

void encoderHandler1(byte inputIndex, long value){
  switch(sampleplayerStateEnc1){
    case 1:
    Serial.print(value);
    Serial.print(" : ");
    Serial.println(directoriesList[numDirectory].directory);
    numDirectory = int(value);
    device->updateLine(1, directoriesList[numDirectory].directory);
    break;

    case 2:
    Serial.print(value);
    Serial.print(" : ");
    Serial.println(directoriesList[numDirectory].files[numFile]);
    numFile = value;
    device->updateLine(1, directoriesList[numDirectory].files[numFile]);
    break;
  }
}

void doublePressHandler1(byte inputIndex){
  switch(sampleplayerStateEnc1){
    case 2:
    device->updateEncodeursMaxValue(1, numDirectorymax-1);
    device->updateEncodeursValue(1, numDirectory);
    device->updateLine(1, directoriesList[numDirectory].directory);
    sampleplayerStateEnc1 = 1;
    break;
  }
}

void longPressHandler(byte inputIndex){

}

void simplePressHandler1(byte inputIndex){
  Serial.println("Switch Encoder 1");
  switch(sampleplayerStateEnc1){
    case 1:
    device->updateEncodeursMaxValue(1, directoriesList[numDirectory].numberFile-1);
    device->updateEncodeursValue(1, numFile);
    device->updateLine(1, directoriesList[numDirectory].files[numFile]);
    sampleplayerStateEnc1 = 2;
    break;

    case 2:
    strcpy(editableBank[sample_number], sampleplayerBaseDir);
    strcat(editableBank[sample_number], directoriesList[numDirectory].directory.c_str());
    strcat(editableBank[sample_number], "/");
    strcat(editableBank[sample_number], directoriesList[numDirectory].files[numFile].c_str());
    load_sd_sample();
  }
}

void OnNoteOff(byte channel, byte note, byte velocity) {
  String line = "MIDI " + String(channel) + " : " + String(note);
  Serial.println(line);
}

void OnNoteOn(byte channel, byte note, byte velocity) {
  int sample_note = map(note, 0, 127, 0, 6);
  // if(editableBank[sample_note] != NULL){
  //   audio_playwav[sample_note].play(editableBank[sample_number]);
  // }
  device->updateLine(2, "PLAYING SAMPLE " + String(sample_number+2));
  String line = "MIDI " + String(channel) + " : " + String(note);
  Serial.println(line);
}

void listDirectories(File dir) {
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      directoriesList[numDirectory].numberFile = numFile;
      break;
    }
    if (entry.isDirectory()) {
      directoriesList[numDirectory].directory = entry.name();
      numFile = 0;
      Serial.print(entry.name());
      Serial.println("/");
      listDirectories(entry);
      numDirectory++;
    } else {
      Serial.print("\t\t");
      Serial.println(entry.name());
      // files have sizes, directories do not
      directoriesList[numDirectory].files[numFile] = entry.name();
      numFile ++;
    }
    entry.close();
  }
}

void initSpAudioObjects(){
  // Set mixers gain
  for(int i = 0; i<4; i++){
    samplemixL.gain(i, 0.25);
    samplemixR.gain(i, 0.25);
    samplemixPTL.gain(i, 0.25);
    samplemixPTR.gain(i, 0.25);
  }

  for(int i = 0; i<sizeof(audio_amp); i++){
    audio_amp[i].gain(0.5);
  }
}

void initSpBanks(){
  if (hasSD) {
    // Open file for reading
    File root = SD.open(sampleplayerBaseDir);
    listDirectories(root);

    Serial.println(numDirectory);
    numDirectorymax = numDirectory;
    for(int i = 0; i<numDirectory; i++){
      Serial.print(directoriesList[i].directory);
      Serial.print(" -> ");
      Serial.println(directoriesList[i].numberFile);
      for(int j = 0; j<directoriesList[i].numberFile; j++){
        Serial.print("\t");
        Serial.println(directoriesList[i].files[j]);
      }
    }
    numFile = 0;
    numDirectory = 0;
  }
  return;
}

// void activeHP(byte inputeIndex, bool value){
//   if(value){
//     sgtl5000_1.adcHighPassFilterDisable();
//     device->updateLine(2, "HP OFF");
//   }else{
//     sgtl5000_1.adcHighPassFilterEnable();
//     device->updateLine(2, "HP ON");
//   }
// }

void initSpHandlers(){
  device->setHandleRisingTrigger(0, sampleplay);
  device->setHandleRisingTrigger(1, sampleplay);

  device->setHandlePress(0, simplePressHandler0);
  device->setHandleDoublePress(0, doublePressHandler0);
  device->setHandleEncoderChange(0, encoderHandler0);
  device->setHandlePress(1, simplePressHandler0);
  device->setHandleDoublePress(1, doublePressHandler1);
  device->setHandleEncoderChange(1, encoderHandler1);
  device->setHandleMuxControlChange(SLIDE1, setStart1);
  device->setHandleMuxControlChange(SLIDE2, setEnd1);
  device->setHandleMuxControlChange(SLIDE3, setSpeed1);

  device->updateEncodeursMaxValue(0, sizeof(audio_playclip));// + sizeof(audio_playwav));
  device->updateEncodeursValue(0, 0);
  device->updateEncodeursMaxValue(1, numDirectorymax-1);
  device->updateEncodeursValue(1, numDirectory);

  // device->setHandleSwitchChange(0, activeHP);

  MIDI.setHandleNoteOff(OnNoteOff);
  MIDI.setHandleNoteOn(OnNoteOn);
}

void setupSampleplayer(){
  initSpBanks();
  initSpAudioObjects();
  initSpHandlers();
}

#endif