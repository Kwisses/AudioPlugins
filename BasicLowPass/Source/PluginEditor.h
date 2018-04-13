/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "LowPass.h"

//==============================================================================

class BasicLowPassAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    BasicLowPassAudioProcessorEditor (BasicLowPassAudioProcessor&);
    ~BasicLowPassAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    BasicLowPassAudioProcessor& processor;

	LowPass lowPassGui;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicLowPassAudioProcessorEditor)
};
