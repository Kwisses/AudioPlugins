/*
  ==============================================================================

    HighPass.cpp
    Created: 9 Apr 2018 1:26:48pm
    Author:  Johnathon Kwisses

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "HighPass.h"

//==============================================================================

HighPass::HighPass(BasicHighPassAudioProcessor& p) : processor(p)
{
	// Slider parameters
	highPassSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	highPassSlider.setTextBoxStyle(Slider::TextBoxBelow, false, PADDING * 3, PADDING * 2);
	highPassSlider.setColour(Slider::rotarySliderFillColourId, Colours::red);
	highPassSlider.setName(HIGHPASS_NAME);
	highPassSlider.setTextValueSuffix(HIGHPASS_SUFFIX);
	addAndMakeVisible(&highPassSlider);

	// int parameters must come AFTER addAndMakeVisible and gainValue init
	highPassValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, HIGHPASS_ID, highPassSlider);
	highPassSlider.setRange(0, HIGHPASS_CUTOFF, 1.0f);
}

HighPass::~HighPass()
{
	highPassValue.~ScopedPointer();
}

void HighPass::paint (Graphics& g)
{
	border.setBounds(PADDING, PADDING * 3, getWidth() - PADDING * 2, getHeight() - PADDING * 4);
	titleArea.setBounds(0, 0, getWidth(), PADDING * 2);

	g.setColour(Colours::lightgrey);
	g.setFont(28.0f);

	g.drawText("HighPass Filter", titleArea, Justification::centredBottom, true);
	g.drawRoundedRectangle(border.toFloat(), 25.0f, 8);
}

void HighPass::resized()
{
	border.setBounds(PADDING, PADDING * 3, getWidth() - PADDING * 2, getHeight() - PADDING * 4);
	highPassSlider.setBounds(border.reduced(PADDING));
}
