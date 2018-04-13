/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

BasicBandPassAudioProcessor::BasicBandPassAudioProcessor()
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
	NormalisableRange<float> bandPassRange(0, BANDPASS_CUTOFF);
	tree.createAndAddParameter(BANDPASS_ID, BANDPASS_NAME, BANDPASS_ID, bandPassRange, 3500.0f, nullptr, nullptr);
	tree.state = ValueTree("savedParams");
}

BasicBandPassAudioProcessor::~BasicBandPassAudioProcessor()
{
}

//==============================================================================

const String BasicBandPassAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BasicBandPassAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BasicBandPassAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BasicBandPassAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BasicBandPassAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BasicBandPassAudioProcessor::getNumPrograms()
{
    return 1;
}

int BasicBandPassAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BasicBandPassAudioProcessor::setCurrentProgram (int index)
{
}

const String BasicBandPassAudioProcessor::getProgramName (int index)
{
    return {};
}

void BasicBandPassAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void BasicBandPassAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	lastSampleRate = sampleRate;

	// Initialize our DSP object.
	dsp::ProcessSpec spec;
	spec.sampleRate = lastSampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = getMainBusNumOutputChannels();

	stateVariableFilter.reset();
	updateFilter();
	stateVariableFilter.prepare(spec);
}

void BasicBandPassAudioProcessor::updateFilter()
{
	int cutoff = *tree.getRawParameterValue(BANDPASS_ID);

	stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::bandPass;
	stateVariableFilter.state->setCutOffFrequency(lastSampleRate, cutoff);
}

void BasicBandPassAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BasicBandPassAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void BasicBandPassAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	// Act as a buffer for the dsp module.
	dsp::AudioBlock<float> block(buffer);
	updateFilter();
	stateVariableFilter.process(dsp::ProcessContextReplacing<float>(block));
}

//==============================================================================
bool BasicBandPassAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* BasicBandPassAudioProcessor::createEditor()
{
    return new BasicBandPassAudioProcessorEditor (*this);
}

//==============================================================================
void BasicBandPassAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	// From "The Audio Programmer - JUCE Tutorial 17 - 
	// Saving and Recalling Plugin Info Using XML"
	ScopedPointer<XmlElement> xml(tree.state.createXml());
	copyXmlToBinary(*xml, destData);
}

void BasicBandPassAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
    return new BasicBandPassAudioProcessor();
}
