//
//  Oscillator.cpp
//  RaspberryJammer
//
//  Created by Ben Wadsworth on 15/08/2016.
//
//

#include <stdio.h>
#include <math.h>

#include "Oscillator.h"

double Oscillator::NaiveWaveformForOscType(OscType type)
{
    double value;
    switch(type) {
        case kOscillatorTypeSine:
            value = sin(phase);
            break;
        case kOscillatorTypeSaw:
            value = (2.0 * phase / TWO_PI) - 1.0;
            break;
        case kOscillatorTypeSquare:
            if (phase < TWO_PI) {
                value = 1.0;
            }
            else {
                value = -1.0;
            }
            break;
        default:
            break;
    }
    return value;
}

void Oscillator::SetFrequency(double frequency)
{
    /*
     wavelength in frames: SAMPLE_RATE / frequency
     we want to increase by 2.0f per cycle
     so 2.0f / wavelength
     */
    this->increment = TWO_PI / (SAMPLE_RATE / frequency);
}
