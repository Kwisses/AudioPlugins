/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define HIGHPASS_ID "highPass"
#define HIGHPASS_NAME "HighPass"
#define HIGHPASS_SUFFIX "Hz"
#define HIGHPASS_CUTOFF 300.0f

//==============================================================================

class BasicHighPassAudioProcessor  : public AudioProcessor
{
public:
    BasicHighPassAudioProcessor();
    ~BasicHighPassAudioProcessor();

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	//==============================================================================

	// Updates the filter dsp module
	void updateFilter();

	// HighPass variables
	float previousHighPass;
	float currentHighPass;
	AudioProcessorValueTreeState tree;

private:
	dsp::ProcessorDuplicator<dsp::StateVariableFilter::Filter<float>, 
							 dsp::StateVariableFilter::Parameters<float>> stateVariableFilter;
	float lastSampleRate;

	//==============================================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicHighPassAudioProcessor)
};