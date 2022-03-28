/*
  ==============================================================================

    WaveTableOscillator.h
    Created: 24 Mar 2022 2:24:18pm
    Author:  Raphael Thurnher

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <cmath>


class WaveTableOscillator
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
