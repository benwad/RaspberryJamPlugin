#pragma once

#include "Voice.h"
#include "ADSR.h"

class StringVoice : public Voice
{
    const static int bufferSize = 1024;

    private:
        double buffer[bufferSize];
        double refBuffer[bufferSize];
        double feedbackValue = 0.996f;
        int currentFrame = 0;
        int cycleSize = 220;
        double gain = 0.8f;
        double sampleRate = 44100.0f;
        int noteNumber;
        bool needsReset = false;
        ADSR envelope;

    public:
        StringVoice();
		FrameData NextFrame();
		void SetNoteNumber(int noteNumber) ;
		void NoteOn();
		void NoteOff();
		bool IsActive();
		int GetNoteNumber();
        void SetEnvelopeParams(
                                   int attack,
                                   int decay,
                                   double sustain,
                                   int release
                               );

        void Reset(); // Reset the buffer
};
