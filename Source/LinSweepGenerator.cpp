/*
  ==============================================================================

    LinSweepGenerator.cpp
    Created: 22 Mar 2022 10:06:03am
    Author:  Raphael Thurnher

    This is the implementation of LinSweepGenerator.

  ==============================================================================
*/

#include "LinSweepGenerator.h"

LinSweepGenerator::LinSweepGenerator(double _fs, int _bufferSize, double _minValue,
    double _maxValue, double _sweepTime)
{
    fs = _fs;
    bufferSize = _bufferSize;
    minValue = _minValue;
    maxValue = _maxValue;
    sweepTime = _sweepTime;
    isActive = false;
    isFinished = false;
    updateDelta();
}

LinSweepGenerator::~LinSweepGenerator() {};

//==============================================================================
double LinSweepGenerator::getMinValue() { return minValue; }
double LinSweepGenerator::getMaxValue() { return maxValue; }
double LinSweepGenerator::getSweepTime() { return sweepTime; }
double LinSweepGenerator::getFs() { return fs; }
int LinSweepGenerator::getBufferSize() { return bufferSize; }
bool LinSweepGenerator::getActive() { return isActive; }
bool LinSweepGenerator::getFinished() { return isFinished; }

double LinSweepGenerator::getNextValue(double currentValue)
{
    // Return currentValue back if generator is not active
    if (!isActive)
        return currentValue;
    // add delta for linear increase
    else if (currentValue < maxValue)
        return currentValue + deltaPerBlock;
    // or just maxValue to prevent overflows
    else
    {
        isFinished = true;
        return maxValue;
    }
}

//==============================================================================
void LinSweepGenerator::startSweep()
{
    isActive = true;
    isFinished = false;
}
void LinSweepGenerator::stopSweep()
{
    isActive = false;
}

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

// Call this function if any parameter has changed
// to always get the correct delta.
void LinSweepGenerator::updateDelta()
{
    deltaPerBlock = (maxValue - minValue) / (sweepTime / (bufferSize / fs));
}