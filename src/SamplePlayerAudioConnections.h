#ifndef Sample_Player_connections_h
#define Sample_Player_connections_h

#include <Audio.h>
#include "custom-objects/CustomAudioObjects.h"

#define NB_PLAYERS 8 // MUST be /able by 2

AudioPlayClip playclip1;
AudioPlayClip playclip2;
AudioPlayClip playclip3;
AudioPlayClip playclip4;

// AudioPlayClip playclips[NB_PLAYERS/2] = {
//     playclip0,
//     playclip1,
//     playclip2,
//     playclip3
// };

AudioPlaySdWav* playsdwavs = new AudioPlaySdWav[NB_PLAYERS/2];

// Charge wav to array block
AudioRecordClip* recordclips = new AudioRecordClip[NB_PLAYERS/2];
AudioPlaySdWav* inputwavs = new AudioPlaySdWav[NB_PLAYERS/2];

AudioAmplifier* amplifiers = new AudioAmplifier[NB_PLAYERS];

AudioAmplifier main_amplifier_L;
AudioAmplifier main_amplifier_R;
AudioAmplifier main_amplifier_PL;
AudioAmplifier main_amplifier_PR;

AudioMixer4              samplemixL;
AudioMixer4              samplemixR;
AudioMixer4              samplemixPTL;
AudioMixer4              samplemixPTR;
AudioFilterStateVariable samplefilter1;
AudioFilterStateVariable samplefilter2;

AudioControlSGTL5000     sgtl5000_1;
AudioOutputI2S           i2s1;           //xy=686,170
AudioInputI2S            i2s2;           //xy=361,355
AudioOutputPT8211_2      pt8211_2_1;     //xy=703,281

AudioConnection          samplepatchCord0(playclip1, amplifiers[0]);
AudioConnection          samplepatchCord1(playclip2, amplifiers[1]);
AudioConnection          samplepatchCord2(playclip3, amplifiers[2]);
AudioConnection          samplepatchCord3(playclip4, amplifiers[3]);
AudioConnection          samplepatchCord4(playsdwavs[0], amplifiers[4]);
AudioConnection          samplepatchCord5(playsdwavs[1], amplifiers[5]);
AudioConnection          samplepatchCord6(playsdwavs[2], amplifiers[6]);
AudioConnection          samplepatchCord7(playsdwavs[3], amplifiers[7]);

AudioConnection          amppatchCord0(amplifiers[0], 0, samplemixL, 0);
AudioConnection          amppatchCord1(amplifiers[0], 0, samplemixR, 0);
AudioConnection          amppatchCord2(amplifiers[1], 0, samplemixL, 1);
AudioConnection          amppatchCord3(amplifiers[1], 0, samplemixR, 1);
AudioConnection          amppatchCord4(amplifiers[2], 0, samplemixL, 2);
AudioConnection          amppatchCord5(amplifiers[2], 0, samplemixR, 2);
AudioConnection          amppatchCord6(amplifiers[4], 0, samplemixL, 3);
AudioConnection          amppatchCord7(amplifiers[4], 0, samplemixR, 3);

AudioConnection          amppatchCord8(amplifiers[0], 0, samplemixPTL, 0);
AudioConnection          amppatchCord9(amplifiers[0], 0, samplemixPTR, 0);
AudioConnection          amppatchCord10(amplifiers[1], 0, samplemixPTL, 1);
AudioConnection          amppatchCord11(amplifiers[1], 0, samplemixPTR, 1);
AudioConnection          amppatchCord12(amplifiers[2], 0, samplemixPTL, 2);
AudioConnection          amppatchCord13(amplifiers[2], 0, samplemixPTR, 2);
AudioConnection          amppatchCord14(amplifiers[4], 0, samplemixPTL, 3);
AudioConnection          amppatchCord15(amplifiers[4], 0, samplemixPTR, 3);
// AudioConnection          samplepatchCord5(samplemix1, 0, samplefilter1, 0);
// AudioConnection          samplepatchCord6(samplemix2, 0, samplefilter2, 0);

AudioConnection          outputpatchCord0(samplemixL, 0, main_amplifier_L, 0);
AudioConnection          outputpatchCord1(samplemixR, 0, main_amplifier_R, 1);
AudioConnection          outputpatchCord2(main_amplifier_L, 0, i2s1, 0);
AudioConnection          outputpatchCord3(main_amplifier_R, 0, i2s1, 1);

AudioConnection          outputpatchCord4(samplemixPTL, 0, main_amplifier_PL, 0);
AudioConnection          outputpatchCord5(samplemixPTR, 0, main_amplifier_PR, 1);
AudioConnection          outputpatchCord6(main_amplifier_PL, 0, pt8211_2_1, 0);
AudioConnection          outputpatchCord7(main_amplifier_PR, 0, pt8211_2_1, 1);

AudioConnection          inputpatchCord0(inputwavs[0], recordclips[0]);
AudioConnection          inputpatchCord1(inputwavs[1], recordclips[1]);
AudioConnection          inputpatchCord2(inputwavs[2], recordclips[2]);
AudioConnection          inputpatchCord3(inputwavs[3], recordclips[3]);

#endif