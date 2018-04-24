/*
  ==============================================================================

    Compressor.h
    Created: 17 Apr 2018 8:55:29pm
    Author:  Johnathon Kwisses

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#define THRESHOLD_NAME "Threshold"
#define THRESHOLD_ID "threshold"
#define THRESHOLD_SUFFIX "dB"
#define RATIO_NAME "Ratio"
#define RATIO_ID "ratio"
#define RATIO_SUFFIX " : 1"
#define GAIN_NAME "Gain"
#define GAIN_ID "gain"
#define GAIN_SUFFIX "dB"
#define PADDING 20

//==============================================================================

class Compressor    : public Component
{
public:
    Compressor(BasicCompressorAudioProcessor&);
    ~Compressor();

    void paint (Graphics&) override;
    void resized() override;

private:
	BasicCompressorAudioProcessor& processor;

	Rectangle<int> titleArea;
	Rectangle<int> border;

	Slider thresholdSlider;
	Slider ratioSlider;
	Slider gainSlider;

	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> thresholdValue;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> ratioValue;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> gainValue;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Compressor)
};
