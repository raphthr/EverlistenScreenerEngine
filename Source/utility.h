/*
  ==============================================================================

    utility.h
    Created: 17 Mar 2022 12:42:00pm
    Author:  Raphael Thurnher

    This file provides utility functions and constants for working with audio data.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define MIN_VOL -96.0   // to avoid denormals
#define MAX_VOL -18.0   // to avoid hearing damage
#define M_PI 3.14159265358979323846

enum class StereoChannel { LEFT = 0, RIGHT = 1 };

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