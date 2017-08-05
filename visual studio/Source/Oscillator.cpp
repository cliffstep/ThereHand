/*
  ==============================================================================

    Oscillator.cpp
    Created: 30 Jan 2017 10:25:48pm
    Author:  Daniel

  ==============================================================================
*/

#include "Oscillator.h"


Oscillator::Oscillator()
	: amplitude(1.0f), sineWaveValue(1.0, 0.0), sampleRate(44100.0), glideTimeInSeconds(0.05), waveShape(WaveShape::Sine)
{
	this->frequency.reset(this->sampleRate, this->glideTimeInSeconds);
}

Oscillator::Oscillator(double sampleRate, WaveShape waveShape, double glideTimeInSeconds)
	: amplitude(1.0f), sineWaveValue(1.0, 0.0)
{
	this->sampleRate = sampleRate;
	this->glideTimeInSeconds = glideTimeInSeconds;
	this->frequency.reset(sampleRate, glideTimeInSeconds);
	this->waveShape = waveShape;
}

Oscillator::~Oscillator()
{
}

void Oscillator::setSampleRate(double sampleRate)
{
	this->sampleRate = sampleRate;
	this->frequency.reset(sampleRate, glideTimeInSeconds);
	this->amplitude.reset(sampleRate, glideTimeInSeconds);
}

void Oscillator::setGlideTime(double glideTimeInSeconds)
{
	this->frequency.reset(sampleRate, glideTimeInSeconds);
	this->amplitude.reset(sampleRate, glideTimeInSeconds);
}

void Oscillator::setFrequency(double freq)
{
	Oscillator::frequency.setValue(2.0 * mathConstants::pi * freq / sampleRate);
}

void Oscillator::setAmplitude(float amplitude)
{
	Oscillator::amplitude.setValue(amplitude);
}

void Oscillator::setWaveShape(WaveShape waveShape)
{
	Oscillator::waveShape = waveShape;
}

void Oscillator::fillBuffer(AudioSampleBuffer & buffer, ChannelMode audioMode)
{
	int channelNumber = 0;
	
	if (audioMode == ChannelMode::Right) channelNumber = 1;
	float *x = buffer.getWritePointer(channelNumber);

	for (size_t i = 0; i < buffer.getNumSamples(); i++)
	{
		double newFreq = Oscillator::frequency.getNextValue();
		float newAmplitude = Oscillator::amplitude.getNextValue();
		float outputWave;

		sineWaveValue *= std::exp(mathConstants::i * 2.0 * mathConstants::pi * newFreq);
		
		outputWave = static_cast<float>(sineWaveValue.real());

		if (Oscillator::waveShape == WaveShape::Square)
		{
			outputWave = (sineWaveValue.real() >= 0.0) ? 1.0f : -1.0f;
		}

		x[i] = newAmplitude * outputWave;

	}


	if (audioMode == ChannelMode::Mono)
	{
		for (size_t i = 1; i < buffer.getNumChannels(); i++)
		{
			buffer.copyFrom(i, 0, buffer.getReadPointer(0), buffer.getNumSamples());
		}
	}

}


void Oscillator::incrementBuffer(AudioSampleBuffer & buffer, ChannelMode audioMode)
{
	int channelNumber = 0;

	if (audioMode == ChannelMode::Right) channelNumber = 1;
	float *x = buffer.getWritePointer(channelNumber);

	for (size_t i = 0; i < buffer.getNumSamples(); i++)
	{
		double newFreq = Oscillator::frequency.getNextValue();
		float newAmplitude = Oscillator::amplitude.getNextValue();
		float outputWave;

		sineWaveValue *= std::exp(mathConstants::i * 2.0 * mathConstants::pi * newFreq);

		outputWave = static_cast<float>(sineWaveValue.real());

		if (Oscillator::waveShape == WaveShape::Square)
		{
			outputWave = (sineWaveValue.real() >= 0.0) ? 1.0f : -1.0f;
		}

		x[i] += newAmplitude * outputWave;

	}


	if (audioMode == ChannelMode::Mono)
	{
		for (size_t i = 1; i < buffer.getNumChannels(); i++)
		{
			buffer.copyFrom(i, 0, buffer.getReadPointer(0), buffer.getNumSamples());
		}
	}

}


