/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

//==============================================================================

BasicSynthAudioProcessorEditor::BasicSynthAudioProcessorEditor (BasicSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), oscillator(p), envelope(p)
{
    setSize (500, 400);
	addAndMakeVisible(&oscillator);
	addAndMakeVisible(&envelope);
}

BasicSynthAudioProcessorEditor::~BasicSynthAudioProcessorEditor()
{
}

//==============================================================================

void BasicSynthAudioProcessorEditor::paint (Graphics& g)
{
	titleArea.setBounds(0, 0, getWidth(), PADDING * 3);
	border.setBounds(PADDING, PADDING * 3, getWidth() - PADDING * 2, getHeight() - PADDING * 3 - PADDING / 2.0f);
	g.setFont(28.0f);
	g.setColour(Colours::lightgrey);

	g.drawText("Basic Synth", titleArea, Justification::centred, false);
	g.drawRoundedRectangle(border.toFloat(), 28.0f, 8.0f);
}

void BasicSynthAudioProcessorEditor::resized()
{
	border.setBounds(PADDING, PADDING, getWidth() - PADDING * 2, getHeight() - PADDING - (PADDING / 2.0f));
	oscillator.setBounds(border.removeFromTop(getHeight() / 4.0f));
	envelope.setBounds(border.removeFromBottom(getHeight() - (getHeight() / 4.0f)));
}
