/*
  ==============================================================================

    WaveTableOscillator.h
    Created: 24 Mar 2022 2:24:18pm
    Author:  Raphael Thurnher

    This class is a simple wavetable oscillator. You can play any waveform
    by providing the according wavetable. Call getNextSample() in your audio
    callback function.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <cmath>

class IOscillator
{
public:

    virtual double getNextSample() = 0;
    virtual double getFs() = 0;
    virtual double getFrequency() = 0;

    virtual void setFs(double newFs) = 0;
    virtual void setFrequency(double newFrequency) = 0; 
};

//==============================================================================
//==============================================================================

class WaveTableOscillator : public IOscillator
{
public:

    WaveTableOscillator(double _fs, double _frequency, const juce::AudioBuffer<double>& _waveTable);
    ~WaveTableOscillator();

    double getNextSample();
    double getFs();
    double getFrequency();
    int getTableSize();

    void setFs(double newFs);
    void setFrequency(double newFrequency);

private:

    void updateTableDelta();

    const juce::AudioBuffer<double>& waveTable;

    double fs, frequency, currentIndex, tableDelta;
    const int tableSize;
};
