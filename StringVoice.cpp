#include "StringVoice.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>

#include "FrameData.h"
#include "MidiNotes.h"

StringVoice::StringVoice()
{
//    this->envelope.SetParams(0, 220, 1.0f, 44100);
    this->envelope.SetParams(5, 2000, 1.0f, 44100);
    // Fill the buffer with white noise
    srand(static_cast<unsigned> (time(0)));

    for (int i=0; i < this->bufferSize; i++) {
        this->refBuffer[i] = static_cast<double>(rand()) / (static_cast<double>(RAND_MAX/2.0f)) - 1.0f;
    }

    this->Reset();
}

FrameData StringVoice::NextFrame()
{
    double frame = 0.0f;
    
//    if (isnan(this->currentFrame)) {
//        std::cout << "nan" << std::endl;
//    }

    buffer[this->currentFrame] = \
        ((buffer[this->currentFrame] + buffer[this->currentFrame+1]) / 2.0f) * this->feedbackValue;

    frame = buffer[this->currentFrame] * this->gain * this->envelope.NextFrame();

    this->currentFrame++;
    if (this->currentFrame >= this->cycleSize) {
        this->currentFrame -= this->cycleSize;
    }

    else if (this->needsReset && !this->envelope.IsActive()) {
        this->Reset();
    }

    return FrameData(
        frame,
        frame
    );
}

void StringVoice::SetNoteNumber(int noteNumber) 
{
    this->noteNumber = noteNumber;

    this->cycleSize = this->sampleRate / midiFrequencies[noteNumber];
}

void StringVoice::NoteOn()
{
    envelope.NoteOn();
}

void StringVoice::NoteOff()
{
    envelope.NoteOff();

    this->needsReset = true;
}

bool StringVoice::IsActive()
{
    return envelope.IsActive();
}

int StringVoice::GetNoteNumber()
{
    return this->noteNumber;
}

void StringVoice::Reset()
{
    std::cout << "Resetting " << this->bufferSize << " bytes of buffer" << std::endl;
    memcpy(this->buffer, this->refBuffer, this->bufferSize);
//    std::copy(std::begin(this->refBuffer), std::end(this->refBuffer), this->buffer);
    this->currentFrame = 0;
    this->needsReset = false;
}

void StringVoice::SetEnvelopeParams(
                       int attack,
                       int decay,
                       double sustain,
                       int release
                       )
{
    this->envelope.SetParams(attack, decay, sustain, release);
}

