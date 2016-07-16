#include "RaspberryJammer.h"
#include "IPlug_include_in_plug_src.h"
#include "resource.h"

#include "IControl.h"
#include "IKeyboardControl.h"

#include <iostream>

const int kNumPrograms = 8;

#define PITCH 440.

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

enum EParams
{
  kGainL = 0,
  kGainR,
  kMode,
  kFilterCutoff,
  kFilterResonance,
  kEnvAttack,
  kEnvDecay,
  kEnvSustain,
  kEnvRelease,
  kLfoRate,
  kLfoDepth,
  kNumParams
};

RaspberryJammer::RaspberryJammer(IPlugInstanceInfo instanceInfo)
  : IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo),
    mGainL(1.),
    mGainR(1.),
    mNumKeys(0),
    mKey(-1),
    mPrevL(0.0),
    mPrevR(0.0)

{
  TRACE;

  memset(mKeyStatus, 0, 128 * sizeof(bool));

  //arguments are: name, defaultVal, minVal, maxVal, step, label
  GetParam(kGainL)->InitDouble("GainL", -12.0, -70.0, 12.0, 0.1, "dB");
  GetParam(kGainR)->InitDouble("GainR", -12.0, -70.0, 12.0, 0.1, "dB");
  GetParam(kFilterCutoff)->InitDouble("Cutoff", 440., 0., 12000., 1.);
  GetParam(kFilterResonance)->InitDouble("Q", 0.1, 0.0, 1.0, 0.01);
  GetParam(kLfoRate)->InitDouble("LFO Rate", 0.5, 0.001, 20.0, 0.001);
  GetParam(kLfoDepth)->InitDouble("LFO Depth", 1., 0.0, 1000., 0.0001);
  GetParam(kEnvAttack)->InitInt("Env Attack", 300, 0, 100000);
  GetParam(kEnvDecay)->InitInt("Env Decay", 1000, 0, 100000);
  GetParam(kEnvSustain)->InitDouble("Env Sustain", 1., 0., 1., 0.01);
  GetParam(kEnvRelease)->InitInt("Env Release", 2000, 0, 100000);
  GetParam(kMode)->InitEnum("Mode", 0, 6);
  GetParam(kMode)->SetDisplayText(0, "a");
  GetParam(kMode)->SetDisplayText(1, "b");
  GetParam(kMode)->SetDisplayText(2, "c");
  GetParam(kMode)->SetDisplayText(3, "d");
  GetParam(kMode)->SetDisplayText(4, "e");
  GetParam(kMode)->SetDisplayText(5, "f");

  IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
  pGraphics->AttachBackground(BG_ID, BG_FN);

  IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, kKnobFrames);
  IText text = IText(14);
  IBitmap regular = pGraphics->LoadIBitmap(WHITE_KEY_ID, WHITE_KEY_FN, 6);
  IBitmap sharp   = pGraphics->LoadIBitmap(BLACK_KEY_ID, BLACK_KEY_FN);
  
  // Filter knobs
  mCutoffKnob = new IKnobMultiControl(this, kFilterCutoffX, kFilterCutoffY, kFilterCutoff, &knob);
  pGraphics->AttachControl(mCutoffKnob);
  mResonanceKnob = new IKnobMultiControl(this, kFilterResonanceX, kFilterResonanceY, kFilterResonance, &knob);
  pGraphics->AttachControl(mResonanceKnob);
  
  // LFO knobs
  mLfoRateKnob = new IKnobMultiControl(this, kLfoRateX, kLfoRateY, kLfoRate, &knob);
  pGraphics->AttachControl(mLfoRateKnob);
  mLfoDepthKnob = new IKnobMultiControl(this, kLfoDepthX, kLfoDepthY, kLfoDepth, &knob);
  pGraphics->AttachControl(mLfoDepthKnob);
  
  // ADSR envelope knobs
  mEnvAttackKnob = new IKnobMultiControl(this, kEnvAttackX, kEnvAttackY, kEnvAttack, &knob);
  pGraphics->AttachControl(mEnvAttackKnob);
  mEnvDecayKnob = new IKnobMultiControl(this, kEnvDecayX, kEnvDecayY, kEnvDecay, &knob);
  pGraphics->AttachControl(mEnvDecayKnob);
  mEnvSustainKnob = new IKnobMultiControl(this, kEnvSustainX, kEnvSustainY, kEnvSustain, &knob);
  pGraphics->AttachControl(mEnvSustainKnob);
  mEnvReleaseKnob = new IKnobMultiControl(this, kEnvReleaseX, kEnvReleaseY, kEnvRelease, &knob);
  pGraphics->AttachControl(mEnvReleaseKnob);

  //                    C#     D#          F#      G#      A#
  int coords[12] = { 0, 7, 12, 20, 24, 36, 43, 48, 56, 60, 69, 72 };
  mKeyboard = new IKeyboardControl(this, kKeybX, kKeybY, 48, 5, &regular, &sharp, coords);

  pGraphics->AttachControl(mKeyboard);

  IBitmap about = pGraphics->LoadIBitmap(ABOUTBOX_ID, ABOUTBOX_FN);
  mAboutBox = new IBitmapOverlayControl(this, 100, 100, &about, IRECT(540, 250, 680, 290));
  pGraphics->AttachControl(mAboutBox);
  AttachGraphics(pGraphics);

  //MakePreset("preset 1", ... );
  MakeDefaultPreset((char *) "-", kNumPrograms);
}

RaspberryJammer::~RaspberryJammer() {}

void RaspberryJammer::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
  // Mutex is already locked for us.
//  double* in1 = inputs[0];
//  double* in2 = inputs[1];
  double* out1 = outputs[0];
  double* out2 = outputs[1];
  double peakL = 0.0, peakR = 0.0;

  GetTime(&mTimeInfo);

  IKeyboardControl* pKeyboard = (IKeyboardControl*) mKeyboard;

  if (pKeyboard->GetKey() != mKey)
  {
    IMidiMsg msg;

    if (mKey >= 0)
    {
      msg.MakeNoteOffMsg(mKey + 48, 0, 0);
      mMidiQueue.Add(&msg);
    }

    mKey = pKeyboard->GetKey();

    if (mKey >= 0)
    {
      msg.MakeNoteOnMsg(mKey + 48, pKeyboard->GetVelocity(), 0, 0);
      mMidiQueue.Add(&msg);
    }
  }

  for (int offset = 0; offset < nFrames; ++offset, /*++in1, ++in2,*/ ++out1, ++out2)
  {
    while (!mMidiQueue.Empty())
    {
      IMidiMsg* pMsg = mMidiQueue.Peek();
      if (pMsg->mOffset > offset) break;

      // TODO: make this work on win sa
#if !defined(OS_WIN) && !defined(SA_API)
      SendMidiMsg(pMsg);
#endif

      int status = pMsg->StatusMsg();

      switch (status)
      {
        case IMidiMsg::kNoteOn:
        case IMidiMsg::kNoteOff:
        {
          int velocity = pMsg->Velocity();
          // Note On
          if (status == IMidiMsg::kNoteOn && velocity)
          {
            this->synth.OnNoteOn(pMsg->NoteNumber(), pMsg->Velocity());
          }
          // Note Off
          else // if (status == IMidiMsg::kNoteOff || !velocity)
          {
            this->synth.OnNoteOff(pMsg->NoteNumber(), pMsg->Velocity());
          }
          break;
        }
      }

      mMidiQueue.Remove();
    }
    
    FrameData nextFrame = this->synth.NextFrame();
    *out1 = nextFrame.left_phase;
    *out2 = nextFrame.right_phase;
    
//    *out1 = sin( 2. * M_PI * mFreq * mPhase / mSampleRate ) * mGainLSmoother.Process(mGainL * mNoteGain);
//    *out2 = sin( 2. * M_PI * mFreq * 1.01 * (mPhase++) / mSampleRate ) * mGainRSmoother.Process(mGainR * mNoteGain);

    peakL = IPMAX(peakL, fabs(*out1));
    peakR = IPMAX(peakR, fabs(*out2));
  }

  const double METER_ATTACK = 0.6, METER_DECAY = 0.05;
  double xL = (peakL < mPrevL ? METER_DECAY : METER_ATTACK);
  double xR = (peakR < mPrevR ? METER_DECAY : METER_ATTACK);

  peakL = peakL * xL + mPrevL * (1.0 - xL);
  peakR = peakR * xR + mPrevR * (1.0 - xR);

  mPrevL = peakL;
  mPrevR = peakR;

  /*
  if (GetGUI())
  {
    GetGUI()->SetControlFromPlug(mMeterIdx_L, peakL);
    GetGUI()->SetControlFromPlug(mMeterIdx_R, peakR);
  }
   */

  mMidiQueue.Flush(nFrames);
}

void RaspberryJammer::Reset()
{
  TRACE;
  IMutexLock lock(this);

  this->synth.Reset();
  this->synth.SetSampleRate(GetSampleRate());
  mMidiQueue.Resize(GetBlockSize());
}

void RaspberryJammer::OnParamChange(int paramIdx)
{
  IMutexLock lock(this);

  switch (paramIdx)
  {
    case kGainL:
      mGainL = GetParam(kGainL)->DBToAmp();
      break;
    case kGainR:
      mGainR = GetParam(kGainR)->DBToAmp();
      break;
    case kFilterCutoff:
      this->synth.SetFilterCutoff(GetParam(kFilterCutoff)->Value());
      break;
    case kFilterResonance:
      this->synth.SetFilterResonance(GetParam(kFilterResonance)->Value());
      break;
    case kEnvAttack:
      this->synth.SetEnvAttack(GetParam(kEnvAttack)->Value());
      break;
    case kEnvDecay:
      this->synth.SetEnvDecay(GetParam(kEnvDecay)->Value());
      break;
    case kEnvSustain:
      this->synth.SetEnvSustain(GetParam(kEnvSustain)->Value());
      break;
    case kEnvRelease:
      this->synth.SetEnvRelease(GetParam(kEnvRelease)->Value());
      break;
    case kLfoRate:
      this->synth.SetLfoRate(GetParam(kLfoRate)->Value());
      break;
    case kLfoDepth:
      this->synth.SetLfoDepth(GetParam(kLfoDepth)->Value());
      break;
    default:
      break;
  }
}

void RaspberryJammer::ProcessMidiMsg(IMidiMsg* pMsg)
{
  int status = pMsg->StatusMsg();
  int velocity = pMsg->Velocity();

  switch (status)
  {
    case IMidiMsg::kNoteOn:
    case IMidiMsg::kNoteOff:
      // filter only note messages
      if (status == IMidiMsg::kNoteOn && velocity)
      {
        mKeyStatus[pMsg->NoteNumber()] = true;
        mNumKeys += 1;
      }
      else
      {
        mKeyStatus[pMsg->NoteNumber()] = false;
        mNumKeys -= 1;
      }
      break;
    default:
      return; // if !note message, nothing gets added to the queue
  }

  mKeyboard->SetDirty();
  mMidiQueue.Add(pMsg);
}

// Should return non-zero if one or more keys are playing.
int RaspberryJammer::GetNumKeys()
{
  IMutexLock lock(this);
  return mNumKeys;
}

// Should return true if the specified key is playing.
bool RaspberryJammer::GetKeyStatus(int key)
{
  IMutexLock lock(this);
  return mKeyStatus[key];
}

//Called by the standalone wrapper if someone clicks about
bool RaspberryJammer::HostRequestingAboutBox()
{
  IMutexLock lock(this);
  if(GetGUI())
  {
    // get the IBitmapOverlay to show
    mAboutBox->SetValueFromPlug(1.);
    mAboutBox->Hide(false);
  }
  return true;
}
