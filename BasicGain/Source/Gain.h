/*
  ==============================================================================

    Gain.h
    Created: 5 Apr 2018 10:09:25am
    Author:  Johnathon Kwisses

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================

class Gain    : public Component
{
public:
    Gain(BasicGainAudioProcessor&);
    ~Gain();

    void paint (Graphics&) override;
    void resized() override;

private:
	BasicGainAudioProcessor & processor;

	Slider gainSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> gainValue;

	Rectangle<int> border;
	Rectangle<int> titleArea;

	const int padding;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Gain)
};