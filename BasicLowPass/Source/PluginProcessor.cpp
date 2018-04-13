/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

BasicLowPassAudioProcessor::BasicLowPassAudioProcessor()
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
	NormalisableRange<float> lowPassRange(LOWPASS_CUTOFF, 20000);
	tree.createAndAddParameter(LOWPASS_ID, LOWPASS_NAME, LOWPASS_ID, lowPassRange, 20000, nullptr, nullptr);
	tree.state = ValueTree("savedParams");
}

BasicLowPassAudioProcessor::~BasicLowPassAudioProcessor()
{
}

//==============================================================================

const String BasicLowPassAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BasicLowPassAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BasicLowPassAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BasicLowPassAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BasicLowPassAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BasicLowPassAudioProcessor::getNumPrograms()
{
    return 1;
}

int BasicLowPassAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BasicLowPassAudioProcessor::setCurrentProgram (int index)
{
}

const String BasicLowPassAudioProcessor::getProgramName (int index)
{
    return {};
}

void BasicLowPassAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================

void BasicLowPassAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	lastSampleRate = sampleRate;

	// Initialize our DSP object.
	dsp::ProcessSpec spec;
	spec.sampleRate = lastSampleRate;
	spec.numChannels = getMainBusNumOutputChannels();
	spec.maximumBlockSize = samplesPerBlock;

	stateVariableFilter.reset();
	updateFilter();
	stateVariableFilter.prepare(spec);
}

void BasicLowPassAudioProcessor::updateFilter()
{
	int cutoff = *tree.getRawParameterValue(LOWPASS_ID);

	stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
	stateVariableFilter.state->setCutOffFrequency(lastSampleRate, cutoff, 2.0f);
}

void BasicLowPassAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BasicLowPassAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void BasicLowPassAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
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

bool BasicLowPassAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* BasicLowPassAudioProcessor::createEditor()
{
    return new BasicLowPassAudioProcessorEditor (*this);
}

//==============================================================================
void BasicLowPassAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	// From "The Audio Programmer - JUCE Tutorial 17 - 
	// Saving and Recalling Plugin Info Using XML"
	ScopedPointer<XmlElement> xml(tree.state.createXml());
	copyXmlToBinary(*xml, destData);
}

void BasicLowPassAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
    return new BasicLowPassAudioProcessor();
}
