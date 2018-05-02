/*
  ==============================================================================

    SynthSound.h
    Created: 26 Apr 2018 8:45:10pm
    Author:  Johnathon Kwisses

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class SynthSound : public SynthesiserSound 
{
public:
	bool appliesToNote(int /*midiNoteNumber*/) 
	{
		return true;
	}

	bool appliesToChannel(int /*midiChannel*/)
	{
		return true;
	}
};