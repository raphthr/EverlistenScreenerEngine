/*
  ==============================================================================

    utility.h
    Created: 17 Mar 2022 12:42:00pm
    Author:  Raphael Thurnher

    This file provides utility functions for working with audio data.

  ==============================================================================
*/

#pragma once

#define _USE_MATH_DEFINES 
#include <cmath>
#include <JuceHeader.h>

double db2Mag(double dbValue);
float db2Mag(float dbValue);

double Mag2db(double magnitude);
float Mag2db(float magnitude);

void createSineTable(juce::AudioBuffer<double>& sineTable, int tableSize);
