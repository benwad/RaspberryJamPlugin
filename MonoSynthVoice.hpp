//
//  MonoSynthVoice.hpp
//  RaspberryJammer
//
//  Created by Ben Wadsworth on 15/08/2016.
//
//

#ifndef MonoSynthVoice_hpp
#define MonoSynthVoice_hpp

#include <stdio.h>

#include "Voice.h"
#include "Oscillator.h"
#include "ADSR.h"

class MonoSynthVoice : public Voice
{
private:
    Oscillator* oscillator;
    
    int noteNumber;
    double gain;
    ADSR envelope;
    
public:
    MonoSynthVoice();
    ~MonoSynthVoice();
    FrameData NextFrame();
    void SetNoteNumber(int noteNumber);
    void SetEnvelopeParams(
                                   int attack,
                                   int decay,
                                   double sustain,
                                   int release
                                   );
    virtual void NoteOn();
    virtual void NoteOff();
    virtual bool IsActive();
    virtual int GetNoteNumber();
};

#endif /* MonoSynthVoice_hpp */
