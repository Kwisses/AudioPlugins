/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

BasicHighPassAudioProcessorEditor::BasicHighPassAudioProcessorEditor (BasicHighPassAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), highPassGUI(p)
{
    setSize (325, 350);
	addAndMakeVisible(&highPassGUI);
}

BasicHighPassAudioProcessorEditor::~BasicHighPassAudioProcessorEditor()
{
}

//==============================================================================

void BasicHighPassAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void BasicHighPassAudioProcessorEditor::resized()
{
	highPassGUI.setBounds(getLocalBounds());
}
