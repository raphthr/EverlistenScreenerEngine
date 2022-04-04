#pragma once

#include <JuceHeader.h>
#include <string>
#include <memory>
#include <iostream>
#include "utility.h"
#include "LinSweepGenerator.h"
#include "WaveTableOscillator.h"
#include "CustomAudioCallback.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
//==============================================================================



class AudioStream : public juce::Timer
{
public:

    

    AudioStream();
    ~AudioStream();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();
    void timerCallback() override;
    void startTest(StereoChannel channel, double testFrequency);
    double stopTest();

private:

    double fs;
    int bufferSize;

    juce::AudioDeviceManager deviceManager;
    juce::AudioBuffer<double> sineTable;

    std::shared_ptr<IOscillator> carrier;
    std::shared_ptr<IOscillator> modulator;
    std::shared_ptr<LinSweepGenerator> envelope;
    CustomAudioCallback* audioCallback;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioStream)
};

//==============================================================================
//==============================================================================

/*class CustomAudioCallback : public juce::AudioIODeviceCallback
{   
public:
    CustomAudioCallback(std::shared_ptr<IOscillator> _carrier, 
                        std::shared_ptr<IOscillator> _modulator, 
                        std::shared_ptr<LinSweepGenerator> _envelope);

    ~CustomAudioCallback();
    void audioDeviceIOCallback(const float** inputChannelData,
                               int numInputChannels,
                               float** outputChannelData,
                               int numOutputChannels,
                               int numSamples) override;

    void audioDeviceAboutToStart(juce::AudioIODevice* device) override;
    void audioDeviceStopped() override;
    void setChannel(StereoChannel newChannel);
    void setTargetVolume(double newTargetVolume);

    double getCurrentVolume();

private:
    std::shared_ptr<IOscillator> carrier;
    std::shared_ptr<IOscillator> modulator;
    std::shared_ptr<LinSweepGenerator> envelope;

    double currentVolume, targetVolume;
    StereoChannel channel;
};*/

