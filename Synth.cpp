//
//  Synth.cpp
//  AnotherTestSynth
//
//  Created by Ben Wadsworth on 02/07/2016.
//
//

#include <math.h>
#include <iostream>

#include "Synth.hpp"

#include "MidiNotes.h"
#include "PolysynthVoice.h"

Synth::Synth()
  : mGainL(1.),
    mGainR(1.),
    mNoteGain(0.),
    mPhase(0),
    mSampleRate(44100.),
    mFreq(440.),
    mCutoffVal(8800.),
    mResonanceVal(0.)
{
    this->SetFilterParams(mCutoffVal, mResonanceVal);
    this->filterLfo.SetParams(1.0f, 0.2f);
    
    for (int i=0; i < this->numVoices; i++) {
        this->voices[i] = new PolysynthVoice();
    }
}

Synth::~Synth()
{
    for (int i=0; i < this->numVoices; i++) {
        delete this->voices[i];
    }
}

FrameData Synth::NextFrame()
{
    this->UpdateFilterParams();
    
    FrameData currentFrame = FrameData(0.0f, 0.0f);
    float activeVoices = 0;
    
    // Mix together the voices and apply the filter
    for (int i=0; i < numVoices; i++) {
        if (voices[i]->IsActive()) {
            FrameData nextFrame = voices[i]->NextFrame();
            currentFrame = currentFrame + nextFrame;
            activeVoices += 1.0f;
        }
    }
    
    return this->filter.Run(currentFrame * mGainL);
}

void Synth::WriteFrames(unsigned long numFrames, double* out)
{
    for (unsigned long i=0; i < numFrames; i++)
    {
        FrameData nextFrame = this->NextFrame();
        
        *out++ = nextFrame.left_phase;
        *out++ = nextFrame.right_phase;
    }
}

void Synth::Reset()
{
    this->mPhase = 0;
    this->mNoteGain = 0;
}

void Synth::OnNoteOn(int noteNumber, int velocity)
{
    Voice* voice = this->FindFreeVoice();
    
    voice->SetNoteNumber(noteNumber);
    voice->NoteOn();
}

void Synth::OnNoteOff(int noteNumber, int velocity)
{
    for (int i=0; i < this->numVoices; i++) {
        Voice* voice = this->voices[i];
        if (voice->IsActive() && voice->GetNoteNumber() == noteNumber) {
            voice->NoteOff();
        }
    }
}

void Synth::SetFilterParams(double cutoff, double q)
{
    this->filter.Set(cutoff, q);
}

void Synth::UpdateFilterParams()
{
    double lfoValue = this->filterLfo.NextFrame();
    double cutoffValue = this->mCutoffVal * ((lfoValue + 1.0f) / 5.0f);
    this->SetFilterParams(cutoffValue, this->mResonanceVal);
}

Voice* Synth::FindFreeVoice()
{
    Voice* freeVoice = NULL;
    for (int i=0; i < this->numVoices; i++) {
        if (!this->voices[i]->IsActive()) {
            freeVoice = this->voices[i];
            std::cout << "Voice " << i << std::endl;
            break;
        }
    }
    
    return freeVoice;
}