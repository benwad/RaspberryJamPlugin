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


class NaiveOscillator : public Oscillator {
    
private:
    FrameData currentFrame;
    
public:
    FrameData NextFrame();
    void SetOscType(OscType oscType) { this->oscType = oscType; }
    
};

#endif /* NaiveOscillator_hpp */
