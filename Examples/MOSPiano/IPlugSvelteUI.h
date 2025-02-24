#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "Oscillator.h"
#include "Smoothers.h"
#include "ISender.h"

#include "pitchgrid.hpp"

using namespace iplug;

const int kNumPresets = 3;

enum EParams
{
  kBaseTune = 0,
  kOctaveExp,
  kG,
  kGs,
  kPeriod,
  kOffset,
  kDepth,
  kNumParams
};
 
enum EMsgTags
{
  kMsgTagButton1 = 0,
  kMsgTagButton2 = 1,
  kMsgTagButton3 = 2,
  kMsgTagBinaryTest = 3
};

enum EControlTags
{
  kCtrlTagMeter = 0,
  kCtrlTagNoteOnOff
};

class IPlugSvelteUI final : public Plugin
{
public:
  IPlugSvelteUI(const InstanceInfo& info);
  
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  //void OnReset() override;
  void OnIdle() override;
  void ProcessMidiMsg(const IMidiMsg& msg) override;
  
  ~IPlugSvelteUI();

private:
  //iplug::IPeakSender<2> mPeakSender;
  iplug::ISender<1,64,char> mSender;
  //FastSinOscillator<sample> mOscillator {0., 440.};
  //LogParamSmooth<sample, 1> mGainSmoother;
  
  ConsistentTuning tuning = ConsistentTuning({2, 5}, 2.f, {1, 3}, pow(2.f, 7.f/12.f));
  
  
  double base_tune_last;
  double octave_exp_last;
  double g_last;
  int period_last;
  int offset_last;
  int depth_last;
  IntegerVector coordinates[128];
  double frequencies[128];
  
  bool did_register_MTS_Master = false;
  
};
