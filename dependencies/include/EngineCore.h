/*
  ==============================================================================

    EngineCore.h
    Created: 31 Mar 2022 1:56:13pm
    Author:  Raphael Thurnher

    This is the header file for the EngineCore class. It contains everything
    you need for generating and outputting an audio signal.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <memory>
#include <iostream>
#include "utility.h"
#include "LinSweepGenerator.h"
#include "Oscillator.h"
#include "AudioCallback.h"

class EngineCore : public juce::Timer
{
public:

    EngineCore();
    ~EngineCore();

    // void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    // void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    // void releaseResources();
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

    // this has to be a regular pointer since i didnt manage to convert
    // a shared pointer to the right type for the AudioDeviceManager
    AudioCallback* audioCallback;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EngineCore)
};