/*
  ==============================================================================

    LinSweepGenerator.h
    Created: 22 Mar 2022 10:06:03am
    Author:  Raphael Thurnher

    This class is for generating a linear sweep between a minimum and maximum
    value. The function getNextValue() should be called once at the beginning
    of each audio callback.

  ==============================================================================
*/

#pragma once

class LinSweepGenerator
{
public:
    //==============================================================================
    LinSweepGenerator(double _fs, int _bufferSize, double _minValue, 
                      double _maxValue, double _sweepTime);
    ~LinSweepGenerator();

    //==============================================================================
    double getNextValue(double currentValue);
    double getMinValue();
    double getMaxValue();
    double getSweepTime();
    double getFs();
    int getBufferSize();
    bool getActive();
    bool getFinished();
    
    
    //==============================================================================
    void startSweep();
    void stopSweep();
    void setMinValue(double newMin);
    void setMaxValue(double newMax);
    void setSweepTime(double newSweepTime);
    void setFs(double newFs);
    void setBufferSize(int newBufferSize);

private:
    //==============================================================================
    double fs;
    int bufferSize;

    double minValue;
    double maxValue;
    double sweepTime;
    double deltaPerBlock;
    bool isActive;
    bool isFinished;

    void updateDelta();
};