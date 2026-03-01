/*
  ==============================================================================

    LFO.h
    Created: 24 Feb 2026 5:05:40pm
    Author:  laura

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class LFO
{
public:
    LFO();
    ~LFO();

    enum WaveType
    {
        Sine,
        Square,
        Triangle,
        Random
    };

    void updateLFOState(int bufferSize);
    void setPhase(double phase);
    void setFreq(float freq);
    void setSampleRate(double sr);
    void setWaveType(WaveType type) { waveType = type; }
    void initSampleHold();
    void updateSampleHold(int bufferSize);

    WaveType getWaveType() { return waveType; }

    void enableLFO(bool shoudBeEnabled);
    float getCurrentValue();
    bool isEnabled = false;
    float freq = 1.f;

private:
    float currentValue = 0.0f;
    float phase = 0.f;
    double sampleRate;
    WaveType waveType = WaveType::Sine;

    juce::Random random;
    int samplesToHold = 0;
};
