#pragma once

#define SAMPLE_RATE (44100)
#define TWO_PI 6.2831853

#include "FrameData.h"

enum OscType {
    kOscillatorTypeSine,
    kOscillatorTypeSaw,
    kOscillatorTypeSquare
};

class Oscillator
{
public:
    virtual FrameData NextFrame()=0;
    void SetFrequency(double frequency);
    void SetOscType(OscType type) { this->oscType = type; }
    
protected:
    double phase = 0.0f;
    double increment;
    OscType oscType = kOscillatorTypeSaw;
    
    double NaiveWaveformForOscType(OscType type);
};