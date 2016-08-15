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
    double nextFrame = this->NaiveWaveformForOscType(this->oscType);
    this->phase += this->increment;
    if (this->phase >= TWO_PI) {
        this->phase -= TWO_PI;
    }
    
    return FrameData(nextFrame, nextFrame);
}

