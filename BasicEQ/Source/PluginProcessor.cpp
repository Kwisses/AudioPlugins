/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#define _USE_MATH_DEFINES
#include <math.h>

//==============================================================================

BasicEqAudioProcessor::BasicEqAudioProcessor()
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
	NormalisableRange<float> frequencyRange(0.1f, 20000.0f);
	NormalisableRange<float> gainRange(0.01, 6.0f);
	NormalisableRange<float> qRange(0.1f, 1.0f);
	tree.createAndAddParameter(FREQUENCY_ID, FREQUENCY_NAME, FREQUENCY_ID, frequencyRange, 0.1f, nullptr, nullptr);
	tree.createAndAddParameter(GAIN_ID, GAIN_NAME, GAIN_ID, gainRange, 1.0f, nullptr, nullptr);
	tree.createAndAddParameter(Q_ID, Q_NAME, Q_ID, qRange, 0.01f, nullptr, nullptr);
	tree.state = ValueTree("savedParams");
}

BasicEqAudioProcessor::~BasicEqAudioProcessor()
{
}

//==============================================================================

const String BasicEqAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BasicEqAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BasicEqAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BasicEqAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BasicEqAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BasicEqAudioProcessor::getNumPrograms()
{
    return 1;
}

int BasicEqAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BasicEqAudioProcessor::setCurrentProgram (int index)
{
}

const String BasicEqAudioProcessor::getProgramName (int index)
{
    return {};
}

void BasicEqAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================

void BasicEqAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	lastSampleRate = sampleRate;
	updateFilter();
}

void BasicEqAudioProcessor::updateFilter()
{
	frequency = *tree.getRawParameterValue(FREQUENCY_ID);
	gain = *tree.getRawParameterValue(GAIN_ID);
	q = *tree.getRawParameterValue(Q_ID);

	// Adjusts for Q value when gain is below 1.0
	if (gain < 1.0f)
	{
		q = 1.01f - q;
	}

	peakFilterLeft.setCoefficients(IIRCoefficients::makePeakFilter(lastSampleRate, frequency, q, gain));
	peakFilterRight.setCoefficients(IIRCoefficients::makePeakFilter(lastSampleRate, frequency, q, gain));
}

void BasicEqAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BasicEqAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void BasicEqAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	updateFilter();
	
	peakFilterLeft.processSamples(buffer.getWritePointer(0), buffer.getNumSamples());
	peakFilterRight.processSamples(buffer.getWritePointer(1), buffer.getNumSamples());
}

//==============================================================================

bool BasicEqAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* BasicEqAudioProcessor::createEditor()
{
    return new BasicEqAudioProcessorEditor (*this);
}

//==============================================================================

void BasicEqAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	// From "The Audio Programmer - JUCE Tutorial 17 - 
	// Saving and Recalling Plugin Info Using XML"
	ScopedPointer<XmlElement> xml(tree.state.createXml());
	copyXmlToBinary(*xml, destData);
}

void BasicEqAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
    return new BasicEqAudioProcessor();
}
