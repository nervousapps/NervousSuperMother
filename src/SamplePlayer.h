#ifndef Sample_Player_h
#define Sample_Player_h

#include "Arduino.h"
#include <ArduinoJson.h>
#include <NervousSuperMother.h>
#include "SamplePlayerAudioConnections.h"
#include "main_utils.h"
#include "display_utils.h"

#ifndef DEVICE
#define DEVICE
NervousSuperMother *device = NervousSuperMother::getInstance();
#endif

// Root file
File root;
// Sample base dir on SD card
const char *sampleplayerBaseDir = "/SAMPLES/";

/** Menu variables */
volatile boolean directory = true; // In directory mode
volatile boolean sample = false;   // In sample mode
int sampleplayerStateEnc0 = 0;     // Encoder 0 mode
int sampleplayerStateEnc1 = 1;     // Encoder 1 mode
long player_index = 0;             // Sample player index
long enc0_value = 0;               // Encoder 0 value (not selected)
bool inverted = false;             // Player index selected
int numDirectory = 0;              // Actual directory number
int numFile = 0;                   // Actual file number
const String sampleEditorMenu[2] = {"Volume", "Nothing here..."};

/** SD fs variables */
struct Directories
{
  String directory;
  String files[150];
  int numberFile;
};
int totalDirectories = 0; // Total number of directories
EXTMEM Directories directoriesList[20];

/** Sample player variables */
char editableBank[NB_PLAYERS][100] = {};

/**
 * Splits a string at a specified separator and returns the substring at the specified index.
 *
 * @param data the string to be split
 * @param separator the character used to split the string
 * @param index the index of the substring to be returned
 *
 * @return the substring at the specified index
 *
 * @throws None
 */
String splitString(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

/**
 * Load a sample from the SD card.
 *
 * @param clipIndex the index of the clip to load
 *
 * @throws ErrorType if the sample fails to load
 */
void load_sd_sample(long clipIndex)
{
  // Retry 10 times to load sample
  for (int j = 0; j < 10; j++)
  {
    // Start recording
    recordclips[clipIndex].startRecording();
    // Play sample
    inputwavs[clipIndex].play(editableBank[clipIndex]);
    // Wait until the sample ends
    while (inputwavs[clipIndex].isPlaying())
    {
      // Starting animation
      draw_starting_animation("!  Loading sample  !", 0.5);
    }
    // Stop recording
    recordclips[clipIndex].stopRecording();
    // Sample has not been correctly recorded
    if (recordclips[clipIndex].getClip() == NULL || recordclips[clipIndex].getClipLength() < 10)
    {
      lcd.setCursor(0, 0);
      lcd.print("!Fail to load sample!");
      // Sample has been successfully recorded, set the clip of the corresponding player
    }
    else
    {
      switch (clipIndex)
      {
      case 0:
        playclip1.noteOff();
        playclip1.setClip(recordclips[clipIndex].getClip(), recordclips[clipIndex].getClipLength());
        break;

      case 1:
        playclip2.noteOff();
        playclip2.setClip(recordclips[clipIndex].getClip(), recordclips[clipIndex].getClipLength());
        break;

      case 2:
        playclip3.noteOff();
        playclip3.setClip(recordclips[clipIndex].getClip(), recordclips[clipIndex].getClipLength());
        break;

      case 3:
        playclip4.noteOff();
        playclip4.setClip(recordclips[clipIndex].getClip(), recordclips[clipIndex].getClipLength());
        break;
      }
      lcd.setCursor(0, 0);
      lcd.print("!  Sample loaded  !");
      delay(0.5);
      break;
    }
  }
  device->refreshDisplay(true);
}

/**
 * Plays a sample based on the given input index.
 * Has there are only two trigger inputs, only playclip1 and playsdwav[0] are played throught.
 * Player index 1 and 5.
 *
 * @param inputIndex the index of the input
 *
 * @throws ErrorType if there is an error playing the sample
 */
void sampleplay(byte inputIndex)
{
  if (inputIndex == 0)
  {
    playclip1.play();
    lcd.setCursor(18, 1);
    lcd.print("!");
  }
  else
  {
    playsdwavs[0].play(editableBank[4]);
    lcd.setCursor(19, 1);
    lcd.print("!");
  }
}

/**
 * Clears trigger character on the LCD display.
 *
 * @param inputIndex the index of the character to clear
 *
 * @return void
 *
 * @throws ErrorType This function does not throw any errors.
 */
void sampleclear(byte inputIndex)
{
  lcd.setCursor(18 + int(inputIndex), 1);
  lcd.print(" ");
}

/** Sliders handlers */
void setSpeed1(byte inputIndex, unsigned int value, int diffToPrevious)
{
  float valflo = float(value) / float(32);
  playclip1.setSpeed(valflo);
  device->updateLine(2, "SPEED 1 : " + String(valflo));
}

void setStart1(byte inputIndex, unsigned int value, int diffToPrevious)
{
  float valflo = float(value) / float(127);
  playclip1.setStartPoint(valflo);
  device->updateLine(2, "START 1 : " + String(valflo));
}

void setEnd1(byte inputIndex, unsigned int value, int diffToPrevious)
{
  float valflo = float(value) / float(127);
  playclip1.setEndPoint(valflo);
  device->updateLine(2, "END 1 : " + String(valflo));
}

void setLoopStartPoint1(byte inputIndex, unsigned int value, int diffToPrevious)
{
  float valflo = float(value) / float(127);
  playclip1.setLoopStartPoint(valflo);
  device->updateLine(2, "LOOP START 1 : " + String(valflo));
}

void setLoopEndPoint1(byte inputIndex, unsigned int value, int diffToPrevious)
{
  float valflo = float(value) / float(127);
  playclip1.setLoopEndPoint(valflo);
  device->updateLine(2, "LOOP END 1 : " + String(valflo));
}

void setLoopCrossfade1(byte inputIndex, unsigned int value, int diffToPrevious)
{
  float valflo = float(value) / float(127);
  playclip1.setLoopCrossFade(valflo);
  device->updateLine(2, "LOOP CROSS 1 : " + String(valflo));
}

void setLoop1(byte inputIndex, bool value)
{
  playclip1.loop(value);
  device->updateLine(2, "LOOP ENABLED 1 : " + String(value));
}

/** Potentiometers handlers */
void setVolSample1(byte inputIndex, unsigned int value, int diffToPrevious)
{
  AudioNoInterrupts();
  amplifiers[0].gain(value);
  AudioInterrupts();
  device->updateLine(2, "VOL sample 1 : " + String(value));
}

void setVolSample2(byte inputIndex, unsigned int value, int diffToPrevious)
{
  AudioNoInterrupts();
  amplifiers[1].gain(value);
  AudioInterrupts();
  device->updateLine(2, "VOL sample 2 : " + String(value));
}

void setVolSample5(byte inputIndex, unsigned int value, int diffToPrevious)
{
  AudioNoInterrupts();
  amplifiers[4].gain(value);
  AudioInterrupts();
  device->updateLine(2, "VOL sample 5 : " + String(value));
}

void setVolSample6(byte inputIndex, unsigned int value, int diffToPrevious)
{
  AudioNoInterrupts();
  amplifiers[5].gain(value);
  AudioInterrupts();
  device->updateLine(2, "VOL sample 6 : " + String(value));
}

/** Encoder 0 handlers */
void encoderHandler0(byte inputIndex, long value)
{
  switch (sampleplayerStateEnc0)
  {
  case 0:
    enc0_value = value;
    inverted = false;
    write_special_char(enc0_value, inverted);
    break;

  case 1:
    // Do nothing, sample number locked
    break;
  }
}

void simplePressHandler0(byte inputIndex)
{
  switch (sampleplayerStateEnc0)
  {
  case 0:
    player_index = enc0_value;
    inverted = true;
    write_special_char(player_index, inverted);
    sampleplayerStateEnc0 = 1;
    break;
  }
}

void doublePressHandler0(byte inputIndex)
{
  switch (sampleplayerStateEnc0)
  {
  case 1:
    device->updateEncodeursValue(0, player_index);
    inverted = false;
    write_special_char(player_index, inverted);
    sampleplayerStateEnc0 = 0;
    break;
  }
}

/** Encoder 1 handlers */
void encoderHandler1(byte inputIndex, long value)
{
  switch (sampleplayerStateEnc1)
  {
  case 0:
    // Volume
    break;

  case 1:
    Serial.print(value);
    Serial.print(" : ");
    Serial.println(directoriesList[numDirectory].directory);
    numDirectory = int(value);
    device->updateLine(1, ">" + directoriesList[numDirectory].directory);
    break;

  case 2:
    Serial.print(value);
    Serial.print(" : ");
    Serial.println(directoriesList[numDirectory].files[numFile]);
    numFile = value;
    device->updateLine(2, ">" + directoriesList[numDirectory].files[numFile]);
    break;
  }
}

void doublePressHandler1(byte inputIndex)
{
  switch (sampleplayerStateEnc1)
  {
  case 2:
    device->updateEncodeursMaxValue(1, totalDirectories - 1);
    device->updateEncodeursValue(1, numDirectory);
    device->updateLine(1, ">" + directoriesList[numDirectory].directory);
    device->updateLine(2, " ");
    sampleplayerStateEnc1 = 1;
    break;
  }
}

void longPressHandler(byte inputIndex)
{
}

void simplePressHandler1(byte inputIndex)
{
  switch (sampleplayerStateEnc1)
  {
  case 1:
    device->updateEncodeursMaxValue(1, directoriesList[numDirectory].numberFile - 1);
    device->updateEncodeursValue(1, numFile);
    device->updateLine(1, " " + directoriesList[numDirectory].directory);
    device->updateLine(2, ">" + directoriesList[numDirectory].files[numFile]);
    sampleplayerStateEnc1 = 2;
    break;

  case 2:
    strcpy(editableBank[player_index], sampleplayerBaseDir);
    strcat(editableBank[player_index], directoriesList[numDirectory].directory.c_str());
    strcat(editableBank[player_index], "/");
    strcat(editableBank[player_index], directoriesList[numDirectory].files[numFile].c_str());
    // No need to load sample for playSDwav, only for playClip
    if (player_index < 4)
    {
      load_sd_sample(player_index);
    }
  }
}

/** CVs handlers */
void onCV(byte inputIndex, unsigned int value, int diffToPrevious)
{
  Serial.print("CV : ");
  Serial.println(inputIndex);
  switch (inputIndex)
  {
  case 0:
    playclip1.setStartPoint(map(value, 1024, 0, 0.0, 1.0));
    break;

  case 1:
    playclip1.setEndPoint(map(value, 1024, 0, 0.0, 1.0));
    break;

  case 2:
    playclip1.setSpeed(map(value, 1024, 0, 0.0, 5.0));
    break;
  }
}

/** MIDI handlers */
void OnNoteOff(byte channel, byte note, byte velocity)
{
  String line = "MIDI " + String(channel) + " : " + String(note);
  Serial.println(line);
}

void OnNoteOn(byte channel, byte note, byte velocity)
{
  switch (note)
  {
  case 36:
    playclip2.play();
    break;

  case 37:
    playclip3.play();
    break;

  case 38:
    playclip4.play();
    break;

  case 39:
    playsdwavs[1].play(editableBank[5]);
    break;

  case 40:
    playsdwavs[2].play(editableBank[6]);
    break;

  case 41:
    playsdwavs[3].play(editableBank[7]);
    break;

  default:
    break;
  }
  String line = "MIDI " + String(channel) + " : " + String(note);
  Serial.println(line);
}

/** Custom display refresh */
void customDisplayRefresh()
{
  write_special_char(enc0_value, inverted);
}

/**
 * Recursively lists all directories and files within a given directory.
 *
 * @param dir The directory to list.
 *
 * @throws ErrorType If there is an error accessing the directory.
 */
void listDirectories(File dir)
{
  while (true)
  {
    File entry = dir.openNextFile();
    if (!entry)
    {
      // no more files
      directoriesList[numDirectory].numberFile = numFile;
      break;
    }
    if (entry.isDirectory())
    {
      directoriesList[numDirectory].directory = String(entry.name());
      numFile = 0;
      Serial.print(entry.name());
      Serial.println("/");
      listDirectories(entry);
      numDirectory++;
    }
    else
    {
      Serial.print("\t\t");
      Serial.println(entry.name());
      // files have sizes, directories do not
      directoriesList[numDirectory].files[numFile] = String(entry.name());
      numFile++;
    }
    entry.close();
  }
}

/**
 * Initializes the sample player banks.
 *
 * @return void
 */
void initSpBanks()
{
  if (hasSD)
  {
    // Open file for reading
    File root = SD.open(sampleplayerBaseDir);
    listDirectories(root);

    Serial.println(numDirectory);
    totalDirectories = numDirectory;
    for (int i = 0; i < numDirectory; i++)
    {
      Serial.print(directoriesList[i].directory);
      Serial.print(" -> ");
      Serial.println(directoriesList[i].numberFile);
      for (int j = 0; j < directoriesList[i].numberFile; j++)
      {
        Serial.print("\t");
        Serial.println(directoriesList[i].files[j]);
      }
    }
    numFile = 0;
    numDirectory = 0;
  }
  return;
}

/**
 * Initializes the SpAudio objects.
 */
void initSpAudioObjects()
{
  // Set mixers gain
  AudioNoInterrupts();
  for (int i = 0; i < 4; i++)
  {
    samplemixL.gain(i, 0.25);
    samplemixR.gain(i, 0.25);
    samplemixPTL.gain(i, 0.25);
    samplemixPTR.gain(i, 0.25);
  }

  for (int i = 0; i < 8; i++)
  {
    amplifiers[i].gain(3);
  }
  AudioInterrupts();
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

/**
 * Initializes the SP handlers for the device.
 *
 * @return void
 */
void initSpHandlers()
{
  device->setHandleRisingTrigger(0, sampleclear);
  device->setHandleRisingTrigger(1, sampleclear);
  device->setHandleFallingTrigger(0, sampleplay);
  device->setHandleFallingTrigger(1, sampleplay);

  device->setHandlePress(0, simplePressHandler0);
  device->setHandleDoublePress(0, doublePressHandler0);
  device->setHandlePress(1, simplePressHandler1);
  device->setHandleDoublePress(1, doublePressHandler1);

  device->setHandleEncoderChange(0, encoderHandler0);
  device->setHandleEncoderChange(1, encoderHandler1);

  device->updateEncodeursMaxValue(0, 1 - NB_PLAYERS);
  device->updateEncodeursValue(0, 0);
  device->updateEncodeursMaxValue(1, totalDirectories - 1);
  device->updateEncodeursValue(1, numDirectory);

  device->setHandleMuxControlChange(SLIDE1, setStart1);
  device->setHandleMuxControlChange(SLIDE2, setEnd1);
  device->setHandleMuxControlChange(SLIDE3, setSpeed1);
  device->setHandleMuxControlChange(SLIDE4, setLoopStartPoint1);
  device->setHandleMuxControlChange(SLIDE5, setLoopEndPoint1);
  device->setHandleMuxControlChange(SLIDE6, setLoopCrossfade1);

  device->setHandleMuxControlChange(POT1, setVolSample1);
  device->setHandleMuxControlChange(POT2, setVolSample2);
  device->setHandleMuxControlChange(POT3, setVolSample5);
  device->setHandleMuxControlChange(POT4, setVolSample6);

  device->setHandleCVChange(0, onCV);
  device->setHandleCVChange(1, onCV);
  device->setHandleCVChange(2, onCV);
  device->setHandleCVChange(3, onCV);

  device->setHandleSwitchChange(0, setLoop1);

  device->setCustomDisplayRefresh(customDisplayRefresh);
  device->updateLine(1, directoriesList[numDirectory].directory);

  // device->setHandleSwitchChange(0, activeHP);

  MIDI.setHandleNoteOff(OnNoteOff);
  MIDI.setHandleNoteOn(OnNoteOn);
}

/**
 * Initializes the sample player :
 *  -> Read SD card and fill banks
 *  -> Initialize audio objects
 *  -> Initialize device handlers
 *  -> Refresh the display
 *
 * @throws ErrorType description of error
 */
void setupSampleplayer()
{
  initSpBanks();
  initSpAudioObjects();
  initSpHandlers();
  device->refreshDisplay();
}

#endif