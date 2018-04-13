/*
  ==============================================================================

    BandPass.cpp
    Created: 12 Apr 2018 8:22:22pm
    Author:  Johnathon Kwisses

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "BandPass.h"

//==============================================================================

BandPass::BandPass(BasicBandPassAudioProcessor& p) : processor(p)
{
	bandPassSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	bandPassSlider.setTextBoxStyle(Slider::TextBoxBelow, false, PADDING * 3, PADDING * 2);
	bandPassSlider.setColour(Slider::rotarySliderFillColourId, Colours::red);
	bandPassSlider.setName(BANDPASS_NAME);
	bandPassSlider.setTextValueSuffix(BANDPASS_SUFFIX);
	addAndMakeVisible(&bandPassSlider);

	// int parameters must come AFTER addAndMakeVisible and gainValue init
	bandPassValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, BANDPASS_ID, bandPassSlider);
	bandPassSlider.setRange(0, BANDPASS_CUTOFF, 1.0f);
}

BandPass::~BandPass()
{
	bandPassValue.~ScopedPointer();
}

void BandPass::paint (Graphics& g)
{
	titleArea.setBounds(0, 0, getWidth(), PADDING * 3);
	border.setBounds(PADDING, PADDING * 3, getWidth() - PADDING * 2, getHeight() - PADDING * 4);

	g.setFont(28.0f);
	g.setColour(Colours::lightgrey);

	g.drawText("Band Pass Filter", titleArea, Justification::centred, true);
	g.drawRoundedRectangle(border.toFloat(), 25.0f, 8.0f);
}

void BandPass::resized()
{
	border.setBounds(PADDING, PADDING * 3, getWidth() - PADDING * 2, getHeight() - PADDING * 4);
	bandPassSlider.setBounds(border.reduced(PADDING));
}
