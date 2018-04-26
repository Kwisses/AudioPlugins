/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "ReverbProcessor.h"

//==============================================================================

class BasicReverbAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    BasicReverbAudioProcessorEditor (BasicReverbAudioProcessor&);
    ~BasicReverbAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    BasicReverbAudioProcessor& processor;

	ReverbProcessor reverbProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicReverbAudioProcessorEditor)
};
