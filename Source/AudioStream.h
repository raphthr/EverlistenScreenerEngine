#pragma once

#define _USE_MATH_DEFINES 

#include <JuceHeader.h>
#include <cmath> 
#include <string>
#include "utility.h"
#include "LinSweepGenerator.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class AudioStream : public juce::AudioAppComponent,
                    public juce::Slider::Listener
{
public:
    //==============================================================================
    AudioStream();
    ~AudioStream() override;

    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void updateAngleDelta(juce::Slider& slider, double& angleDelta);

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider) override;
    void updateToggleState(juce::Button* button, juce::String name);

    enum RadioButtonIds { StereoButtons = 1001, StartStopButtons = 1002 };
    enum class StereoChannel { LEFT = 0, RIGHT = 1 };

private:
    //==============================================================================
    // Your private member variables go here...
    double fs;
    int bufferSize;

    double currentVolume;
    double targetVolume;

    double phase;
    double phaseDelta;
    double modPhase;
    double modPhaseDelta;

    LinSweepGenerator envelope;
    StereoChannel channel;

    juce::Slider volSlider;
    juce::Label  volLabel;

    juce::Slider freqSlider;
    juce::Label  freqLabel;

    juce::Slider modSlider;
    juce::Label  modLabel;

    juce::Slider timeSlider;
    juce::Label  timeLabel;

    juce::ToggleButton leftButton{ "Left" },
                       rightButton{ "Right" };

    juce::ToggleButton startButton{ "Start" },
                       stopButton{ "Stop" };

    juce::Label maxVolume;
    juce::Label testResult;
    juce::Label testResultLabel;

  

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioStream)
};
