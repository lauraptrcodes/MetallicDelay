/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace UIColours
{
    // Base
    static const juce::Colour background = juce::Colour::fromString("#121212");
    static const juce::Colour panel = juce::Colour::fromString("#252525");

    // Accents
    static const juce::Colour accent = juce::Colour::fromString("#E03131");
    static const juce::Colour accentSoft = juce::Colour::fromString("#CCFF6A00");

    // Text
    static const juce::Colour textPrimary = juce::Colour::fromString("#FFFFFF");
    static const juce::Colour textMuted = juce::Colour::fromString("#FFADADAD");

    // States
    static const juce::Colour hover = juce::Colour::fromString("#333333");
    static const juce::Colour disabled = juce::Colour::fromString("#666666");
}

//==============================================================================
MetallicDelayAudioProcessorEditor::MetallicDelayAudioProcessorEditor (MetallicDelayAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(vts)
{
    juce::LookAndFeel::setDefaultLookAndFeel(&lnf);

    delayTimeAttachement.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "delayTime", delayTimeSlider));
    delayTimeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    delayTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(delayTimeSlider);
    configLabel(delayTimeLabel, delayTimeSlider, "delay time");

    feedbackAttachement.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "feedback", feedbackSlider));
    feedbackSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    feedbackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(feedbackSlider);
    configLabel(feedbackLabel, feedbackSlider, "feedback");

    modDepthAttachement.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "modDepth", modDepthSlider));
    modDepthSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    modDepthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(modDepthSlider);
    configLabel(modDepthLabel, modDepthSlider, "mod depth");

    modRateAttachement.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "modRate", modRateSlider));
    modRateSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    modRateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(modRateSlider);
    configLabel(modRateLabel, modRateSlider, "mod rate");

    modTypeAttachement.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(valueTreeState, "modType", modTypeDropDown));
    modTypeDropDown.addItemList(juce::StringArray{ "Sine", "Square", "Triangle", "S&H Noise"}, 1);
    modTypeDropDown.setSelectedId(1);
    addAndMakeVisible(modTypeDropDown);
    configLabel(modTypeLabel, modTypeDropDown, "shape");

    hpCutoffAttachement.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "hpCutoff", hpCutoffSlider));
    hpCutoffSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    hpCutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(hpCutoffSlider);
    configLabel(hpCutoffLabel, hpCutoffSlider, "HPF");

    lpCutoffAttachement.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "lpCutoff", lpCutoffSlider));
    lpCutoffSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lpCutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(lpCutoffSlider);
    configLabel(lpCutoffLabel, lpCutoffSlider, "LPF");

    driveAttachement.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "drive", driveSlider));
    driveSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    driveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(driveSlider);
    configLabel(driveLabel, driveSlider, "drive");

    distortionModeAttachement.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(valueTreeState, "distortionMode", distortionModeDropDown));
    distortionModeDropDown.addItemList(juce::StringArray{ "Softclip", "Hardclip", "Wavefolder"}, 1);
    distortionModeDropDown.setSelectedId(1);
    addAndMakeVisible(distortionModeDropDown);
    configLabel(distortionModeLabel, distortionModeDropDown, "mode");

    outputAttachement.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "output", outputSlider));
    outputSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    outputSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(outputSlider);
    configLabel(outputLabel, outputSlider, "out gain");

    mixAttachement.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "mix", mixSlider));
    mixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(mixSlider);
    configLabel(mixLabel, mixSlider, "dry/wet");

    setSize (800, 420);
}

MetallicDelayAudioProcessorEditor::~MetallicDelayAudioProcessorEditor()
{
}

//==============================================================================
void MetallicDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colour::fromString("#FF121212"));
    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
}

void MetallicDelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto area = getLocalBounds();
    juce::Rectangle<int> delayArea = area.removeFromLeft((area.getWidth() / 2) - padding).reduced(padding);
    juce::Rectangle<int> fxArea = area.reduced(padding);
    juce::Rectangle<int> outputArea = area.removeFromRight(100 + padding).reduced(padding);


    delayTimeSlider.setBounds(delayArea.getX(), delayArea.getY(), 100, 120);
    feedbackSlider.setBounds(delayArea.getTopLeft().getX() + 100, delayArea.getTopLeft().getY(), 100, 120);
    modRateSlider.setBounds(delayArea.getTopLeft().getX(), delayArea.getBottomLeft().getY() - 140, 100, 120);
    modDepthSlider.setBounds(delayArea.getTopLeft().getX() + 100, delayArea.getBottomLeft().getY() - 140, 100, 120);
    modTypeDropDown.setBounds(delayArea.getTopLeft().getX() + 220, delayArea.getBottomLeft().getY() - 140, 80, 36);
    lpCutoffSlider.setBounds(fxArea.getTopLeft().getX() + 100, fxArea.getTopLeft().getY(), 100, 120);
    hpCutoffSlider.setBounds(fxArea.getTopLeft().getX(), fxArea.getTopLeft().getY(), 100, 120);

    driveSlider.setBounds(fxArea.getTopLeft().getX(), fxArea.getBottomRight().getY() - 140, 100, 120);
    distortionModeDropDown.setBounds(fxArea.getTopLeft().getX() + 100, fxArea.getBottomRight().getY() - 140, 80, 36);
    outputSlider.setBounds(outputArea.getTopLeft().getX() - padding, outputArea.getCentreY() - 140, 100, 120);
    mixSlider.setBounds(outputArea.getTopLeft().getX() - padding, outputArea.getCentreY() + 20, 100, 120);
}

void MetallicDelayAudioProcessorEditor::configLabel(juce::Label& l, juce::Component& attachedComp, juce::String text)
{
    l.setColour(juce::Label::textColourId, UIColours::textMuted);
    l.setText(text, juce::dontSendNotification);
    l.setJustificationType(juce::Justification::left);
    l.attachToComponent(&attachedComp, false);
}
