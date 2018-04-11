/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "HighPass.h"

//==============================================================================

class BasicHighPassAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    BasicHighPassAudioProcessorEditor (BasicHighPassAudioProcessor&);
    ~BasicHighPassAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

	HighPass highPassGUI;

private:
    BasicHighPassAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicHighPassAudioProcessorEditor)
};
