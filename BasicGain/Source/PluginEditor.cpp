/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

BasicGainAudioProcessorEditor::BasicGainAudioProcessorEditor (BasicGainAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), gainGUI(p)
{
    setSize (325, 350);
	addAndMakeVisible(&gainGUI);
}

BasicGainAudioProcessorEditor::~BasicGainAudioProcessorEditor()
{
}

//==============================================================================

void BasicGainAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

//==============================================================================

void BasicGainAudioProcessorEditor::resized()
{
	gainGUI.setBounds(getLocalBounds());
}
