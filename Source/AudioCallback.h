/*
  ==============================================================================

    AudioCallback.h
    Created: 31 Mar 2022 1:53:35pm
    Author:  Raphael Thurnher

    This file contains the audio callback class.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "utility.h"
#include "Oscillator.h"
#include "LinSweepGenerator.h"

class AudioCallback : public juce::AudioIODeviceCallback
{
public:
    AudioCallback(std::shared_ptr<IOscillator> _carrier,
        std::shared_ptr<IOscillator> _modulator,
        std::shared_ptr<LinSweepGenerator> _envelope);

    ~AudioCallback();
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
};