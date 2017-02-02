/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Oscillator.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainContentComponent()
    {
        setSize (800, 600);

        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.

        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.

        // For more details, see the help for AudioProcessor::prepareToPlay()


		OSC.resize(5);

		for (size_t i = 0; i < OSC.size(); i++)
		{
			OSC[i].setSampleRate(sampleRate);
			OSC[i].setWaveShape(Oscillator::WaveShape::Sine);
			OSC[i].setGlideTime(0.05);
			OSC[i].setFrequency((i+1)*220.0);
		}

    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {

		bufferToFill.clearActiveBufferRegion();

		for (size_t i = 0; i < OSC.size(); i++)
		{
			OSC[i].setFrequency((0.1 + i) * freq);
			OSC[i].setAmplitude(amp);
			OSC[i].incrementBuffer(*bufferToFill.buffer);
		}

		

    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (Colours::black);


        // You can add your drawing code here!
    }


	// Mouse handling..
	void mouseDown(const MouseEvent& e) override
	{
		mouseDrag(e);
	}

	void mouseDrag(const MouseEvent& e) override
	{
		freq = pow(10.0, log10(1.2e3) * ((getHeight() - e.y) / static_cast<double> (getHeight())));
		amp = jmin(0.9f, 0.2f * e.position.x / getWidth());
	}

	void mouseUp(const MouseEvent&) override
	{
		amp = 0.0f;
	}

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
    }


private:
    //==============================================================================

    // Your private member variables go here...

	float amp;
	double freq;

	std::vector<Oscillator> OSC;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
