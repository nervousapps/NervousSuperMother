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

#ifndef RECORD_SAMPLE_H
#define RECORD_SAMPLE_H
#include "Audio.h"

// Get the middle sample in the current audio block.

class AudioRecordSample : public AudioStream
{
public:
    AudioRecordSample() : AudioStream(1, inputQueueArray)
    {
        isAvailable = false;
        sample = 0;
    }
    virtual void update(void)
    {
        audio_block_t *block;
        block = receiveReadOnly(0);
        if (block)
        {
            sample = block->data[AUDIO_BLOCK_SAMPLES / 2];
            isAvailable = true;
            release(block);
        }
    }
    // returns whether a new sample is available
    bool available(void)
    {
        bool ret;
        __disable_irq();
        ret = isAvailable;
        __enable_irq();
        return ret;
    }
    // returns a sample. If not available, returns the old value.
    int16_t readSample(void)
    {
        int16_t ret = 0;
        __disable_irq();
        if (isAvailable)
        {
            isAvailable = false;
        }
        ret = sample;
        __enable_irq();
        return ret;
    }

private:
    audio_block_t *inputQueueArray[1];
    bool isAvailable;
    int16_t sample;
};

#endif // RECORD_SAMPLE_H