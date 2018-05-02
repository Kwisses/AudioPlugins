/*
  ==============================================================================

    Filter.h
    Created: 29 Apr 2018 6:29:15pm
    Author:  Johnathon Kwisses

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#define OSCILLATOR_ID "oscillator"
#define OSCILLATOR_NAME "Oscillator"
#define OSCILLATOR_CUTOFF 300
#define OSCILLATOR_RES 1

//==============================================================================
/*
*/
class Oscillator    : public Component
{
public:
    Oscillator(BasicSynthAudioProcessor&);
    ~Oscillator();

    void paint (Graphics&) override;
    void resized() override;

private:
	BasicSynthAudioProcessor& processor;

	juce::Rectangle<int> titleArea;
	juce::Rectangle<int> border;

	ComboBox oscillatorMenu;

	ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> oscillatorValue;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Oscillator)
};
