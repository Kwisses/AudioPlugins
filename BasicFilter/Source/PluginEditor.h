/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Filter.h"

//==============================================================================

class BasicFilterAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    BasicFilterAudioProcessorEditor (BasicFilterAudioProcessor&);
    ~BasicFilterAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    BasicFilterAudioProcessor& processor;

	Filter filter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicFilterAudioProcessorEditor)
};
