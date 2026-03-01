/*
  ==============================================================================

    CombDelay.h
    Created: 23 Feb 2026 4:11:45pm
    Author:  laura

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CombDelay 
{
public:
    CombDelay() = default;
    void prepare(int maxDelaySamples, float sampleRate);
    void reset();
    void setDelayMs(float ms);
    float getDelayMs() { return delayMs; }
    void setFeedback(float fb) { feedback = juce::jlimit(0.0f, 0.999f, fb); }
    float getFeedback() { return feedback; }
    void processSample(float s);
    float read();
    void write(float output);

private:
    float sr = 44100.0f;
    int bufferSize = 0;
    int writePos = 0;
    float delayMs = 0.f;
    int delaySamples = 1;
    float feedback = 0.7f;

    std::vector<float> delayBuffer;
};