/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "BandPass.h"

//==============================================================================

class BasicBandPassAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    BasicBandPassAudioProcessorEditor (BasicBandPassAudioProcessor&);
    ~BasicBandPassAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    BasicBandPassAudioProcessor& processor;

	BandPass bandPassGui;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicBandPassAudioProcessorEditor)
};
