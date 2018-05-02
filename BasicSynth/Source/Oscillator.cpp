/*
  ==============================================================================

    Filter.cpp
    Created: 29 Apr 2018 6:29:15pm
    Author:  Johnathon Kwisses

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Oscillator.h"

//==============================================================================

Oscillator::Oscillator(BasicSynthAudioProcessor& p) : processor(p)
{
	oscillatorMenu.setJustificationType(Justification::centred);
	oscillatorMenu.addItem("Sine Wave", 1);
	oscillatorMenu.addItem("Saw Wave", 2);
	oscillatorMenu.addItem("Square Wave", 3);
	oscillatorMenu.setSelectedId(1);
	addAndMakeVisible(&oscillatorMenu);

	oscillatorValue = new AudioProcessorValueTreeState::ComboBoxAttachment(processor.tree, OSCILLATOR_ID, oscillatorMenu);
}

Oscillator::~Oscillator()
{
	oscillatorValue.~ScopedPointer();
}

void Oscillator::paint (Graphics& g)
{
}

void Oscillator::resized()
{
	border.setBounds(PADDING, PADDING * 3, getWidth() - PADDING * 2, getHeight() / 3.0f);
	oscillatorMenu.setBounds(border);
}
