/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

BasicGainAudioProcessor::BasicGainAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                      .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
tree(*this, nullptr)
#endif
{
	NormalisableRange<float> gainParam(-48.0f, 6.0f);
	tree.createAndAddParameter("gain", "Gain", "gain", gainParam, 0, nullptr, nullptr);
	tree.state = ValueTree("savedParams");
}

BasicGainAudioProcessor::~BasicGainAudioProcessor()
{
}

//==============================================================================

const String BasicGainAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

//==============================================================================

bool BasicGainAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

//==============================================================================

bool BasicGainAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

//==============================================================================

bool BasicGainAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

//==============================================================================

double BasicGainAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

//==============================================================================

int BasicGainAudioProcessor::getNumPrograms()
{
    return 1;
}

//==============================================================================

int BasicGainAudioProcessor::getCurrentProgram()
{
    return 0;
}

//==============================================================================

void BasicGainAudioProcessor::setCurrentProgram (int index)
{
}

//==============================================================================

const String BasicGainAudioProcessor::getProgramName (int index)
{
    return {};
}

//==============================================================================

void BasicGainAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================

void BasicGainAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	previousGain = pow(10.0f, *tree.getRawParameterValue("gain") / 20.0f);
}

//==============================================================================

void BasicGainAudioProcessor::releaseResources()
{
}

//==============================================================================

#ifndef JucePlugin_PreferredChannelConfigurations
bool BasicGainAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

//==============================================================================

void BasicGainAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

	// Logarithmic gain curves
	currentGain = pow(10.0f, *tree.getRawParameterValue("gain") / 20.0f);

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	// For gain ramp
	if (currentGain == previousGain)
	{
		buffer.applyGain(currentGain);
	}
	else
	{
		buffer.applyGainRamp(0, buffer.getNumSamples(), previousGain, currentGain);
		previousGain = currentGain;
	}
}

//==============================================================================

bool BasicGainAudioProcessor::hasEditor() const
{
    return true;
}

//==============================================================================

AudioProcessorEditor* BasicGainAudioProcessor::createEditor()
{
    return new BasicGainAudioProcessorEditor (*this);
}

//==============================================================================

void BasicGainAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	// From "The Audio Programmer - JUCE Tutorial 17 - 
	// Saving and Recalling Plugin Info Using XML"
	ScopedPointer<XmlElement> xml(tree.state.createXml());
	copyXmlToBinary(*xml, destData);
}

//==============================================================================

void BasicGainAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	// From "The Audio Programmer - JUCE Tutorial 17 - "
	// Saving and Recalling Plugin Info Using XML"
	ScopedPointer<XmlElement> params(getXmlFromBinary(data, sizeInBytes));
	if (params != nullptr)
	{
		if (params->hasTagName(tree.state.getType()))
		{
			tree.state = ValueTree::fromXml(*params);
		}
	}
}

//==============================================================================

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BasicGainAudioProcessor();
}
