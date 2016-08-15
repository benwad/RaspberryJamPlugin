//
//  PolyBLEPOscillator.cpp
//  RaspberryJammer
//
//  Created by Ben Wadsworth on 15/08/2016.
//
//

#include "PolyBLEPOscillator.hpp"

double PolyBLEPOscillator::PolyBlep(double t)
{
    double dt = this->increment / TWO_PI;
    
    // 0 <= t < 1
    if (t < dt) {
        t /= dt;
        return t+t - t*t - 1.0;
    }
    
    // -1 < t < 0
    else if (t > 1.0 - dt) {
        t = (t - 1.0) / dt;
        return t*t + t+t + 1.0;
    }
    
    // 0 otherwise
    else return 0;
}

FrameData PolyBLEPOscillator::NextFrame()
{
    double value = 0.0;
    double t = phase / TWO_PI;
    
    if (this->oscType == kOscillatorTypeSine) {
        value = this->NaiveWaveformForOscType(kOscillatorTypeSine);
    }
    else if (this->oscType == kOscillatorTypeSaw) {
        value = this->NaiveWaveformForOscType(kOscillatorTypeSaw);
        value -= this->PolyBlep(t);
    }
    
    this->phase += this->increment;
    if (this->phase >= TWO_PI) {
        this->phase -= TWO_PI;
    }
    
    return FrameData(value, value);
}