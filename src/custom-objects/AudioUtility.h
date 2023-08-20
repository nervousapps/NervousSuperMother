/*MIT License

Copyright (c) 2023 jerry20091103

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#ifndef AUDIO_UTILITY_H
#define AUDIO_UTILITY_H

#include "Arduino.h"

// * Data types

// Represents a single audio block of int16_t samples
struct audio_block_data_t
{
    int16_t data[AUDIO_BLOCK_SAMPLES];
};

// * Utility functions

inline float dBtoGain(float dB) { return pow10f(dB * 0.05); }

inline float gaintodB(float gain) { return 20 * log10f(gain); }

inline float noteToFreq(uint8_t note) { return powf(2, (float)(note - 69) / 12.0f) * 440.0f; }

#endif
