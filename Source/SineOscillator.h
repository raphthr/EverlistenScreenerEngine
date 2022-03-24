/*
  ==============================================================================

    SineOscillator.h
    Created: 24 Mar 2022 10:17:37am
    Author:  Raphael Thurnher

  ==============================================================================
*/

#pragma once
#define _USE_MATH_DEFINES 
#include <cmath>

class SineOscillator
{
public:
    //==============================================================================
    SineOscillator(double _fs, double _frequency);
    SineOscillator();
    ~SineOscillator();

    double getFs();
    double getFrequency();
    double getNextSample();

    void setFs(double newFs);
    void setFrequency(double newFrequency);
    

private:
    //==============================================================================
    void updatePhaseDelta();

    double fs;
    double frequency;
    double phase;
    double phaseDelta;
};
