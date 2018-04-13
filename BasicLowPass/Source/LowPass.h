/*
  ==============================================================================

    LowPass.h
    Created: 11 Apr 2018 4:29:16pm
    Author:  Johnathon Kwisses

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#define LOWPASS_ID "lowpass"
#define LOWPASS_NAME "LowPass"
#define LOWPASS_SUFFIX "Hz"
#define LOWPASS_CUTOFF 5000.0f
#define PADDING 20

//==============================================================================

class LowPass    : public Component
{
public:
    LowPass(BasicLowPassAudioProcessor&);
    ~LowPass();

    void paint (Graphics&) override;
    void resized() override;

private:
	BasicLowPassAudioProcessor& processor;

	Slider lowPassSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> lowPassValue;

	Rectangle<int> titleArea;
	Rectangle<int> border;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LowPass)
};
