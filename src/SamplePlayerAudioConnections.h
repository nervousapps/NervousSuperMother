#ifndef Sample_Player_connections_h
#define Sample_Player_connections_h

#include <Audio.h>
#include "custom-objects/CustomAudioObjects.h"


AudioPlayClip            playclip1;
// AudioPlayClip            playclip2;
// AudioPlaySdWav           playwav1;
// AudioPlaySdWav           playwav2;
// AudioPlaySdWav           playwav3;
// AudioPlaySdWav           playwav4;
// AudioPlaySdWav           playwav5;
// AudioPlaySdWav           playwav6;

AudioAmplifier           sampleamp1;
AudioAmplifier           sampleamp2;
AudioAmplifier           sampleamp3;
AudioAmplifier           sampleamp4;
AudioAmplifier           sampleamp5;
AudioAmplifier           sampleamp6;
AudioAmplifier           sampleamp7;
AudioAmplifier           sampleamp8;

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

// Charge wav to array block
AudioRecordClip          recordclip;
AudioPlaySdWav           inputwav;

AudioConnection          samplepatchCord0(playclip1, sampleamp1);
// AudioConnection          samplepatchCord1(playclip2, sampleamp2);
// AudioConnection          samplepatchCord2(playwav1, sampleamp3);
// AudioConnection          samplepatchCord3(playwav2, sampleamp4);
// AudioConnection          samplepatchCord4(playwav3, sampleamp5);
// AudioConnection          samplepatchCord5(playwav4, sampleamp6);
// AudioConnection          samplepatchCord6(playwav5, sampleamp7);
// AudioConnection          samplepatchCord7(playwav6, sampleamp8);

AudioConnection          amppatchCord0(sampleamp1, 0, samplemixL, 0);
AudioConnection          amppatchCord1(sampleamp1, 0, samplemixR, 0);
AudioConnection          amppatchCord2(sampleamp2, 0, samplemixL, 1);
AudioConnection          amppatchCord3(sampleamp2, 0, samplemixR, 1);
AudioConnection          amppatchCord4(sampleamp3, 0, samplemixL, 2);
AudioConnection          amppatchCord5(sampleamp3, 0, samplemixR, 2);
AudioConnection          amppatchCord6(sampleamp4, 0, samplemixL, 3);
AudioConnection          amppatchCord7(sampleamp4, 0, samplemixR, 3);
AudioConnection          amppatchCord8(sampleamp1, 0, samplemixPTL, 0);
AudioConnection          amppatchCord9(sampleamp1, 0, samplemixPTR, 0);
AudioConnection          amppatchCord10(sampleamp2, 0, samplemixPTL, 1);
AudioConnection          amppatchCord11(sampleamp2, 0, samplemixPTR, 1);
AudioConnection          amppatchCord12(sampleamp3, 0, samplemixPTL, 2);
AudioConnection          amppatchCord13(sampleamp3, 0, samplemixPTR, 2);
AudioConnection          amppatchCord14(sampleamp4, 0, samplemixPTL, 3);
AudioConnection          amppatchCord15(sampleamp4, 0, samplemixPTR, 3);
// AudioConnection          samplepatchCord5(samplemix1, 0, samplefilter1, 0);
// AudioConnection          samplepatchCord6(samplemix2, 0, samplefilter2, 0);

AudioConnection          outputpatchCord0(samplemixL, 0, i2s1, 0);
AudioConnection          outputpatchCord1(samplemixR, 0, i2s1, 1);

AudioConnection          outputpatchCord2(samplemixPTL, 0, pt8211_2_1, 0);
AudioConnection          outputpatchCord3(samplemixPTR, 0, pt8211_2_1, 1);

AudioConnection          inputpatchCord0(inputwav, recordclip);


// AudioPlaySdWav audio_playwav[6] = {
//     playwav1,
//     playwav2,
//     playwav3,
//     playwav4,
//     playwav5,
//     playwav6
// };

AudioPlayClip audio_playclip[1] = {
    playclip1
    // playclip2
};

AudioAmplifier audio_amp[8] = {
    sampleamp1,
    sampleamp2,
    sampleamp3,
    sampleamp4,
    sampleamp5,
    sampleamp6,
    sampleamp7,
    sampleamp8
};

#endif