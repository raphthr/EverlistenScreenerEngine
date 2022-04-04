/*
  ==============================================================================

    utility.h
    Created: 17 Mar 2022 12:42:00pm
    Author:  Raphael Thurnher

    This file provides utility functions for working with audio data.

  ==============================================================================
*/

#pragma once

#define MIN_VOL -96.0
#define MAX_VOL -18.0
#define M_PI 3.14159265358979323846
#include <cmath>
#include <JuceHeader.h>

enum class StereoChannel { LEFT = 0, RIGHT = 1 };

inline double db2Mag(double dbValue) { return pow(10, dbValue / 20); }

inline float db2Mag(float dbValue) { return (float)pow(10, dbValue / 20); }

inline double Mag2db(double magnitude)
{
    if (magnitude > 0.f)
        return 20 * log10(magnitude);
    else
        return NAN;
}

inline float Mag2db(float magnitude)
{
    if (magnitude > 0.f)
        return 20 * log10(magnitude);
    else
        return NAN;
}

inline void createSineTable(juce::AudioBuffer<double>& sineTable, int tableSize)
{
    sineTable.setSize(1, tableSize + 1);
    sineTable.clear();
    auto* samples = sineTable.getWritePointer(0);

    auto phaseDelta = 2 * M_PI / (double)(tableSize - 1);
    auto phase = 0.0;

    for (unsigned int i = 0; i < (unsigned int)tableSize; ++i)
    {
        auto sample = sin(phase);
        samples[i] = sample;
        phase += phaseDelta;
    }

    samples[tableSize] = samples[0];
}
