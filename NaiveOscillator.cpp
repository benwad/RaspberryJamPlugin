//
//  NaiveOscillator.cpp
//  RaspberryJammer
//
//  Created by Ben Wadsworth on 15/08/2016.
//
//

#include <math.h>

#include "NaiveOscillator.hpp"

FrameData NaiveOscillator::NextFrame()
{
    if (this->oscType == kOscillatorTypeSaw) {
        this->currentFrame = this->currentFrame + this->increment;
        if (this->currentFrame.left_phase >= 1.0f) {
            this->currentFrame = this->currentFrame - 2.0f;
        }
    }
    else if (this->oscType == kOscillatorTypeSine) {
        this->phase += this->increment;
        double sineValue = sin(this->phase);
        this->currentFrame = FrameData(sineValue, sineValue);
        if (this->phase > TWO_PI) {
            this->phase -= TWO_PI;
        }
    }
    
    return this->currentFrame;
}

void NaiveOscillator::SetFrequency(double frequency)
{
    /*
     wavelength in frames: SAMPLE_RATE / frequency
     we want to increase by 2.0f per cycle
     so 2.0f / wavelength
     */
    this->increment = TWO_PI / (SAMPLE_RATE / frequency);
}
