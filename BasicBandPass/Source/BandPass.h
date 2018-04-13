/*
  ==============================================================================

    BandPass.h
    Created: 12 Apr 2018 8:22:22pm
    Author:  Johnathon Kwisses

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#define BANDPASS_ID "bandPass"
#define BANDPASS_NAME "BandPass"
#define BANDPASS_SUFFIX "Hz"
#define BANDPASS_CUTOFF 20000
#define PADDING 20

//==============================================================================

class BandPass    : public Component
{
public:
    BandPass(BasicBandPassAudioProcessor&);
    ~BandPass();

    void paint (Graphics&) override;
    void resized() override;

private:
	BasicBandPassAudioProcessor & processor;

	Slider bandPassSlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> bandPassValue;

	Rectangle<int> titleArea;
	Rectangle<int> border;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BandPass)
};
