//
//  PolyBLEPOscillator.hpp
//  RaspberryJammer
//
//  Created by Ben Wadsworth on 15/08/2016.
//
//

#ifndef PolyBLEPOscillator_hpp
#define PolyBLEPOscillator_hpp

#include <iostream>

#include "Oscillator.h"
#include "FrameData.h"

class PolyBLEPOscillator: public Oscillator
{
private:
    double lastOutput;
    double PolyBlep(double t);
public:
    FrameData NextFrame();
    void SetFrequency(double frequency);
};

#endif /* PolyBLEPOscillator_hpp */
