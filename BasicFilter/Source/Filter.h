/*
  ==============================================================================

    Filter.h
    Created: 13 Apr 2018 10:04:21pm
    Author:  Johnathon Kwisses

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "LowPass.h"
#include "BandPass.h"
#include "HighPass.h"

#define FILTERMENU_ID "filterMenu"
#define FILTERMENU_NAME "filterMenu"
#define CUTOFF_SLIDER_NAME "CutoffSlider"
#define CUTOFF_SLIDER_ID "cutoffSlider"
#define CUTOFF_SLIDER_SUFFIX "Hz"
#define CUTOFF_VALUE 20000
#define RESONANCE_SLIDER_NAME "ResonanceSlider"
#define RESONANCE_SLIDER_ID "resonanceSlider"
#define RESONANCE_VALUE 10
#define PADDING 20

//==============================================================================

class Filter    : public Component
{
public:
    Filter(BasicFilterAudioProcessor&);
    ~Filter();

    void paint (Graphics&) override;
    void resized() override;

private:
	BasicFilterAudioProcessor& processor;

	Rectangle<int> titleArea;
	Rectangle<int> border;

	ComboBox filterMenu;
	Slider cutoffSlider;
	Slider resonanceSlider;

	ScopedPointer<AudioProcessorValueTreeState::ComboBoxAttachment> filterMenuValue;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> filterCutoffValue;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> filterResonanceValue;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Filter)
};
