#pragma once

#include <utility>

#include "Voice.h"
#include "Oscillator.h"
#include "ADSR.h"

class PolysynthVoice : public Voice {
    
	const static int numOscillators = 6;

    public:
        PolysynthVoice();
        ~PolysynthVoice();
		FrameData NextFrame();
		void SetNoteNumber(int noteNumber);
        void SetEnvelopeParams(
                                int attack,
                                int decay,
                                double sustain,
                                int release
                               );
		void NoteOn();
		void NoteOff();
		bool IsActive();
		int GetNoteNumber();

    private:
		int noteNumber;
		double gain;
        ADSR envelope;
		std::pair<Oscillator*, double> oscillators[numOscillators];
};
