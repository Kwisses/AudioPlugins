/*
  ==============================================================================

    SynthVoice.h
    Created: 26 Apr 2018 8:45:18pm
    Author:  Johnathon Kwisses

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#include "Maximilian.h"

class SynthVoice : public SynthesiserVoice
{
public:
	bool canPlaySound(SynthesiserSound* sound) override
	{
		return dynamic_cast<SynthSound*>(sound) != nullptr;
	}

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override
	{
		env.trigger = 1; // 0 or 1
		frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		level = velocity;
	}

	void stopNote(float velocity, bool allowTailOff) override
	{
		env.trigger = 0;
		allowTailOff = true;

		if (velocity == 0)
		{
			clearCurrentNote();
		}
	}

	void pitchWheelMoved(int newPitchWheelValue) override
	{
	}

	void controllerMoved(int controllerNumber, int newControllerValue) override
	{
	}

	void getOscType(float* selection)
	{
		oscType = *selection;
	}

	double setOscillator()
	{
		switch (oscType)
		{
		case 0:
			return osc.sinewave(frequency);
		case 1:
			return osc.saw(frequency);
		case 2:
			return osc.square(frequency);
		default:
			return osc.sinewave(frequency);
		}
		
	}

	void setADSR(float* attack, float* decay, float* sustain, float* release)
	{
		env.setAttack(*attack);
		env.setDecay(*decay);
		env.setSustain(*sustain);
		env.setRelease(*release);
	}

	void renderNextBlock(AudioBuffer<float> &outputBuffer, int startSample, int numSamples) override
	{
		for (int sample = 0; sample < numSamples; ++sample)
		{
			for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
			{
				sound = env.adsr(setOscillator(), env.trigger);
				outputBuffer.addSample(channel, startSample, sound * level);
			}
			++startSample;
		}
	}

private:
	double level;
	double frequency;
	double wave;
	double sound;
	int oscType;

	maxiOsc osc;
	maxiEnv env;
};