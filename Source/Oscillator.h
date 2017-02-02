/*
  ==============================================================================

    Oscillator.h
    Created: 30 Jan 2017 10:25:48pm
    Author:  Daniel

  ==============================================================================
*/

#ifndef OSCILLATOR_H_INCLUDED
#define OSCILLATOR_H_INCLUDED

#include <complex>
#include "../JuceLibraryCode/JuceHeader.h"

namespace MathConstants
{
	const double pi = 3.14159265358979323846;
	const std::complex<double> i(0.0, 1.0);
}

class Oscillator
{
public:

	enum WaveShape { Sine, Square, Triangle, Sawtooth };

	Oscillator();
	Oscillator(double sampleRate, WaveShape waveShape = WaveShape::Sine, double glideTimeInSeconds = 0.05);
	~Oscillator();


	void setSampleRate(double sampleRate);
	void setGlideTime(double glideTimeInSeconds);
	void setFrequency(double freq);
	void setAmplitude(float amplitude);
	void setWaveShape(WaveShape waveShape);

	enum ChannelMode { Mono, Left, Right };

	void fillBuffer(AudioSampleBuffer &buffer, ChannelMode audioMode = ChannelMode::Mono);
	void incrementBuffer(AudioSampleBuffer &buffer, ChannelMode audioMode = ChannelMode::Mono);


private:

	std::complex<double> sineWaveValue;

	LinearSmoothedValue<double> frequency;
	LinearSmoothedValue<float> amplitude;
	double sampleRate;
	double glideTimeInSeconds;

	WaveShape waveShape;
	

};



#endif  // OSCILLATOR_H_INCLUDED
