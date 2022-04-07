/*
  ==============================================================================

    AudioStream.cpp
    Created: 30 Mar 2022 3:06:42pm
    Author:  Raphael Thurnher

  ==============================================================================
*/

#include "AudioStream.h"

AudioStream::AudioStream()
{   
    juce::initialiseJuce_GUI();

    deviceManager.initialise(0, 2, nullptr, true);
    auto deviceSetup = deviceManager.getAudioDeviceSetup();
    fs = deviceSetup.sampleRate;
    bufferSize = deviceSetup.bufferSize;

    createSineTable(sineTable, 1024);
    carrier = std::make_shared<WaveTableOscillator>(fs, 1000.0, sineTable);
    modulator = std::make_shared<WaveTableOscillator>(fs, 5.0, sineTable);
    envelope = std::make_shared<LinSweepGenerator>(fs, bufferSize, MIN_VOL,
                                                   MAX_VOL, 5.0);
    audioCallback = new CustomAudioCallback(carrier, modulator, envelope);

    deviceManager.addAudioCallback(audioCallback);
}

AudioStream::~AudioStream()
{
    // This shuts down the audio device and clears the audio source.
    // Also free any heap memory here.
    deviceManager.removeAudioCallback(audioCallback);
    delete audioCallback;
    juce::shutdownJuce_GUI();
}

//==============================================================================
void AudioStream::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // This function is called once (on the audio thread)
    // before audio playback starts.
    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void AudioStream::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {}

void AudioStream::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void AudioStream::isAtMaxVolume()
{
    // This callback is called every 100ms (see constructor) on the message thread
    // to decrease the load on the audio thread and because GUI elements should 
    // not be called on the audio thread.

    if (envelope->getFinished() && envelope->getActive())
    {   
        double testedVolume = stopTest();
        std::cout << "Maximum Volume reached. You are deaf." << std::endl;
        std::cout << "Tested Volume: " << testedVolume << std::endl;
    }
}

void AudioStream::startTest(StereoChannel channel, double testFrequency)
{   
    audioCallback->setChannel(channel);
    carrier->setFrequency(testFrequency);
    audioCallback->setTargetVolume(db2Mag(MIN_VOL));
    envelope->startSweep();
}

double AudioStream::stopTest() 
{
    double testVolume = Mag2db(audioCallback->getCurrentVolume());
    envelope->stopSweep();
    audioCallback->setTargetVolume(db2Mag(MIN_VOL));
    return testVolume;
}



