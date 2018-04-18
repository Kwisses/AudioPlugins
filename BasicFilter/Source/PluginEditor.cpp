/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

BasicFilterAudioProcessorEditor::BasicFilterAudioProcessorEditor (BasicFilterAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), filter(p)
{
    setSize (450, 400);
	addAndMakeVisible(&filter);
}

BasicFilterAudioProcessorEditor::~BasicFilterAudioProcessorEditor()
{
}

//==============================================================================

void BasicFilterAudioProcessorEditor::paint (Graphics& g)
{
	g.fillAll(Colours::black);
}

void BasicFilterAudioProcessorEditor::resized()
{
	filter.setBounds(getLocalBounds());
}
