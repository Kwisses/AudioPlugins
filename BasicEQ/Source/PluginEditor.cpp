/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

BasicEqAudioProcessorEditor::BasicEqAudioProcessorEditor (BasicEqAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), eq(p)
{
    setSize (500, 400);
	addAndMakeVisible(&eq);
}

BasicEqAudioProcessorEditor::~BasicEqAudioProcessorEditor()
{
}

//==============================================================================

void BasicEqAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void BasicEqAudioProcessorEditor::resized()
{
	eq.setBounds(getLocalBounds());
}
