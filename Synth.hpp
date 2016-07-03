//
//  Synth.hpp
//  AnotherTestSynth
//
//  Created by Ben Wadsworth on 02/07/2016.
//
//

#ifndef Synth_hpp
#define Synth_hpp

#include <stdio.h>

#include "FrameData.h"
#include "Voice.h"

class Synth
{
    static const int numVoices = 16;

private:
    double mFreq;
    double mNoteGain;
    double mSampleRate;
    int mPhase;
    double mGainL, mGainR;
    Voice* voices[numVoices];
public:
    Synth();
    FrameData NextFrame();
    void WriteFrames(unsigned long numFrames, double* out);
    void Reset();
    void OnNoteOn(int noteNumber, int velocity);
    void OnNoteOff(int noteNumber, int velocity);
    Voice* FindFreeVoice();

    void SetFrequency(double pFreq) { this->mFreq = pFreq; }
    void SetGain(double pGain) { this->mNoteGain = pGain; }
    void SetSampleRate(double pSampleRate) { this->mSampleRate = pSampleRate; }
    void SetMasterGainL(double pGain) { this->mGainL = pGain; }
    void SetMasterGainR(double pGain) { this->mGainR = pGain; }
};

#endif /* Synth_hpp */
