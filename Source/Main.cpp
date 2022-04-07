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
    AudioStream as;
    as.startTest(StereoChannel::LEFT, 1000.0);
    std::cout << "Hello Sine" << std::endl;
    std::cin.get();

    double testVolume = as.stopTest();
    std::cout << testVolume << std::endl;
    std::cin.get();

    return 0;
}
