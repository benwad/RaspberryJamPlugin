#ifndef __RASPBERRYJAMMER__
#define __RASPBERRYJAMMER__

#include "IPlug_include_in_plug_hdr.h"
#include "IMidiQueue.h"

#include "Synth.hpp"

// http://www.musicdsp.org/archive.php?classid=3#257

class CParamSmooth
{
public:
  CParamSmooth() { a = 0.99; b = 1. - a; z = 0.; };
  ~CParamSmooth() {};
  inline double Process(double in) { z = (in * b) + (z * a); return z; }
private:
  double a, b, z;
};

class RaspberryJammer : public IPlug
{
public:

  RaspberryJammer(IPlugInstanceInfo instanceInfo);
  ~RaspberryJammer();

  void Reset();
  void OnParamChange(int paramIdx);

  void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);
  bool HostRequestingAboutBox();

  int GetNumKeys();
  bool GetKeyStatus(int key);
  void ProcessMidiMsg(IMidiMsg* pMsg);
  void CreatePreset(
                    char* pName,
                    double pGainL,
                    double pGainR,
                    double pFilterCutoff,
                    double pFilterResonance,
                    int pEnvAttack,
                    int pEnvDecay,
                    double pEnvSustain,
                    int pEnvRelease,
                    double pLfoRate,
                    double pLfoDepth);
  void CreatePresets();

private:
  IBitmapOverlayControl* mAboutBox;
  IControl* mKeyboard;
  
  // Filter knobs
  IControl* mCutoffKnob;
  IControl* mResonanceKnob;
  
  // ADSR envelope knobs
  IControl* mEnvAttackKnob;
  IControl* mEnvDecayKnob;
  IControl* mEnvSustainKnob;
  IControl* mEnvReleaseKnob;
  IControl* mLfoRateKnob;
  IControl* mLfoDepthKnob;
  
  int mMeterIdx_L, mMeterIdx_R;

  IMidiQueue mMidiQueue;
  
  Synth synth;

  int mNumKeys; // how many keys are being played (via midi)
  bool mKeyStatus[128]; // array of on/off for each key

  int mKey;

  double mGainL, mGainR;
  double mPrevL, mPrevR;

  ITimeInfo mTimeInfo;

  CParamSmooth mGainLSmoother, mGainRSmoother;
};

enum ELayout
{
  kWidth = GUI_WIDTH,  // width of plugin window
  kHeight = GUI_HEIGHT, // height of plugin window

  kKeybX = 1,
  kKeybY = 233,

  kFilterCutoffX = 50,
  kFilterCutoffY = 40,
  kFilterResonanceX = 109,
  kFilterResonanceY = 40,
  
  kEnvAttackX = 50,
  kEnvAttackY = 130,
  kEnvDecayX = 109,
  kEnvDecayY = 130,
  kEnvSustainX = 168,
  kEnvSustainY = 130,
  kEnvReleaseX = 227,
  kEnvReleaseY = 130,
  
  kLfoRateX = 168,
  kLfoRateY = 40,
  kLfoDepthX = 227,
  kLfoDepthY = 40,
  
  kKnobFrames = 60
};

#endif //__RASPBERRYJAMMER__
