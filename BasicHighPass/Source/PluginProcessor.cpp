/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

BasicHighPassAudioProcessor::BasicHighPassAudioProcessor()
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
	NormalisableRange<float> highPassParam(0, HIGHPASS_CUTOFF);
	tree.createAndAddParameter(HIGHPASS_ID, HIGHPASS_NAME, HIGHPASS_ID, highPassParam, 0, nullptr, nullptr);
	tree.state = ValueTree("savedParams");
}

BasicHighPassAudioProcessor::~BasicHighPassAudioProcessor()
{
}

//==============================================================================

const String BasicHighPassAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BasicHighPassAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BasicHighPassAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BasicHighPassAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BasicHighPassAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BasicHighPassAudioProcessor::getNumPrograms()
{
    return 1;
}

int BasicHighPassAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BasicHighPassAudioProcessor::setCurrentProgram (int index)
{
}

const String BasicHighPassAudioProcessor::getProgramName (int index)
{
    return {};
}

void BasicHighPassAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================

void BasicHighPassAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	lastSampleRate = sampleRate;

	// Initialize our dsp object.
	dsp::ProcessSpec spec;
	spec.sampleRate = lastSampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = getMainBusNumOutputChannels();

	stateVariableFilter.reset();
	updateFilter();
	stateVariableFilter.prepare(spec);
}

void BasicHighPassAudioProcessor::releaseResources()
{

}

void BasicHighPassAudioProcessor::updateFilter()
{
	int cutoff = *tree.getRawParameterValue(HIGHPASS_ID);

	stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::highPass;
	stateVariableFilter.state->setCutOffFrequency(lastSampleRate, cutoff, 1.0f);
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BasicHighPassAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void BasicHighPassAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
	
	// Acts as buffer, but for dsp module.
	dsp::AudioBlock<float> block(buffer);
	updateFilter();
	stateVariableFilter.process(dsp::ProcessContextReplacing<float>(block));

}

//==============================================================================

bool BasicHighPassAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* BasicHighPassAudioProcessor::createEditor()
{
    return new BasicHighPassAudioProcessorEditor (*this);
}

//==============================================================================

void BasicHighPassAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	// From "The Audio Programmer - JUCE Tutorial 17 - 
	// Saving and Recalling Plugin Info Using XML"
	ScopedPointer<XmlElement> xml(tree.state.createXml());
	copyXmlToBinary(*xml, destData);
}

void BasicHighPassAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
    return new BasicHighPassAudioProcessor();
}
