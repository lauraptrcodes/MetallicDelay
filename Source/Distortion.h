/*
  ==============================================================================

    Distortion.h
    Created: 27 Feb 2026 4:51:20pm
    Author:  laura

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Distortion {
public:
    Distortion() = default;
    ~Distortion() {}

    enum Mode {
        Softclip,
        Hardclip,
        Wavefold
    };

    float getAmount() { return drive; }
    Mode getMode() { return mode; }
    void setAmount(float amount) { drive = amount; }
    void setMode(Mode mode) { this->mode = mode; }

    float processSample(float x) {
        float shaped = shapeDrive();
        float preGain = juce::jmap(shaped, 0.0f, 1.0f, 1.0f, 10.0f);
        float threshold = juce::jmap(shaped, 1.f, 0.1f);
        float out = 0.f;

        switch (mode)
        {
        case Distortion::Softclip:
            return std::tanh(x * preGain);
        case Distortion::Hardclip:
            return juce::jlimit(-threshold, threshold, x * preGain);
        case Distortion::Wavefold:
            return wavefold(x * preGain);
        default:
            break;
        }
    }

private:

    float shapeDrive()
    {
        switch (mode)
        {
        case Mode::Softclip:  return std::pow(drive, 2.5f);
        case Mode::Hardclip:  return std::pow(drive, 2.0f);
        case Mode::Wavefold:  return std::pow(drive, 3.5f);
        }
    }

    float wavefold(float x) {
        return std::abs(std::fmod(x + 1.0f, 4.0f) - 2.0f) - 1.0f;
    }

    float drive = 0.f;
    Mode mode = Mode::Softclip;
};