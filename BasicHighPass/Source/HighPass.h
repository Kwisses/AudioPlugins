/*
  ==============================================================================

    HighPass.h
    Created: 9 Apr 2018 1:26:48pm
    Author:  Johnathon Kwisses

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#define PADDING 20

//==============================================================================

class HighPass    : public Component
{
public:
    HighPass(BasicHighPassAudioProcessor&);
    ~HighPass();

    void paint (Graphics&) override;
    void resized() override;

private:
	BasicHighPassAudioProcessor& processor;

	Slider highPassSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> highPassValue;

	juce::Rectangle<int> border;
	juce::Rectangle<int> titleArea;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HighPass)
};
