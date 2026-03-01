/*
  ==============================================================================

    mdLookAndFeel.h
    Created: 23 Feb 2026 12:49:25pm
    Author:  laura

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class MDLookAndFeel : public juce::LookAndFeel_V4 {
public:
    MDLookAndFeel();
    void drawRotarySlider(juce::Graphics& g,
        int x, int y, int width, int height,
        float sliderPosProportional,
        float rotaryStartAngle,
        float rotaryEndAngle,
        juce::Slider&) override;
    void drawComboBox(juce::Graphics& g,
        int width, int height,
        bool isButtonDown,
        int buttonX, int buttonY,
        int buttonW, int buttonH,
        juce::ComboBox& box) override;
    void drawPopupMenuItem(juce::Graphics& g,
        const juce::Rectangle<int>& area,
        bool isSeparator,
        bool isActive,
        bool isHighlighted,
        bool isTicked,
        bool hasSubMenu,
        const juce::String& text,
        const juce::String& shortcutKeyText,
        const juce::Drawable* icon,
        const juce::Colour* textColour) override;

private:
    std::unique_ptr<juce::Drawable> knob;
    std::unique_ptr<juce::Drawable> toggleOn, toggleOff;
    std::unique_ptr<juce::Drawable> comboboxbg, comboboxarrow;
};