#pragma once

#include <JuceHeader.h>
#include <string>
#include <memory>
#include "utility.h"
#include "LinSweepGenerator.h"
#include "WaveTableOscillator.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
//==============================================================================

class AudioStream : public juce::AudioAppComponent,
                    public juce::Slider::Listener,
                    public juce::Timer
{
public:
    
    AudioStream();
    ~AudioStream() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void timerCallback() override;
    void paint(juce::Graphics& g) override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider) override;
    void updateToggleState(juce::Button* button, juce::String name);

    enum RadioButtonIds { StereoButtons = 1001, StartStopButtons = 1002 };
    enum class StereoChannel { LEFT = 0, RIGHT = 1 };

private:

    double fs, currentVolume, targetVolume;
    int bufferSize;

    // Store oscillator/sweep objects as pointers,
    // since they dont/cant get initialised @ construction time.

    std::shared_ptr<WaveTableOscillator> carrier;
    std::shared_ptr<WaveTableOscillator> modulator;
    std::shared_ptr<LinSweepGenerator> envelope;
    StereoChannel channel;
    juce::AudioBuffer<double> sineTable;

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

    juce::Label maxVolumeText;
    juce::Label testResult;
    juce::Label testResultLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioStream)
};

