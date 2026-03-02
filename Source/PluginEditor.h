/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "mdLookAndFeel.h"

//==============================================================================
/**
*/
class MetallicDelayAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MetallicDelayAudioProcessorEditor (MetallicDelayAudioProcessor&, juce::AudioProcessorValueTreeState& vts);
    ~MetallicDelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    static constexpr int padding = 36;
private:
    void configLabel(juce::Label& l, juce::Component& attachedComp, juce::String text);

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayTimeAttachement;
    juce::Slider delayTimeSlider;
    juce::Label delayTimeLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackAttachement;
    juce::Slider feedbackSlider;
    juce::Label feedbackLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> modRateAttachement;
    juce::Slider modRateSlider;
    juce::Label modRateLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> modDepthAttachement;
    juce::Slider modDepthSlider;
    juce::Label modDepthLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modTypeAttachement;
    juce::ComboBox modTypeDropDown;
    juce::Label modTypeLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lpCutoffAttachement;
    juce::Slider lpCutoffSlider;
    juce::Label lpCutoffLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hpCutoffAttachement;
    juce::Slider hpCutoffSlider;
    juce::Label hpCutoffLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachement;
    juce::Slider driveSlider;
    juce::Label driveLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> distortionModeAttachement;
    juce::ComboBox distortionModeDropDown;
    juce::Label distortionModeLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputAttachement;
    juce::Slider outputSlider;
    juce::Label outputLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachement;
    juce::Slider mixSlider;
    juce::Label mixLabel;

    juce::AudioProcessorValueTreeState& valueTreeState;
    MetallicDelayAudioProcessor& audioProcessor;
    MDLookAndFeel lnf;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MetallicDelayAudioProcessorEditor)
};
