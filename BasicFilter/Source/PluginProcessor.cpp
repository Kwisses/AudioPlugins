/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

BasicFilterAudioProcessor::BasicFilterAudioProcessor()
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
	NormalisableRange<float> cutoffRange(0, 20000);
	NormalisableRange<float> resonanceRange(1.0f, 10.0f);
	NormalisableRange<float> filterMenuRange(0, 2.0f);
	tree.createAndAddParameter(FILTERMENU_ID, FILTERMENU_NAME, FILTERMENU_ID, filterMenuRange, 0, nullptr, nullptr);
	tree.createAndAddParameter(CUTOFF_SLIDER_ID, CUTOFF_SLIDER_NAME, CUTOFF_SLIDER_ID, cutoffRange, 5000, nullptr, nullptr);
	tree.createAndAddParameter(RESONANCE_SLIDER_ID, RESONANCE_SLIDER_NAME, RESONANCE_SLIDER_ID, resonanceRange, 1, nullptr, nullptr);
	tree.state = ValueTree("savedParams");
}

BasicFilterAudioProcessor::~BasicFilterAudioProcessor()
{
}

//==============================================================================

const String BasicFilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BasicFilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BasicFilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BasicFilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BasicFilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BasicFilterAudioProcessor::getNumPrograms()
{
    return 1;
}

int BasicFilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BasicFilterAudioProcessor::setCurrentProgram (int index)
{
}

const String BasicFilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void BasicFilterAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================

void BasicFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
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

void BasicFilterAudioProcessor::updateFilter()
{
	int filterChoice = *tree.getRawParameterValue(FILTERMENU_ID);
	int cutoff = *tree.getRawParameterValue(CUTOFF_SLIDER_ID);
	int resonance = *tree.getRawParameterValue(RESONANCE_SLIDER_ID);

	switch (filterChoice) {
	case 0:
		stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
		break;
	case 1:
		stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::bandPass;
		break;
	case 2:
		stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::highPass;
		break;
	}

	stateVariableFilter.state->setCutOffFrequency(lastSampleRate, cutoff, resonance);
}

void BasicFilterAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BasicFilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void BasicFilterAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	// Create an AudioBlock (acts like buffer, but include the dsp module).
	dsp::AudioBlock<float> block(buffer);
	updateFilter();
	stateVariableFilter.process(dsp::ProcessContextReplacing<float>(block));
}

//==============================================================================

bool BasicFilterAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* BasicFilterAudioProcessor::createEditor()
{
    return new BasicFilterAudioProcessorEditor (*this);
}

//==============================================================================
void BasicFilterAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	// From "The Audio Programmer - JUCE Tutorial 17 - 
	// Saving and Recalling Plugin Info Using XML"
	ScopedPointer<XmlElement> xml(tree.state.createXml());
	copyXmlToBinary(*xml, destData);
}

void BasicFilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
    return new BasicFilterAudioProcessor();
}
