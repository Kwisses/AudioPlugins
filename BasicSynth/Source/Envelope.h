/*
  ==============================================================================

    Envelope.h
    Created: 29 Apr 2018 6:28:56pm
    Author:  Johnathon Kwisses

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#define ATTACK_ID "attack"
#define	ATTACK_NAME "Attack"
#define DECAY_ID "decay"
#define DECAY_NAME "Decay"
#define SUSTAIN_ID "sustain"
#define SUSTAIN_NAME "Sustain"
#define RELEASE_ID "release"
#define RELEASE_NAME "Release"
#define SLIDER_SUFFIX "ms"

//==============================================================================

class Envelope    : public Component
{
public:
    Envelope(BasicSynthAudioProcessor&);
    ~Envelope();

    void paint (Graphics&) override;
    void resized() override;

private:
	BasicSynthAudioProcessor& processor;

	juce::Rectangle<float> titleArea;
	juce::Rectangle<int> border;

	Slider attackSlider;
	Slider decaySlider;
	Slider sustainSlider;
	Slider releaseSlider;

	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> attackValue;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> decayValue;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> sustainValue;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> releaseValue;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Envelope)
};
