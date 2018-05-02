/*
  ==============================================================================

    Envelope.cpp
    Created: 29 Apr 2018 6:28:56pm
    Author:  Johnathon Kwisses

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Envelope.h"

//==============================================================================
Envelope::Envelope(BasicSynthAudioProcessor& p) : processor(p)
{
	attackSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	attackSlider.setTextBoxStyle(Slider::TextBoxBelow, false, PADDING * 3, PADDING * 2);
	attackSlider.setColour(Slider::rotarySliderFillColourId, Colours::red);
	attackSlider.setName(ATTACK_NAME);
	attackSlider.setTextValueSuffix(SLIDER_SUFFIX);
	addAndMakeVisible(&attackSlider);

	decaySlider.setSliderStyle(Slider::SliderStyle::Rotary);
	decaySlider.setTextBoxStyle(Slider::TextBoxBelow, false, PADDING * 3, PADDING * 2);
	decaySlider.setColour(Slider::rotarySliderFillColourId, Colours::red);
	decaySlider.setName(DECAY_NAME);
	decaySlider.setTextValueSuffix(SLIDER_SUFFIX);
	addAndMakeVisible(&decaySlider);

	sustainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	sustainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, PADDING * 3, PADDING * 2);
	sustainSlider.setColour(Slider::rotarySliderFillColourId, Colours::red);
	sustainSlider.setName(SUSTAIN_NAME);
	sustainSlider.setTextValueSuffix(SLIDER_SUFFIX);
	addAndMakeVisible(&sustainSlider);

	releaseSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	releaseSlider.setTextBoxStyle(Slider::TextBoxBelow, false, PADDING * 3, PADDING * 2);
	releaseSlider.setColour(Slider::rotarySliderFillColourId, Colours::red);
	releaseSlider.setName(RELEASE_NAME);
	releaseSlider.setTextValueSuffix(SLIDER_SUFFIX);
	addAndMakeVisible(&releaseSlider);

	attackValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, ATTACK_ID, attackSlider);
	decayValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, DECAY_ID, decaySlider);
	sustainValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, SUSTAIN_ID, sustainSlider);
	releaseValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, RELEASE_ID, releaseSlider);

	attackSlider.setRange(0.1f, 3000.0f, 1.0f);
	decaySlider.setRange(0.1f, 3000.0f, 1.0f);
	sustainSlider.setRange(0.1f, 3000.0f, 1.0f);
	releaseSlider.setRange(0.1f, 3000.0f, 1.0f);
}

Envelope::~Envelope()
{
	attackValue.~ScopedPointer();
	decayValue.~ScopedPointer();
	sustainValue.~ScopedPointer();
	releaseValue.~ScopedPointer();
}

void Envelope::paint (Graphics& g)
{
	border.setBounds(PADDING, PADDING * 4, getWidth() - PADDING * 2, getHeight() - PADDING * 3 - PADDING / 2.0f);

	g.setColour(Colours::lightgrey);
	g.setFont(18.0f);

	g.drawText(ATTACK_NAME, attackSlider.getBounds(), Justification::centredTop, false);
	g.drawText(DECAY_NAME, decaySlider.getBounds(), Justification::centredTop, false);
	g.drawText(SUSTAIN_NAME, sustainSlider.getBounds(), Justification::centredTop, false);
	g.drawText(RELEASE_NAME, releaseSlider.getBounds(), Justification::centredTop, false);
}

void Envelope::resized()
{
	border.setBounds(PADDING, PADDING * 3, getWidth() - PADDING * 2, getHeight() - PADDING * 4);
	attackSlider.setBounds(border.removeFromLeft(border.getWidth() / 4.0f).reduced(PADDING / 4.0f));
	decaySlider.setBounds(border.removeFromLeft(border.getWidth() / 3.0f).reduced(PADDING / 4.0f));
	sustainSlider.setBounds(border.removeFromLeft(border.getWidth() / 2.0f).reduced(PADDING / 4.0f));
	releaseSlider.setBounds(border.removeFromLeft(border.getWidth()).reduced(PADDING / 4.0f));
}
