/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Gain.h"

//==============================================================================

class BasicGainAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    BasicGainAudioProcessorEditor (BasicGainAudioProcessor&);
    ~BasicGainAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    BasicGainAudioProcessor& processor;
	Gain gainGUI;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicGainAudioProcessorEditor)
};
