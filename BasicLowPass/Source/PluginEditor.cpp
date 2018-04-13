/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

BasicLowPassAudioProcessorEditor::BasicLowPassAudioProcessorEditor (BasicLowPassAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), lowPassGui(p)
{
    setSize (325, 350);
	addAndMakeVisible(&lowPassGui);
}

BasicLowPassAudioProcessorEditor::~BasicLowPassAudioProcessorEditor()
{
}

//==============================================================================

void BasicLowPassAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll(Colours::black);
}

void BasicLowPassAudioProcessorEditor::resized()
{
	lowPassGui.setBounds(getLocalBounds());
}
