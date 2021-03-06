/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GabunityDemoAudioProcessor::GabunityDemoAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                       ),
       vts(*this, nullptr)
{
    vts.createAndAddParameter ("cutoff",                        // parameter ID
                               "Cutoff (Pole Location)",        // parameter name
                               String(),                        // parameter label (suffix)
                               NormalisableRange<float> (0.0f, 0.999f),    // range
                               0.5f,                            // default value
                               nullptr,
                               nullptr);

    vts.state = ValueTree (Identifier (JucePlugin_Name));
}

GabunityDemoAudioProcessor::~GabunityDemoAudioProcessor()
{
}

//==============================================================================
const String GabunityDemoAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GabunityDemoAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GabunityDemoAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GabunityDemoAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GabunityDemoAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GabunityDemoAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GabunityDemoAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GabunityDemoAudioProcessor::setCurrentProgram (int index)
{
}

const String GabunityDemoAudioProcessor::getProgramName (int index)
{
    return {};
}

void GabunityDemoAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void GabunityDemoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    lastOuts.fill(0.f);
}

void GabunityDemoAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GabunityDemoAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void GabunityDemoAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    const float p = *vts.getRawParameterValue("cutoff");
    const int len = buffer.getNumSamples();

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < jmin(2, totalNumInputChannels); ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int i = 0; i < len; ++i)
        {
            channelData[i] = p * (lastOuts[channel] - channelData[i]) + channelData[i];
            lastOuts[channel] = channelData[i];
        }
    }
}

//==============================================================================
bool GabunityDemoAudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* GabunityDemoAudioProcessor::createEditor()
{
    return nullptr;
}

//==============================================================================
void GabunityDemoAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GabunityDemoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GabunityDemoAudioProcessor();
}
