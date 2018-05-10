/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "EQ.h"

//==============================================================================

class BasicEqAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    BasicEqAudioProcessorEditor (BasicEqAudioProcessor&);
    ~BasicEqAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    BasicEqAudioProcessor& processor;

	EQ eq;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicEqAudioProcessorEditor)
};
