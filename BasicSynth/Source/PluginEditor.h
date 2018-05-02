/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Oscillator.h"
#include "Envelope.h"

#define PADDING 20

//==============================================================================

class BasicSynthAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    BasicSynthAudioProcessorEditor (BasicSynthAudioProcessor&);
    ~BasicSynthAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    BasicSynthAudioProcessor& processor;

	Oscillator oscillator;
	Envelope envelope;

	juce::Rectangle<int> titleArea;
	juce::Rectangle<int> border;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BasicSynthAudioProcessorEditor)
};
