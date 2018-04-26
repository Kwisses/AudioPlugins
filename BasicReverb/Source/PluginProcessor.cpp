/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

BasicReverbAudioProcessor::BasicReverbAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	),
	tree(*this, nullptr)
#endif
{
	NormalisableRange<float> reverbRange(0, 100.0f);
	NormalisableRange<float> dryRange(0, 100.0f);
	tree.createAndAddParameter(REVERB_ID, REVERB_NAME, REVERB_ID, reverbRange, 0, nullptr, nullptr);
	tree.createAndAddParameter(DRY_ID, DRY_NAME, DRY_ID, dryRange, reverbParams.dryLevel, nullptr, nullptr);
	tree.state = ValueTree("savedParams");
}

BasicReverbAudioProcessor::~BasicReverbAudioProcessor()
{
}

//==============================================================================

const String BasicReverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BasicReverbAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BasicReverbAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BasicReverbAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BasicReverbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BasicReverbAudioProcessor::getNumPrograms()
{
    return 1;
}

int BasicReverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BasicReverbAudioProcessor::setCurrentProgram (int index)
{
}

const String BasicReverbAudioProcessor::getProgramName (int index)
{
    return {};
}

void BasicReverbAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================

void BasicReverbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	lastSampleRate = sampleRate;
	updateReverb();
}

void BasicReverbAudioProcessor::updateReverb()
{
	reverb = *tree.getRawParameterValue(REVERB_ID);
	dry = *tree.getRawParameterValue(DRY_ID);
	reverbParams.wetLevel = reverb / 100.0f; // percentage to range.
	reverbParams.dryLevel = dry / 100.0f;
	reverbObj.setParameters(reverbParams);
}

void BasicReverbAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BasicReverbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void BasicReverbAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	updateReverb();
	reverbObj.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());
}

//==============================================================================

bool BasicReverbAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* BasicReverbAudioProcessor::createEditor()
{
    return new BasicReverbAudioProcessorEditor (*this);
}

//==============================================================================
void BasicReverbAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	// From "The Audio Programmer - JUCE Tutorial 17 - 
	// Saving and Recalling Plugin Info Using XML"
	ScopedPointer<XmlElement> xml(tree.state.createXml());
	copyXmlToBinary(*xml, destData);
}

void BasicReverbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	// From "The Audio Programmer - JUCE Tutorial 17 - 
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
    return new BasicReverbAudioProcessor();
}
