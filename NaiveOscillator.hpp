//
//  NaiveOscillator.hpp
//  RaspberryJammer
//
//  Created by Ben Wadsworth on 15/08/2016.
//
//

#ifndef NaiveOscillator_hpp
#define NaiveOscillator_hpp

#include <stdio.h>

#include "FrameData.h"
#include "Oscillator.h"

enum OscType {
    kOscillatorTypeSine,
    kOscillatorTypeSaw,
    kOscillatorTypeSquare
};

class NaiveOscillator : public Oscillator {
    
private:
    FrameData currentFrame;
    double phase = 0.0f;
    double increment;
    OscType oscType = kOscillatorTypeSaw;
    
public:
    FrameData NextFrame();
    void SetOscType(OscType oscType) { this->oscType = oscType; }
    void SetFrequency(double frequency);
    
};

#endif /* NaiveOscillator_hpp */
