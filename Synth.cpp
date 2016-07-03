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

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

Synth::Synth()
  : mGainL(1.),
    mGainR(1.),
    mNoteGain(0.),
    mPhase(0),
    mSampleRate(44100.0),
    mFreq(440.)
{
    for (int i=0; i < this->numVoices; i++) {
        this->voices[i] = new PolysynthVoice();
    }
}

FrameData Synth::NextFrame()
{
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
    
    return currentFrame * mGainL;
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