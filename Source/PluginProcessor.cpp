/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MetallicDelayAudioProcessor::MetallicDelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
    , params(*this, nullptr, juce::Identifier("apvts"), {
            std::make_unique<juce::AudioParameterFloat>("delayTime", "DelayTime", juce::NormalisableRange<float>(0.1f, 20.f, 0.1f), 10.0f),
            std::make_unique<juce::AudioParameterFloat>("feedback", "Feedback", juce::NormalisableRange<float>(0.0f, 1.0f, 0.1f), 0.5f),
            std::make_unique<juce::AudioParameterFloat>("modRate", "ModRate", juce::NormalisableRange<float>(0.1f, 20.0f, 0.1f), 1.f),
            std::make_unique<juce::AudioParameterFloat>("modDepth", "ModDepth", juce::NormalisableRange<float>(0.0f, 1.0f, 0.1f), 0.0f),
            std::make_unique<juce::AudioParameterChoice>("modType", "ModType",juce::StringArray { "Sine", "Square", "Triangle", "S&H Noise"}, 0),
            std::make_unique<juce::AudioParameterFloat>("hpCutoff", "Highpass Cutoff", juce::NormalisableRange<float>(20.f, 5000.f, 1.f), 20.f),
            std::make_unique<juce::AudioParameterFloat>("lpCutoff", "Lowpass Cutoff", juce::NormalisableRange<float>(1000.f, 20000.0f, 1.0f), 20000.f),
            std::make_unique<juce::AudioParameterFloat>("drive", "Drive", juce::NormalisableRange<float>(0.0f, 1.0f, 0.1f), 0.0f),
            std::make_unique<juce::AudioParameterChoice>("distortionMode", "DistortionMode",juce::StringArray { "Softclip", "Hardclip", "Wavefolder"}, 0),
            std::make_unique<juce::AudioParameterFloat>("output", "Output", juce::NormalisableRange<float>(-24.0f, 24.0f, 0.01f), 0.0f),
            std::make_unique<juce::AudioParameterFloat>("mix", "Mix", juce::NormalisableRange<float>(0.0f, 1.0f, 0.1f), 0.5f)
        })
{
    delayTimeParam = params.getRawParameterValue("delayTime");
    feedbackParam = params.getRawParameterValue("feedback");
    modRateParam = params.getRawParameterValue("modRate");
    modDepthParam = params.getRawParameterValue("modDepth");
    modTypeParam = params.getRawParameterValue("modType");
    hpCutoffParam = params.getRawParameterValue("hpCutoff");
    lpCutoffParam = params.getRawParameterValue("lpCutoff");
    driveParam = params.getRawParameterValue("drive");
    distortionModeParam = params.getRawParameterValue("distortionMode");
    outputParam = params.getRawParameterValue("output");
    mixParam = params.getRawParameterValue("mix");
}

MetallicDelayAudioProcessor::~MetallicDelayAudioProcessor()
{
}

//==============================================================================
const juce::String MetallicDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MetallicDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MetallicDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MetallicDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MetallicDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MetallicDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MetallicDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MetallicDelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MetallicDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void MetallicDelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MetallicDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{    
    lfo.setSampleRate(sampleRate);
    lfo.setPhase(0.f);

    previousOutput = outputParam->load();
    gainLinear = juce::Decibels::decibelsToGain(previousOutput);

    feedbackFilter.prepare(sampleRate, samplesPerBlock, getTotalNumOutputChannels());

    const int maxDelaySamples = (int)(sampleRate);

    combDelays.resize(getTotalNumInputChannels());

    for (auto& delay : combDelays) {
        delay.prepare(maxDelaySamples, sampleRate);
        delay.reset();
    }
}

void MetallicDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MetallicDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif 
}
#endif

void MetallicDelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    updateParameters(totalNumInputChannels, buffer.getNumSamples());

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            float in = channelData[i];
            float delayed = combDelays[channel].read();

            float distorted = distortion.processSample(delayed);

            float lPassed = feedbackFilter.lowpass.processSample(distorted);
            float hpassed = feedbackFilter.highpass.processSample(lPassed);
            float wet = hpassed * combDelays[channel].getFeedback();

            float out = in + wet;

            combDelays[channel].write(out);
            channelData[i] = (in * dryGain + wet * wetGain) * gainLinear;
        }
    }
}

//==============================================================================
bool MetallicDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MetallicDelayAudioProcessor::createEditor()
{
    return new MetallicDelayAudioProcessorEditor (*this, params);
}

//==============================================================================
void MetallicDelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MetallicDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void MetallicDelayAudioProcessor::setDryWetMix()
{
    dryGain = std::cos(previousMix * juce::MathConstants<float>::halfPi);
    wetGain = std::sin(previousMix * juce::MathConstants<float>::halfPi);
}

void MetallicDelayAudioProcessor::updateParameters(int numChannels, int numSamples)
{
    float delayTime = delayTimeParam->load();
    float feedback = feedbackParam->load();
    float modRate = modRateParam->load();
    float modDepth = modDepthParam->load();
    float modType = modTypeParam->load();
    float hpCutoff = hpCutoffParam->load();
    float lpCutoff = lpCutoffParam->load();
    float drive = driveParam->load();
    float distortionMode = distortionModeParam->load();
    float output = outputParam->load();
    float mix = mixParam->load();

    bool shouldBeEnabled = !juce::approximatelyEqual(modDepth, 0.f);

    if (lfo.getWaveType() != static_cast<int>(modType)) {
        lfo.setWaveType(static_cast<LFO::WaveType>(modType));
    }

    if (lfo.isEnabled != shouldBeEnabled) {
        lfo.enableLFO(shouldBeEnabled);
    }

    if (lfo.isEnabled) {
        lfo.updateLFOState(numSamples);
        float minValue = delayTime * (1 - modDepth);
        float modDelayTime = std::max(float(lfo.getCurrentValue() * modDepth * delayTime + minValue), 0.1f);
        delayTime = modDelayTime;
    }

    if (!juce::approximatelyEqual(modRate, lfo.freq)) {
        lfo.setFreq(modRate);
    }

    if (!juce::approximatelyEqual(drive, distortion.getAmount())) {
        distortion.setAmount(drive);
    }

    if (!juce::approximatelyEqual(static_cast<Distortion::Mode>(distortionMode), distortion.getMode())) {
        distortion.setMode(static_cast<Distortion::Mode>(distortionMode));
    }

    if (!juce::approximatelyEqual(hpCutoff, feedbackFilter.getHighCutoff())) {
        feedbackFilter.updateHighpass(hpCutoff);
    }

    if (!juce::approximatelyEqual(lpCutoff, feedbackFilter.getLowCutoff())) {
        feedbackFilter.updateLowpass(lpCutoff);
    }
    if (!juce::approximatelyEqual(output, previousOutput)) {
        previousOutput = output;
        gainLinear = juce::Decibels::decibelsToGain(output);
    }
    if (!juce::approximatelyEqual(mix, previousMix)) {
        previousMix = mix;
        setDryWetMix();
    }

    for (int channel = 0; channel < numChannels; ++channel) {
        if (!juce::approximatelyEqual(delayTime, combDelays[channel].getDelayMs())) {
            combDelays[channel].setDelayMs(delayTime);
        }
        if (!juce::approximatelyEqual(feedback, combDelays[channel].getFeedback())) {
            combDelays[channel].setFeedback(feedback);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MetallicDelayAudioProcessor();
}
