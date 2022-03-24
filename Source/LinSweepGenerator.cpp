/*
  ==============================================================================

    LinSweepGenerator.cpp
    Created: 22 Mar 2022 10:06:03am
    Author:  Raphael Thurnher

    This is the implementation of LinSweepGenerator.h

  ==============================================================================
*/

#include "LinSweepGenerator.h"

LinSweepGenerator::LinSweepGenerator(double samplerate, int buffsize, double minvalue,
                                     double maxvalue, double sweeptime)
{
    fs = samplerate;
    bufferSize = buffsize;
    minValue = minvalue;
    maxValue = maxvalue;
    sweepTime = sweeptime;
    isActive = false;
    updateDelta();
}

LinSweepGenerator::LinSweepGenerator() {};
LinSweepGenerator::~LinSweepGenerator() {};

//==============================================================================
double LinSweepGenerator::getMinValue() { return minValue; }
double LinSweepGenerator::getMaxValue() { return maxValue; }
double LinSweepGenerator::getSweepTime() { return sweepTime; }
double LinSweepGenerator::getFs() { return fs; }
int LinSweepGenerator::getBufferSize() { return bufferSize; }
bool LinSweepGenerator::getActive() { return isActive; }

double LinSweepGenerator::getNextValue(double currentValue) 
{   
    // return currentValue back if it is not active
    if (!isActive)
        return currentValue;
    // add delta for linear increase
    else if (currentValue < maxValue)
        return currentValue + deltaPerBlock;
    // or just maxValue to prevent overflows
    else
        return maxValue;
}

double LinSweepGenerator::getNextValue(double currentValue, bool& isFinished)
{
    if (!isActive)
        return currentValue;
    else if (currentValue < maxValue)
        return currentValue + deltaPerBlock;
    else
    {
        isFinished = true;
        return maxValue;
    }
        
}

//==============================================================================
void LinSweepGenerator::startSweep() { isActive = true; }
void LinSweepGenerator::stopSweep() { isActive = false; }

void LinSweepGenerator::setMinValue(double newMin) 
{
    minValue = newMin; 
    updateDelta();
}

void LinSweepGenerator::setMaxValue(double newMax) 
{ 
    maxValue = newMax;
    updateDelta();
}
void LinSweepGenerator::setSweepTime(double newSweepTime) 
{ 
    sweepTime = newSweepTime; 
    updateDelta();
}

void LinSweepGenerator::setFs(double newFs)
{
    fs = newFs;
    updateDelta();
}

void LinSweepGenerator::setBufferSize(int newBufferSize)
{
    fs = newBufferSize;
    updateDelta();
}

void LinSweepGenerator::updateDelta()
{   
    // call this function if any parameter has changed
    // to always get the correct delta
    deltaPerBlock = (maxValue - minValue) / (sweepTime / (bufferSize / fs));
}