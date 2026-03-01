/*
  ==============================================================================

    LFO.cpp
    Created: 24 Feb 2026 5:05:40pm
    Author:  laura

  ==============================================================================
*/

#include "LFO.h"

LFO::LFO()
{
}

LFO::~LFO()
{
}

void LFO::updateLFOState(int bufferSize)
{
    float lfoPhaseIncrement = float(juce::MathConstants<float>::twoPi * freq) / (float(this->sampleRate / bufferSize));

    switch (waveType) {
    case Sine:     currentValue = (std::sin(phase) / 2.f + 0.5f); break;
    case Square:   currentValue = phase < juce::MathConstants<float>::pi ? 0.0f : 1.0f; break;
    case Triangle: currentValue = 2.0f * std::abs(phase - std::floor(phase + 0.5)); break;
    case Random:   updateSampleHold(bufferSize); break;
    }

    phase += lfoPhaseIncrement;

    if (phase >= 2 * juce::MathConstants<float>::pi)
        phase -= 2 * juce::MathConstants<float>::pi;
}

void LFO::setPhase(double phase)
{
    this->phase = phase;
}

void LFO::setFreq(float freq)
{
    this->freq = freq;
}

void LFO::setSampleRate(double sr)
{
    this->sampleRate = sr;
}

// reinit when freq changes & when samplesToHold == 0
void LFO::initSampleHold()
{
    currentValue = random.nextFloat();
    int freqInSamples = int(sampleRate / freq);
    samplesToHold = random.nextFloat() * freqInSamples;
    DBG(currentValue);
}

void LFO::updateSampleHold(int bufferSize)
{
    samplesToHold -= bufferSize;
    if (samplesToHold <= 0) initSampleHold();
}

void LFO::enableLFO(bool shoudBeEnabled)
{
    this->isEnabled = shoudBeEnabled;
}

float LFO::getCurrentValue()
{
    return currentValue;
}
