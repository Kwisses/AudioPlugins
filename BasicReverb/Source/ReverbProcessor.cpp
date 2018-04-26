/*
  ==============================================================================

    Reverb.cpp
    Created: 24 Apr 2018 7:59:43pm
    Author:  Johnathon Kwisses

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ReverbProcessor.h"

//==============================================================================

ReverbProcessor::ReverbProcessor(BasicReverbAudioProcessor& p) : processor(p)
{
	reverbSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	reverbSlider.setTextBoxStyle(Slider::TextBoxBelow, false, PADDING * 3, PADDING * 2);
	reverbSlider.setColour(Slider::rotarySliderFillColourId, Colours::red);
	reverbSlider.setTextValueSuffix(REVERB_SUFFIX);
	reverbSlider.setName(REVERB_NAME);
	addAndMakeVisible(&reverbSlider);

	drySlider.setSliderStyle(Slider::SliderStyle::Rotary);
	drySlider.setTextBoxStyle(Slider::TextBoxBelow, false, PADDING * 3, PADDING * 2);
	drySlider.setColour(Slider::rotarySliderFillColourId, Colours::red);
	drySlider.setTextValueSuffix(DRY_SUFFIX);
	drySlider.setName(DRY_NAME);
	addAndMakeVisible(&drySlider);

	reverbValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, REVERB_ID, reverbSlider);
	dryValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, DRY_ID, drySlider);
	reverbSlider.setRange(0, 100.0f, 1.0f);
	drySlider.setRange(0, 100.0f, 1.0f);
}

ReverbProcessor::~ReverbProcessor()
{
	reverbValue.~ScopedPointer();
}

void ReverbProcessor::paint (Graphics& g)
{
	titleArea.setBounds(0, 0, getWidth(), PADDING * 3);
	border.setBounds(PADDING, PADDING * 3, getWidth() - PADDING * 2, getHeight() - PADDING * 3 - PADDING / 2);

	g.setColour(Colours::lightgrey);
	g.setFont(28.0f);
	g.drawText("Basic Reverb", titleArea, Justification::centred, false);

	g.setFont(24.0f);
	g.drawText("Wet", reverbSlider.getBounds().removeFromTop(PADDING * 2.5f), Justification::centred, false);
	g.drawText("Dry", drySlider.getBounds().removeFromTop(PADDING * 2.5f), Justification::centred, false);
	g.drawRoundedRectangle(border.toFloat(), 28.0f, 8.0f);
}

void ReverbProcessor::resized()
{
	border.setBounds(PADDING, PADDING * 3, getWidth() - PADDING * 2, getHeight() - PADDING * 3 - PADDING / 2);
	reverbSlider.setBounds(border.removeFromLeft(border.getWidth() / 2.0f).reduced(PADDING / 2.0f));
	drySlider.setBounds(border.removeFromLeft(border.getWidth()).reduced(PADDING / 2.0f));
}
