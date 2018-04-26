/*
  ==============================================================================

    Reverb.h
    Created: 24 Apr 2018 7:59:43pm
    Author:  Johnathon Kwisses

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#define REVERB_ID "reverb"
#define REVERB_NAME "Reverb"
#define REVERB_SUFFIX "%"
#define DRY_ID "dry"
#define DRY_NAME "Dry"
#define DRY_SUFFIX "%"
#define PADDING 20

//==============================================================================
/*
*/
class ReverbProcessor : public Component
{
public:
	ReverbProcessor(BasicReverbAudioProcessor&);
    ~ReverbProcessor();

    void paint (Graphics&) override;
    void resized() override;

private:
	BasicReverbAudioProcessor& processor;

	Rectangle<int> titleArea;
	Rectangle<int> border;

	Slider reverbSlider;
	Slider drySlider;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> reverbValue;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> dryValue;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbProcessor)
};
