/*
  ==============================================================================

    FeedbackFilter.h
    Created: 26 Feb 2026 6:20:05pm
    Author:  laura

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class FeedbackFilter {
public:
    void prepare(double sampleRate, int samplesPerBlock, int numChannels);
    void updateLowpass(float cutoffHz);
    void updateHighpass(float cutoffHz);

    float getLowCutoff() { return lowCutoff; }
    float getHighCutoff() { return highCutoff; }
    
    juce::dsp::IIR::Filter<float> highpass;
    juce::dsp::IIR::Filter<float> lowpass;
private:
    
    double sampleRate;
    float lowCutoff = 20.f;
    float highCutoff = 20000.f;
};