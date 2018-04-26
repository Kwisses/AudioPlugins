/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

BasicReverbAudioProcessorEditor::BasicReverbAudioProcessorEditor (BasicReverbAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), reverbProcessor(p)
{
    setSize (400, 375);
	addAndMakeVisible(&reverbProcessor);
}

BasicReverbAudioProcessorEditor::~BasicReverbAudioProcessorEditor()
{
}

//==============================================================================

void BasicReverbAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll(Colours::black);
}

void BasicReverbAudioProcessorEditor::resized()
{
	reverbProcessor.setBounds(getLocalBounds());
}
