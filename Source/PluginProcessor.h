/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CombDelay.h"
#include "LFO.h"
#include "FeedbackFilter.h"
#include "Distortion.h"

//==============================================================================
/**
*/
class MetallicDelayAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    MetallicDelayAudioProcessor();
    ~MetallicDelayAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    juce::AudioProcessorValueTreeState params;
    std::atomic<float>* delayTimeParam = nullptr;
    std::atomic<float>* feedbackParam = nullptr;
    std::atomic<float>* modRateParam = nullptr;
    std::atomic<float>* modDepthParam = nullptr;
    std::atomic<float>* modTypeParam = nullptr;
    std::atomic<float>* hpCutoffParam = nullptr;
    std::atomic<float>* lpCutoffParam = nullptr;
    std::atomic<float>* driveParam = nullptr;
    std::atomic<float>* distortionModeParam = nullptr;
    std::atomic<float>* outputParam = nullptr;
    std::atomic<float>* mixParam = nullptr;

    float previousOutput = 0.f;
    float gainLinear = 0.f;
    float previousMix = 0.5f;
    float dryGain = 0.5f;
    float wetGain = 0.5f;
    void setDryWetMix();
    
    //CombDelay delay;
    std::vector<CombDelay> combDelays;
    FeedbackFilter feedbackFilter;
    Distortion distortion;
    LFO lfo;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MetallicDelayAudioProcessor)
};
