/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

BasicCompressorAudioProcessorEditor::BasicCompressorAudioProcessorEditor (BasicCompressorAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), compressor(p)
{
    setSize (400, 300);
	addAndMakeVisible(&compressor);
}

BasicCompressorAudioProcessorEditor::~BasicCompressorAudioProcessorEditor()
{
}

//==============================================================================

void BasicCompressorAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void BasicCompressorAudioProcessorEditor::resized()
{
	compressor.setBounds(getLocalBounds());
}
