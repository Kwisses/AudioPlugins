/*
  ==============================================================================

    EQ.h
    Created: 2 May 2018 10:30:13am
    Author:  Johnathon Kwisses

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#define FREQUENCY_ID "frequency"
#define FREQUENCY_NAME "Frequency"
#define FREQUENCY_SUFFIX "Hz"
#define GAIN_ID "gain"
#define GAIN_NAME "Gain"
#define GAIN_SUFFIX "dB"
#define Q_ID "q"
#define Q_NAME "Q"
#define Q_SUFFIX "Q"

#define PADDING 20

//==============================================================================

class EQ    : public Component
{
public:
    EQ(BasicEqAudioProcessor&);
    ~EQ();

    void paint (Graphics&) override;
    void resized() override;

private:
	BasicEqAudioProcessor& processor;

	Rectangle<int> titleArea;
	Rectangle<int> border;

	Slider frequencySlider;
	Slider gainSlider;
	Slider qSlider;

	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> frequencyValue;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> gainValue;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> qValue;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQ)
};
