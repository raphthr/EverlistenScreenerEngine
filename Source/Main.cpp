#include <iostream>
#include "EngineCore.h"

//==============================================================================
int main (int argc, char* argv[])
{
    EngineCore core{};
    std::cout << "Press Enter to start Test" << std::endl;
    std::cin.get();
    core.startTest(StereoChannel::LEFT, 1000.0);
    std::cout << "Started Test, press Enter to stop Test" << std::endl;
    std::cin.get();

    double testVolume = core.stopTest();
    std::cout << "Your hearing level is: " << testVolume << " dBHL" << std::endl;
    std::cout << "Press Enter to terminate" << std::endl;
    std::cin.get();

    return 0;
}

// juce_core
// juce_audio_basics
// juce_events
// juce_audio_devices