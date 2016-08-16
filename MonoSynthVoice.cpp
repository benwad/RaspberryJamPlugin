//
//  MonoSynthVoice.cpp
//  RaspberryJammer
//
//  Created by Ben Wadsworth on 15/08/2016.
//
//

#include "MonoSynthVoice.hpp"

#include "PolyBLEPOscillator.hpp"
#include "MidiNotes.h"

MonoSynthVoice::MonoSynthVoice()
{
    this->oscillator = new PolyBLEPOscillator();
    
    this->gain = 0.5f;
}

MonoSynthVoice::~MonoSynthVoice()
{
    delete this->oscillator;
}

FrameData MonoSynthVoice::NextFrame()
{
    double envValue = this->envelope.NextFrame();
    
    FrameData oscFrame = this->oscillator->NextFrame();
    
    return oscFrame * (this->gain * envValue);
}

void MonoSynthVoice::SetNoteNumber(int noteNumber)
{
    this->noteNumber = noteNumber;
    double fundamental = midiFrequencies[noteNumber];
    this->oscillator->SetFrequency(fundamental);
//    oscillators[0].first->SetFrequency(fundamental);
//    for (int i=1; i < numOscillators; i++) {
//        oscillators[i].first->SetFrequency(fundamental * (i+1));
//    }
}

void MonoSynthVoice::SetEnvelopeParams(
                                       int attack,
                                       int decay,
                                       double sustain,
                                       int release
                                       )
{
    this->envelope.SetParams(attack, decay, sustain, release);
}

int MonoSynthVoice::GetNoteNumber()
{
    return this->noteNumber;
}

void MonoSynthVoice::NoteOn()
{
    envelope.NoteOn();
}

void MonoSynthVoice::NoteOff()
{
    envelope.NoteOff();
}

bool MonoSynthVoice::IsActive()
{
    return envelope.IsActive();
}