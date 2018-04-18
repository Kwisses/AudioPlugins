/*
  ==============================================================================

    Filter.cpp
    Created: 13 Apr 2018 10:04:21pm
    Author:  Johnathon Kwisses

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Filter.h"

//==============================================================================

Filter::Filter(BasicFilterAudioProcessor& p) : processor(p)
{
	filterMenu.setJustificationType(Justification::centred);
	filterMenu.addItem("Low Pass", 1);
	filterMenu.addItem("Band Pass", 2);
	filterMenu.addItem("High Pass", 3);
	addAndMakeVisible(&filterMenu);

	cutoffSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	cutoffSlider.setTextBoxStyle(Slider::TextBoxBelow, false, PADDING * 3, PADDING * 2);
	cutoffSlider.setColour(Slider::rotarySliderFillColourId, Colours::red);
	cutoffSlider.setName(CUTOFF_SLIDER_NAME);
	cutoffSlider.setTextValueSuffix(CUTOFF_SLIDER_SUFFIX);
	addAndMakeVisible(&cutoffSlider);

	resonanceSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	resonanceSlider.setTextBoxStyle(Slider::TextBoxBelow, false, PADDING * 3, PADDING * 2);
	resonanceSlider.setColour(Slider::rotarySliderFillColourId, Colours::red);
	resonanceSlider.setName(RESONANCE_SLIDER_NAME);
	addAndMakeVisible(&resonanceSlider);

	filterMenuValue = new AudioProcessorValueTreeState::ComboBoxAttachment(processor.tree, FILTERMENU_ID, filterMenu);
	filterCutoffValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, CUTOFF_SLIDER_ID, cutoffSlider);
	filterResonanceValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, RESONANCE_SLIDER_ID, resonanceSlider);

	// setRange() MUST be after creation of all the AudioProcessorValueTreeState's.
	cutoffSlider.setRange(0, CUTOFF_VALUE, 1.0f);
	resonanceSlider.setRange(1.0f, RESONANCE_VALUE, 1.0f);
}

Filter::~Filter()
{
	filterMenuValue.~ScopedPointer();
	filterCutoffValue.~ScopedPointer();
	filterResonanceValue.~ScopedPointer();
}

void Filter::paint (Graphics& g)
{
	titleArea.setBounds(0, 0, getWidth(), PADDING * 3);
	border.setBounds(PADDING, PADDING * 3, getWidth() - PADDING * 2, getHeight() - PADDING * 3 - PADDING / 2);

	g.setColour(Colours::lightgrey);
	g.setFont(28.0f);

	g.fillAll(Colours::black);
	g.drawText("Basic Filter", titleArea, Justification::centred, true);
	g.drawRoundedRectangle(border.toFloat(), 28.0f, 8.0f);
}

void Filter::resized()
{
	border.setBounds(PADDING, PADDING * 3, getWidth() - PADDING * 2, getHeight() - PADDING * 4);
	filterMenu.setBounds(border.removeFromTop(border.getHeight() / 4).reduced(PADDING));
	cutoffSlider.setBounds(border.removeFromLeft(border.getWidth() / 2).reduced(PADDING / 4));
	resonanceSlider.setBounds(border.removeFromRight(border.getWidth()).reduced(PADDING / 4));
}
