/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

BasicCompressorAudioProcessor::BasicCompressorAudioProcessor()
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
	NormalisableRange<float> thresholdRange(-48.0f, 0.1f); // in dB
	NormalisableRange<float> ratioRange(1.0f, 8.0f); // 8:1
	NormalisableRange<float> gainRange(0, 6.0f); // in dB
	tree.createAndAddParameter(THRESHOLD_ID, THRESHOLD_NAME, THRESHOLD_ID, thresholdRange, -5.0f, nullptr, nullptr);
	tree.createAndAddParameter(RATIO_ID, RATIO_NAME, RATIO_ID, ratioRange, 1.5f, nullptr, nullptr);
	tree.createAndAddParameter(GAIN_ID, GAIN_NAME, GAIN_ID, gainRange, 0, nullptr, nullptr);
	tree.state = ValueTree("savedParams");
}

BasicCompressorAudioProcessor::~BasicCompressorAudioProcessor()
{
}

const String BasicCompressorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BasicCompressorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BasicCompressorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BasicCompressorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BasicCompressorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BasicCompressorAudioProcessor::getNumPrograms()
{
    return 1;
}

int BasicCompressorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BasicCompressorAudioProcessor::setCurrentProgram (int index)
{
}

const String BasicCompressorAudioProcessor::getProgramName (int index)
{
    return {};
}

void BasicCompressorAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================

void BasicCompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	lastSampleRate = sampleRate;

	// Initialize our DSP object.
	dsp::ProcessSpec spec;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = getMainBusNumOutputChannels();
	spec.sampleRate = lastSampleRate;

	stateVariableFilter.reset();
	updateFilter();
	stateVariableFilter.prepare(spec);
}

void BasicCompressorAudioProcessor::updateFilter()
{
	threshold = *tree.getRawParameterValue(THRESHOLD_ID);
	ratio = *tree.getRawParameterValue(RATIO_ID);
	gain = *tree.getRawParameterValue(GAIN_ID);
}

void BasicCompressorAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BasicCompressorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void BasicCompressorAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	updateFilter();

	// ---------------
	// Copied From: https://github.com/sricketts/compressor/blob/master/Source/PluginProcessor.cpp#L153

	for (int channel = 0; channel < totalNumInputChannels; ++channel)
	{
		numSamples = buffer.getNumSamples();
		peak = buffer.getMagnitude(channel, 0, numSamples);
		peakDB = Decibels::gainToDecibels<float>(peak);

		// Great, so now where does attack and release come into play?

		// Peak Sensing
		// attack time is 0 (FIXME)
		// release time is the time it takes to play the buffer (FIXME)
		if (peakDB > threshold) {
			const float compression = 1.0f - (1.0f / ratio); // compression amount
			float gainDB = compression * (threshold - peakDB); // gain between threshold and peak
			gainDB = jmin<float>(0, gainDB); // gain reduction give compression amount
			const float gainConversion = Decibels::decibelsToGain<float>(gainDB + gain); // add gain variable to output
			buffer.applyGain(channel, 0, numSamples, gainConversion); // apply gain to only sample above threshold
		}
	}
	const float outputGain = Decibels::decibelsToGain<float>(1.0f); // apply gain like normal
	buffer.applyGain(outputGain + gain);// add gain variable to output

	// ---------------
}

//==============================================================================

bool BasicCompressorAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* BasicCompressorAudioProcessor::createEditor()
{
    return new BasicCompressorAudioProcessorEditor (*this);
}

//==============================================================================

void BasicCompressorAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	// From "The Audio Programmer - JUCE Tutorial 17 - 
	// Saving and Recalling Plugin Info Using XML"
	ScopedPointer<XmlElement> xml(tree.state.createXml());
	copyXmlToBinary(*xml, destData);
}

void BasicCompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
    return new BasicCompressorAudioProcessor();
}
