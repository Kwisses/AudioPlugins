/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

BasicBandPassAudioProcessorEditor::BasicBandPassAudioProcessorEditor (BasicBandPassAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), bandPassGui(p)
{
    setSize (325, 350);
	addAndMakeVisible(&bandPassGui);
}

BasicBandPassAudioProcessorEditor::~BasicBandPassAudioProcessorEditor()
{
}

void BasicBandPassAudioProcessorEditor::paint (Graphics& g)
{
	g.fillAll(Colours::black);
}

void BasicBandPassAudioProcessorEditor::resized()
{
	bandPassGui.setBounds(getLocalBounds());
}
