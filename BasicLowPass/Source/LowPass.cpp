/*
  ==============================================================================

    LowPass.cpp
    Created: 11 Apr 2018 4:29:16pm
    Author:  Johnathon Kwisses

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "LowPass.h"

//==============================================================================

LowPass::LowPass(BasicLowPassAudioProcessor& p) : processor(p)
{
	lowPassSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	lowPassSlider.setTextBoxStyle(Slider::TextBoxBelow, false, PADDING * 3, PADDING * 2);
	lowPassSlider.setColour(Slider::rotarySliderFillColourId, Colours::red);
	lowPassSlider.setName(LOWPASS_NAME);
	lowPassSlider.setTextValueSuffix(LOWPASS_SUFFIX);
	addAndMakeVisible(&lowPassSlider);

	// int parameters must come AFTER addAndMakeVisible and gainValue init
	lowPassValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, LOWPASS_ID, lowPassSlider);
	lowPassSlider.setRange(LOWPASS_CUTOFF, 20000, 1.0f);
	lowPassSlider.setSkewFactorFromMidPoint(10000.0);
}

LowPass::~LowPass()
{
	lowPassValue.~ScopedPointer();
}

void LowPass::paint (Graphics& g)
{
	titleArea.setBounds(0, 0, getWidth(), PADDING * 3);
	border.setBounds(PADDING, PADDING * 3, getWidth() - PADDING * 2, getHeight() - PADDING * 4);

	g.setFont(28.0f);
	g.setColour(Colours::lightgrey);

	g.drawText("LowPass Filter", titleArea, Justification::centred, true);
	g.drawRoundedRectangle(border.toFloat(), 25.0f, 8.0f);
}

void LowPass::resized()
{
	border.setBounds(PADDING, PADDING * 3, getWidth() - PADDING * 2, getHeight() - PADDING * 4);
	lowPassSlider.setBounds(border.reduced(PADDING));
}
