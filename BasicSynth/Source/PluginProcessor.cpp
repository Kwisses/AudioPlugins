/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/


#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

BasicSynthAudioProcessor::BasicSynthAudioProcessor()
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
	NormalisableRange<float> attackRange(0.1f, 3000.0f);
	NormalisableRange<float> decayRange(0.1f, 3000.0f);
	NormalisableRange<float> sustainRange(0.1f, 3000.0f);
	NormalisableRange<float> releaseRange(0.1f, 3000.0f);
	NormalisableRange<float> oscillatorRange(0, 2);
	tree.createAndAddParameter(ATTACK_ID, ATTACK_NAME, ATTACK_ID, attackRange, 0.1f, nullptr, nullptr);
	tree.createAndAddParameter(DECAY_ID, DECAY_NAME, DECAY_ID, decayRange, 0.1f, nullptr, nullptr);
	tree.createAndAddParameter(SUSTAIN_ID, SUSTAIN_NAME, SUSTAIN_ID, sustainRange, 0.1f, nullptr, nullptr);
	tree.createAndAddParameter(RELEASE_ID, RELEASE_NAME, RELEASE_ID, releaseRange, 0.1f, nullptr, nullptr);
	tree.createAndAddParameter(OSCILLATOR_ID, OSCILLATOR_NAME, OSCILLATOR_ID, oscillatorRange, 0, nullptr, nullptr);
	tree.state = ValueTree("savedParams");

	synth.clearVoices();
	for (int i = 0; i < 5; ++i)
	{
		synth.addVoice(new SynthVoice());
	}
	synth.clearSounds();
	synth.addSound(new SynthSound());
}

BasicSynthAudioProcessor::~BasicSynthAudioProcessor()
{
}

//==============================================================================

const String BasicSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BasicSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BasicSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BasicSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BasicSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BasicSynthAudioProcessor::getNumPrograms()
{
    return 1;
}

int BasicSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BasicSynthAudioProcessor::setCurrentProgram (int index)
{
}

const String BasicSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void BasicSynthAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================

void BasicSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	ignoreUnused(samplesPerBlock);
	lastSampleRate = sampleRate;
	synth.setCurrentPlaybackSampleRate(lastSampleRate);
}

void BasicSynthAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BasicSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void BasicSynthAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	ScopedNoDenormals noDenormals;

	for (int i = 0; i < synth.getNumVoices(); i++)
	{
		if ((voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))))
		{
			voice->setADSR(tree.getRawParameterValue(ATTACK_ID),
				tree.getRawParameterValue(DECAY_ID),
				tree.getRawParameterValue(SUSTAIN_ID),
				tree.getRawParameterValue(RELEASE_ID));

			voice->getOscType(tree.getRawParameterValue(OSCILLATOR_ID));
		}
	}

	buffer.clear();
	synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool BasicSynthAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* BasicSynthAudioProcessor::createEditor()
{
    return new BasicSynthAudioProcessorEditor (*this);
}

//==============================================================================
void BasicSynthAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	// From "The Audio Programmer - JUCE Tutorial 17 - 
	// Saving and Recalling Plugin Info Using XML"
	ScopedPointer<XmlElement> xml(tree.state.createXml());
	copyXmlToBinary(*xml, destData);
}

void BasicSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
    return new BasicSynthAudioProcessor();
}
