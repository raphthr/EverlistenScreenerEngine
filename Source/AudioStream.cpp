#include "AudioStream.h"

AudioStream::AudioStream()
{   
    // Initialize GUI elemements
    addAndMakeVisible(volSlider);
    volSlider.setRange(-96.0, -3.0, 3.0);
    volSlider.setValue(-18.0, juce::dontSendNotification);
    volSlider.setTextValueSuffix(" dB FS");
    volSlider.setNumDecimalPlacesToDisplay(1);
    volSlider.addListener(this);
    volSlider.setSkewFactorFromMidPoint(-20);

    addAndMakeVisible(volLabel);
    volLabel.setText("MaxVolume", juce::dontSendNotification);
    volLabel.attachToComponent(&volSlider, true);

    addAndMakeVisible(freqSlider);
    freqSlider.setRange(125, 8000, 125);
    freqSlider.setValue(1000, juce::dontSendNotification);
    freqSlider.setTextValueSuffix(" Hz");
    freqSlider.setNumDecimalPlacesToDisplay(1);
    freqSlider.addListener(this);

    addAndMakeVisible(freqLabel);
    freqLabel.setText("Frequency", juce::dontSendNotification);
    freqLabel.attachToComponent(&freqSlider, true);

    addAndMakeVisible(modSlider);
    modSlider.setRange(0.5, 30, 0.5);
    modSlider.setValue(5, juce::dontSendNotification);
    modSlider.setTextValueSuffix(" Hz");
    modSlider.setNumDecimalPlacesToDisplay(1);
    modSlider.addListener(this);
    modSlider.setSkewFactorFromMidPoint(10);

    addAndMakeVisible(modLabel);
    modLabel.setText("ModFrequency", juce::dontSendNotification);
    modLabel.attachToComponent(&modSlider, true);

    addAndMakeVisible(timeSlider);
    timeSlider.setRange(5, 60, 5);
    timeSlider.setValue(25, juce::dontSendNotification);
    timeSlider.setTextValueSuffix(" s");
    timeSlider.setNumDecimalPlacesToDisplay(1);
    timeSlider.addListener(this);

    addAndMakeVisible(timeLabel);
    timeLabel.setText("Test Time", juce::dontSendNotification);
    timeLabel.attachToComponent(&timeSlider, true);

    addAndMakeVisible(leftButton);
    addAndMakeVisible(rightButton);
    leftButton.onClick = [this] { updateToggleState(&leftButton, "Left"); };
    rightButton.onClick = [this] { updateToggleState(&rightButton, "Right"); };
    leftButton.setToggleState(true, juce::dontSendNotification);
    leftButton.setRadioGroupId(StereoButtons);
    rightButton.setRadioGroupId(StereoButtons);

    addAndMakeVisible(startButton);
    addAndMakeVisible(stopButton);
    startButton.onClick = [this] { updateToggleState(&startButton, "Start"); };
    stopButton.onClick = [this] { updateToggleState(&stopButton, "Stop"); };
    stopButton.setToggleState(true, juce::dontSendNotification);
    startButton.setRadioGroupId(StartStopButtons);
    stopButton.setRadioGroupId(StartStopButtons);

    addAndMakeVisible(maxVolumeText);
    maxVolumeText.setText("Maximum volume reached!", juce::dontSendNotification);
    maxVolumeText.setEditable(false);
    maxVolumeText.setVisible(false);

    addAndMakeVisible(testResultLabel);
    testResultLabel.setText("Your hearing Level in dB FS: ", juce::dontSendNotification);
    testResultLabel.attachToComponent(&testResult, true);

    addAndMakeVisible(testResult);
    testResult.setText(" ", juce::dontSendNotification);

    // initialize wavetable
    createSineTable(sineTable, 1024);
    
    // Set initial window size
    setSize(800, 600);

    // Set 2 audio channels (stereo)
    setAudioChannels(0, 2);

    // Start timer for timer callback
    startTimer(100);
}

AudioStream::~AudioStream()
{
    // This shuts down the audio device and clears the audio source.
    // Also free any heap memory here.
    shutdownAudio();
}

//==============================================================================
void AudioStream::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // This function is called once (on the audio thread)
    // before audio playback starts.
    // For more details, see the help for AudioProcessor::prepareToPlay()

    fs = sampleRate;
    bufferSize = samplesPerBlockExpected;

    currentVolume = db2Mag(volSlider.getMinimum());
    targetVolume = db2Mag(volSlider.getMinimum());
    channel = leftButton.getToggleState() ? StereoChannel::LEFT : StereoChannel::RIGHT;

    carrier = std::make_shared<WaveTableOscillator>(sampleRate, freqSlider.getValue(), sineTable);
    modulator = std::make_shared<WaveTableOscillator>(sampleRate, modSlider.getValue(), sineTable);
    envelope = std::make_shared<LinSweepGenerator>(sampleRate, samplesPerBlockExpected, volSlider.getMinimum(),
                                 volSlider.getValue(), timeSlider.getValue());
}

void AudioStream::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{   
    // This is the time critical, real time audio callback function which gets called 
    // periodically on the audio thread.
    // Dont allocate any heap memory here (or do other operations that can take longer than
    // expected). Since it is called from another thread than the GUI, watch out for
    // race conditions and use atomic variables or a message queue if you need to exchange
    // data with other threads.

    bufferToFill.clearActiveBufferRegion();
    auto* channelData = bufferToFill.buffer->getWritePointer((int) channel, bufferToFill.startSample);
    double carrierSample = 0.0;
    double modSample = 0.0;

    // Call envelope->getNextValue() to generate next volume step for sweep.
    // Also note that the volume is converted to db and then back to magnitude
    // to generate a logarithmic sweep.
    targetVolume = db2Mag(envelope->getNextValue(Mag2db(targetVolume)));

    // Volume has changed
    if (targetVolume != currentVolume)
    {   
        auto volumeIncrement = (targetVolume - currentVolume) / bufferToFill.numSamples;

        for (auto i = 0; i < bufferToFill.numSamples; ++i)
        {
            // Sample generation
            carrierSample = carrier->getNextSample();
            modSample = modulator->getNextSample();
            channelData[i] = (float) (currentVolume * carrierSample * (modSample + 1.0) / 2.0);

            // Increment the volume for the next sample to reduce clicking
            currentVolume += volumeIncrement;
        }
        currentVolume = targetVolume;
    }
    // Volume stayed the same
    else
    {
        for (auto i = 0; i < bufferToFill.numSamples; ++i)
        {
            // Sample generation
            carrierSample = carrier->getNextSample();
            modSample = modulator->getNextSample();
            channelData[i] = (float) (currentVolume * carrierSample * (modSample + 1.0) / 2.0);
        }
    }
}

void AudioStream::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void AudioStream::timerCallback()
{   
    // This callback is called every 100ms (see constructor) on the message thread
    // to decrease the load on the audio thread and because GUI elements should 
    // not be called on the audio thread.

    if (envelope->getFinished() && envelope->getActive())
    {   
        maxVolumeText.setVisible(true);
        stopButton.triggerClick();
    }  
}

void AudioStream::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    // You can add your drawing code here!

    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("Everlisten Screener Engine v0.1", getLocalBounds(), juce::Justification::centred, true);
}

void AudioStream::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    auto sliderLeft = 120;
    volSlider.setBounds(sliderLeft, 20, getWidth() - sliderLeft - 10, 20);
    freqSlider.setBounds(sliderLeft, 50, getWidth() - sliderLeft - 10, 20);
    modSlider.setBounds(sliderLeft, 80, getWidth() - sliderLeft - 10, 20);
    timeSlider.setBounds(sliderLeft, 110, getWidth() - 30, 20);
    leftButton.setBounds(sliderLeft, 140, getWidth() - 30, 20);
    rightButton.setBounds(sliderLeft + 60, 140, getWidth() - 30, 20);
    startButton.setBounds(sliderLeft, 170, getWidth() - 30, 20);
    stopButton.setBounds(sliderLeft + 60, 170, getWidth() - 30, 20);
    maxVolumeText.setBounds(sliderLeft + 120, 170, getWidth() - 30, 20);
    testResult.setBounds(sliderLeft + 120, 200, getWidth() - 30, 20);
}

void AudioStream::sliderValueChanged(juce::Slider* slider)
{   
    // This function is called every time a slider is moved.
    // Maybe check if fs > 0.0 to see if app is currently running.

    if (slider == &volSlider)
        envelope->setMaxValue(volSlider.getValue());
    else if (slider == &freqSlider)
        carrier->setFrequency(slider->getValue());
    else if (slider == &modSlider)
        modulator->setFrequency(slider->getValue());
    else if (slider == &timeSlider)
        envelope->setSweepTime(timeSlider.getValue());
}

void AudioStream::updateToggleState(juce::Button* button, juce::String name)
{   
    // This function is called every time a button is pressed.
    // You also need to check the toggle state of the button, 
    // not only the press event.

    if ((button == &leftButton) || (button == &rightButton))
        channel = leftButton.getToggleState() ? StereoChannel::LEFT : StereoChannel::RIGHT;

    else if ((button == &startButton) && (startButton.getToggleState()))
    {   
        testResult.setText(" ", juce::dontSendNotification);
        maxVolumeText.setVisible(false);
        targetVolume = db2Mag(volSlider.getMinimum());
        envelope->startSweep();
    }
    else if ((button == &stopButton) && (stopButton.getToggleState()))
    {   
        testResult.setText(std::to_string(Mag2db(currentVolume)), juce::dontSendNotification);
        targetVolume = db2Mag(volSlider.getMinimum());
        envelope->stopSweep();
    } 
}