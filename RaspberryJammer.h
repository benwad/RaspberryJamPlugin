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

private:
  IBitmapOverlayControl* mAboutBox;
  IControl* mKeyboard;
  IControl* mCutoffKnob;
  
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

  kFilterCutoffX = 100,
  kFilterCutoffY = 100,
  kKnobFrames = 60
};

#endif //__RASPBERRYJAMMER__
