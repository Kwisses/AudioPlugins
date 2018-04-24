/*
  ==============================================================================

    Compressor.cpp
    Created: 17 Apr 2018 8:55:30pm
    Author:  Johnathon Kwisses

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Compressor.h"

//==============================================================================

Compressor::Compressor(BasicCompressorAudioProcessor& p) : processor(p)
{
	thresholdSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	thresholdSlider.setTextBoxStyle(Slider::TextBoxBelow, false, PADDING * 3, PADDING * 2);
	thresholdSlider.setColour(Slider::rotarySliderFillColourId, Colours::red);
	thresholdSlider.setName(THRESHOLD_NAME);
	thresholdSlider.setTextValueSuffix(THRESHOLD_SUFFIX);
	addAndMakeVisible(&thresholdSlider);

	ratioSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	ratioSlider.setTextBoxStyle(Slider::TextBoxBelow, false, PADDING * 3, PADDING * 2);
	ratioSlider.setColour(Slider::rotarySliderFillColourId, Colours::red);
	ratioSlider.setName(RATIO_NAME);
	ratioSlider.setTextValueSuffix(RATIO_SUFFIX);
	addAndMakeVisible(&ratioSlider);

	gainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	gainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, PADDING * 3, PADDING * 2);
	gainSlider.setColour(Slider::rotarySliderFillColourId, Colours::red);
	gainSlider.setName(GAIN_NAME);
	gainSlider.setTextValueSuffix(GAIN_SUFFIX);
	addAndMakeVisible(&gainSlider);

	thresholdValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, THRESHOLD_ID, thresholdSlider);
	gainValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, GAIN_ID, gainSlider);
	ratioValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, RATIO_ID, ratioSlider);

	thresholdSlider.setRange(-48.0f, 0.1f, 0.1f);
	ratioSlider.setRange(1.0, 5.0, 0.1f);
	gainSlider.setRange(0, 6.0f, 0.1f);
}

Compressor::~Compressor()
{
	thresholdValue.~ScopedPointer();
	ratioValue.~ScopedPointer();
	gainValue.~ScopedPointer();
}

void Compressor::paint (Graphics& g)
{
	titleArea.setBounds(0, 0, getWidth(), PADDING * 3);
	border.setBounds(PADDING, PADDING * 3, getWidth() - PADDING * 2, getHeight() - PADDING * 3 - PADDING / 2);

	g.setFont(28.0f);
	g.setColour(Colours::lightgrey);
	g.drawText("Basic Compressor", titleArea, Justification::centred, true);

	g.setFont(18.0f);
	g.drawText(THRESHOLD_NAME, thresholdSlider.getBounds().removeFromTop(PADDING * 2.5f), Justification::centred, false);
	g.drawText(RATIO_NAME, ratioSlider.getBounds().removeFromTop(PADDING * 2.5f), Justification::centred, false);
	g.drawText(GAIN_NAME, gainSlider.getBounds().removeFromTop(PADDING * 2.5f), Justification::centred, false);
	g.drawRoundedRectangle(border.toFloat(), 28.0f, 8.0f);
}

void Compressor::resized()
{
	border.setBounds(PADDING, PADDING * 3, getWidth() - PADDING * 2, getHeight() - PADDING * 3 - PADDING / 2);
	thresholdSlider.setBounds(border.removeFromLeft(border.getWidth() / 3).reduced(PADDING / 2));
	ratioSlider.setBounds(border.removeFromLeft(border.getWidth() / 2).reduced(PADDING / 2));
	gainSlider.setBounds(border.removeFromLeft(border.getWidth()).reduced(PADDING / 2));
}
