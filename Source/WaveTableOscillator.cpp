/*
  ==============================================================================

    WaveTableOscillator.cpp
    Created: 24 Mar 2022 2:24:18pm
    Author:  Raphael Thurnher
    This is the implementation of WaveTableOscillator.

  ==============================================================================
*/

#include "WaveTableOscillator.h"

WaveTableOscillator::WaveTableOscillator(double _fs, double _frequency, const juce::AudioBuffer<double>& _waveTable)
    : waveTable(_waveTable), fs(_fs), frequency(_frequency), tableSize(waveTable.getNumSamples() - 1),
    currentIndex(0.0), tableDelta(0.0)
{
    jassert(waveTable.getNumChannels() == 1);
    updateTableDelta();
}

WaveTableOscillator::~WaveTableOscillator() {};

//==============================================================================

double WaveTableOscillator::getNextSample()
{
    auto index0 = (unsigned int)currentIndex;
    auto index1 = index0 + 1;
    auto frac = currentIndex - (float)index0;

    auto* table = waveTable.getReadPointer(0);
    auto value0 = table[index0];
    auto value1 = table[index1];

    // Linear interpolation to reduce SNR.
    auto currentSample = value0 + frac * (value1 - value0);

    currentIndex = fmod(currentIndex + tableDelta, (double)tableSize);

    return currentSample;
}

double WaveTableOscillator::getFs() { return fs; }
double WaveTableOscillator::getFrequency() { return frequency; }
int WaveTableOscillator::getTableSize() { return tableSize; }

//==============================================================================

void WaveTableOscillator::setFs(double newFs)
{
    fs = newFs;
    updateTableDelta();
}

void WaveTableOscillator::setFrequency(double newFrequency)
{
    frequency = newFrequency;
    updateTableDelta();
}

// Call this function every time fs or frequency is changed.
void WaveTableOscillator::updateTableDelta()
{
    tableDelta = frequency * (double)tableSize / fs;
}

