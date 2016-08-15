#pragma once

#define SAMPLE_RATE (44100)
#define TWO_PI 6.2831853

#include "FrameData.h"
#include "Oscillator.h"


class Oscillator
{
public:
    virtual FrameData NextFrame()=0;
    virtual void SetFrequency(double frequency)=0;
};