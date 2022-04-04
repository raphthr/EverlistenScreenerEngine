/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>
#include <iostream>
#include "AudioStream.h"

//==============================================================================
int main (int argc, char* argv[])
{
    //juce::ScopedJuceInitialiser_GUI initialiser = juce::ScopedJuceInitialiser_GUI();
    juce::initialiseJuce_GUI();

    AudioStream as;
    as.startTest(StereoChannel::LEFT, 1000.0);
    std::cout << "Hello Sine" << std::endl;
    std::cin.get();

    double testVolume = as.stopTest();
    std::cout << testVolume << std::endl;
    std::cin.get();

    juce::shutdownJuce_GUI();

    return 0;
}
