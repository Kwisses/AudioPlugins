/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Compressor.h"

//==============================================================================

class BasicCompressorAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    BasicCompressorAudioProcessorEditor (BasicCompressorAudioProcessor&);
    ~BasicCompressorAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    BasicCompressorAudioProcessor& processor;

	Compressor compressor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicCompressorAudioProcessorEditor)
};
