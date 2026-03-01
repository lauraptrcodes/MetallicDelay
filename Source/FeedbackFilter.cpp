/*
  ==============================================================================

    FeedbackFilter.cpp
    Created: 26 Feb 2026 6:20:05pm
    Author:  laura

  ==============================================================================
*/

#include "FeedbackFilter.h"

void FeedbackFilter::prepare(double sampleRate, int samplesPerBlock, int numChannels)
{
    highpass.reset();
    lowpass.reset();

    juce::dsp::ProcessSpec spec;

    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = numChannels;

    highpass.prepare(spec);
    lowpass.prepare(spec);
    
    this->sampleRate = sampleRate;
}

void FeedbackFilter::updateLowpass(float cutoffHz)
{
    auto coeffs = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, cutoffHz);
    *lowpass.coefficients = *coeffs;
}

void FeedbackFilter::updateHighpass(float cutoffHz)
{
    auto coeffs = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, cutoffHz);
    *highpass.coefficients = *coeffs;
}
