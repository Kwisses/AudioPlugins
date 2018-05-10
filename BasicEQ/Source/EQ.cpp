/*
  ==============================================================================

    EQ.cpp
    Created: 2 May 2018 10:30:13am
    Author:  Johnathon Kwisses

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "EQ.h"

//==============================================================================
EQ::EQ(BasicEqAudioProcessor& p) : processor(p)
{
	frequencySlider.setSliderStyle(Slider::SliderStyle::Rotary);
	frequencySlider.setTextBoxStyle(Slider::TextBoxBelow, false, PADDING * 3, PADDING * 2);
	frequencySlider.setColour(Slider::rotarySliderFillColourId, Colours::red);
	frequencySlider.setName(FREQUENCY_NAME);
	frequencySlider.setTextValueSuffix(FREQUENCY_SUFFIX);
	addAndMakeVisible(&frequencySlider);

	gainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	gainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, PADDING * 3, PADDING * 2);
	gainSlider.setColour(Slider::rotarySliderFillColourId, Colours::red);
	gainSlider.setName(GAIN_NAME);
	gainSlider.setTextValueSuffix(GAIN_SUFFIX);
	addAndMakeVisible(&gainSlider);

	qSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	qSlider.setTextBoxStyle(Slider::TextBoxBelow, false, PADDING * 3, PADDING * 2);
	qSlider.setColour(Slider::rotarySliderFillColourId, Colours::red);
	qSlider.setName(Q_NAME);
	qSlider.setTextValueSuffix(Q_SUFFIX);
	addAndMakeVisible(&qSlider);

	frequencyValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, FREQUENCY_ID, frequencySlider);
	gainValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, GAIN_ID, gainSlider);
	qValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, Q_ID, qSlider);

	frequencySlider.setRange(1.0f, 20000.0f, 1.0f);
	gainSlider.setRange(0.01, 6.0f, 0.01f);
	gainSlider.setSkewFactorFromMidPoint(1.0f);
	qSlider.setRange(0.01, 1.0f, 0.01);
}

EQ::~EQ()
{
	frequencyValue.~ScopedPointer();
	gainValue.~ScopedPointer();
	qValue.~ScopedPointer();
}

void EQ::paint (Graphics& g)
{
	titleArea.setBounds(0, 0, getWidth(), PADDING * 3);
	border.setBounds(PADDING, PADDING * 3, getWidth() - PADDING * 2, getHeight() - PADDING * 3 - PADDING / 2.0f);

	g.setFont(28.0f);
	g.setColour(Colours::lightgrey);

	g.drawText("Basic EQ", titleArea, Justification::centred, false);
	g.drawRoundedRectangle(border.toFloat(), 28.0f, 8.0f);

	g.setFont(18.0f);
	g.drawText(FREQUENCY_NAME, frequencySlider.getBounds().removeFromTop(PADDING * 4.5f), Justification::centred, false);
	g.drawText(GAIN_NAME, gainSlider.getBounds().removeFromTop(PADDING * 4.5f), Justification::centred, false);
	g.drawText(Q_NAME, qSlider.getBounds().removeFromTop(PADDING * 4.5f), Justification::centred, false);
}

void EQ::resized()
{
	border.setBounds(PADDING, PADDING * 3, getWidth() - PADDING * 2, getHeight() - PADDING * 3 - PADDING / 2.0f);
	frequencySlider.setBounds(border.removeFromLeft(border.getWidth() / 3.0f).reduced(PADDING));
	gainSlider.setBounds(border.removeFromLeft(border.getWidth() / 2.0f).reduced(PADDING));
	qSlider.setBounds(border.removeFromLeft(border.getWidth()).reduced(PADDING));
}
