/*
  ==============================================================================

    SineOscillator.cpp
    Created: 24 Mar 2022 10:17:37am
    Author:  rapha

  ==============================================================================
*/

#include "SineOscillator.h"

SineOscillator::SineOscillator(double _fs, double _frequency)
{
    fs = _fs;
    frequency = _frequency;
    phase = 0.0;
    updatePhaseDelta();
}

SineOscillator::SineOscillator() {};
SineOscillator::~SineOscillator() {};

//==============================================================================

double SineOscillator::getFs() { return fs; }
double SineOscillator::getFrequency() { return frequency; }
double SineOscillator::getNextSample() 
{
    double nextSample = sin(phase);
    phase = fmod(phase + phaseDelta, 2 * M_PI);
    return nextSample;
}

void SineOscillator::setFs(double newFs)
{
    fs = newFs;
    updatePhaseDelta();
}
void SineOscillator::setFrequency(double newFrequency) 
{
    frequency = newFrequency;
    updatePhaseDelta();
}

void SineOscillator::updatePhaseDelta()
{
    phaseDelta = 2.0 * M_PI * frequency / fs;
}
