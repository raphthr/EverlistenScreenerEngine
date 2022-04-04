/*
  ==============================================================================

    CustomAudioCallback.h
    Created: 31 Mar 2022 1:53:35pm
    Author:  rapha

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "utility.h"
#include "WaveTableOscillator.h"
#include "LinSweepGenerator.h"

class CustomAudioCallback : public juce::AudioIODeviceCallback
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
};