/*
  ==============================================================================

    Gain.cpp
    Created: 5 Apr 2018 10:09:25am
    Author:  Johnathon Kwisses

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Gain.h"

//==============================================================================

Gain::Gain(BasicGainAudioProcessor& p) : processor(p), padding(20)
{
	// Slider parameters
	gainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	gainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, padding * 3, padding * 2);
	gainSlider.setColour(Slider::rotarySliderFillColourId, Colours::red);
	gainSlider.setName("Gain");
	gainSlider.setTextValueSuffix("dB");
	addAndMakeVisible(&gainSlider);

	// int parameters must come AFTER addAndMakeVisible and gainValue init
	gainValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "gain", gainSlider);
	gainSlider.setRange(-48.0f, 6.0f, 0.1f);
	gainSlider.setSkewFactorFromMidPoint(0);
}

//==============================================================================

Gain::~Gain()
{
	gainValue.~ScopedPointer();
}

//==============================================================================

void Gain::paint (Graphics& g)
{
	Rectangle<int> border(padding, padding * 3, getWidth() - padding * 2, getHeight() - padding * 4);
	Rectangle<int>titleArea(0, 0, getWidth(), padding * 2);

	g.setColour(Colours::lightgrey);
	g.setFont(28.0f);

    g.drawText ("Gain", titleArea, Justification::centredBottom, true);
	g.drawRoundedRectangle(border.toFloat(), 25.0f, 8);
}

//==============================================================================

void Gain::resized()
{
	Rectangle<int> border(padding, padding * 3, getWidth() - padding * 2, getHeight() - padding * 4);
	gainSlider.setBounds(border.reduced(padding));
}
