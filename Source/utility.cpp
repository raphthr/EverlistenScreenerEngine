/*
  ==============================================================================

    utility.cpp
    Created: 17 Mar 2022 12:42:00pm
    Author:  Raphael Thurnher

  ==============================================================================
*/

#include "utility.h"

double db2Mag(double dbValue) { return pow (10, dbValue / 20); }

float db2Mag(float dbValue) { return (float) pow(10, dbValue / 20); }

double Mag2db(double magnitude)
{
    if (magnitude > 0.f)
        return 20 * log10(magnitude);
    else
        return NAN;
}

float Mag2db(float magnitude)
{
    if (magnitude > 0.f)
        return 20 * log10(magnitude);
    else
        return NAN;
}

void createSineTable(juce::AudioBuffer<double>& sineTable, int tableSize)
{
    sineTable.setSize(1, tableSize + 1);
    sineTable.clear();
    auto* samples = sineTable.getWritePointer(0);

    auto phaseDelta = 2 * M_PI / (double)(tableSize - 1);
    auto phase = 0.0;

    for (unsigned int i = 0; i < (unsigned int) tableSize; ++i)
    {
        auto sample = sin(phase);
        samples[i] = sample;
        phase += phaseDelta;
    }

    samples[tableSize] = samples[0];
}
