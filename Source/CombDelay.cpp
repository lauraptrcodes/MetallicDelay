/*
  ==============================================================================

    CombDelay.cpp
    Created: 23 Feb 2026 4:11:45pm
    Author:  laura

  ==============================================================================
*/

#include "CombDelay.h"

void CombDelay::prepare(int maxDelaySamples, float sampleRate)
{
    sr = sampleRate;
    delayBuffer.resize(maxDelaySamples);
    bufferSize = maxDelaySamples;
    writePos = 0;
}

void CombDelay::reset()
{
    std::fill(delayBuffer.begin(), delayBuffer.end(), 0.0f);
    writePos = 0;
}

void CombDelay::setDelayMs(float ms)
{
    delayMs = ms;

    float samples = ms * 0.001f * sr;
    delaySamples = juce::jlimit(1, bufferSize - 1, (int)samples);
}

float CombDelay::read()
{
    int readPos = writePos - delaySamples;
    if (readPos < 0)
        readPos += bufferSize;

    return delayBuffer[readPos];
}

void CombDelay::write(float output)
{
    delayBuffer[writePos] = output;
    writePos = (writePos + 1) % bufferSize;
}
