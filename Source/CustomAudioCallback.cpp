/*
  ==============================================================================

    CustomAudioCallback.cpp
    Created: 31 Mar 2022 1:56:13pm
    Author:  rapha

  ==============================================================================
*/

#include "CustomAudioCallback.h"

CustomAudioCallback::CustomAudioCallback(std::shared_ptr<IOscillator> _carrier,
    std::shared_ptr<IOscillator> _modulator,
    std::shared_ptr<LinSweepGenerator> _envelope)
    : juce::AudioIODeviceCallback()
{
    carrier = _carrier;
    modulator = _modulator;
    envelope = _envelope;
    currentVolume = db2Mag(MIN_VOL);
    targetVolume = db2Mag(MIN_VOL);
    channel = StereoChannel::LEFT;
}

CustomAudioCallback::~CustomAudioCallback() {}

void CustomAudioCallback::audioDeviceIOCallback(const float** inputChannelData,
    int numInputChannels,
    float** outputChannelData,
    int numOutputChannels,
    int numSamples)
{
    // This is the time critical, real time audio callback function which gets called 
    // periodically on the audio thread.
    // Dont allocate any heap memory here (or do other operations that can take longer than
    // expected). Since it is called from another thread than the GUI, watch out for
    // race conditions and use atomic variables or a message queue if you need to exchange
    // data with other threads.

    // Call envelope->getNextValue() to generate next volume step for sweep.
    // Also note that the volume is converted to db and then back to magnitude
    // to generate a logarithmic sweep.
    targetVolume = db2Mag(envelope->getNextValue(Mag2db(targetVolume)));

    // Volume has changed
    if (targetVolume != currentVolume)
    {
        auto volumeIncrement = (targetVolume - currentVolume) / (double)numSamples;

        for (auto i = 0; i < numSamples; ++i)
        {
            // Sample generation
            double carrierSample = carrier->getNextSample();
            double modSample = modulator->getNextSample();
            outputChannelData[(int)channel][i] = (float)(currentVolume * carrierSample * (modSample + 1.0) / 2.0);

            // Increment the volume for the next sample to reduce clicking
            currentVolume += volumeIncrement;
        }
        currentVolume = targetVolume;
    }
    // Volume stayed the same
    else
    {
        for (auto i = 0; i < numSamples; ++i)
        {
            // Sample generation
            double carrierSample = carrier->getNextSample();
            double modSample = modulator->getNextSample();
            outputChannelData[(int)channel][i] = (float)(currentVolume * carrierSample * (modSample + 1.0) / 2.0);
        }
    }
}

void CustomAudioCallback::audioDeviceAboutToStart(juce::AudioIODevice* device) {}
void CustomAudioCallback::audioDeviceStopped() {}

void CustomAudioCallback::setChannel(StereoChannel newChannel)
{
    channel = newChannel;
}

void CustomAudioCallback::setTargetVolume(double newTargetVolume)
{
    targetVolume = newTargetVolume;
}

double CustomAudioCallback::getCurrentVolume() { return currentVolume; }
