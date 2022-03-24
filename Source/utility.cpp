/*
  ==============================================================================

    utility.cpp
    Created: 17 Mar 2022 12:42:00pm
    Author:  Raphael Thurnher

  ==============================================================================
*/

#include "utility.h"

double db2Mag(double dbValue) { return pow (10, dbValue / 20); }

float db2Mag(float dbValue) { return pow(10, dbValue / 20); }

double Mag2db(double magnitude)
{
    if (magnitude > 0.f)
        return 20 * log10(magnitude);
    else
        return NAN;
}

float Mag2db(float magnitude)
{
    if (magnitude > 0.f)
        return 20 * log10(magnitude);
    else
        return NAN;
}
