#include "IPlugSvelteUI.h"
#include "IPlug_include_in_plug_src.h"
#include "IPlugPaths.h"
#include "libMTSMaster.h"
#include <iostream>

IPlugSvelteUI::IPlugSvelteUI(const InstanceInfo& info)
: Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kBaseTune)->InitGain("base", 0., -1., 1.);
  GetParam(kOctaveExp)->InitGain("stretch", 1., 0.1, 2.2);
  GetParam(kG)->InitGain("angle coarse", .6, .5, 1.);
  GetParam(kGs)->InitGain("angle fine", .5, 0., 1.);
  GetParam(kPeriod)->InitGain("period", 12, 2, 36);
  GetParam(kOffset)->InitGain("offset", 5, 1, 12);
  GetParam(kDepth)->InitGain("depth", 0, 8, 3);
  
  base_tune_last = GetParam(kBaseTune)->Value();
  octave_exp_last = GetParam(kOctaveExp)->Value();
  g_last = GetParam(kG)->Value();
  //period_last = (int)(GetParam(kPeriod)->Value()+.5);
  //offset_last =  (int)(GetParam(kOffset)->Value()+.5);
  //depth_last = (int)(GetParam(kDepth)->Value()+.5);
  period_last = GetParam(kPeriod)->Int();
  offset_last =  GetParam(kOffset)->Int();
  depth_last = GetParam(kDepth)->Int();

  std::cout << "init values for params" <<
  " base_tune " << base_tune_last <<
  " octave_exp " << octave_exp_last <<
  " g " << g_last <<
   " period " << period_last <<
   " offset " << offset_last <<
  " depth " << depth_last << "\n";

  
  //#ifdef DEBUG
  SetCustomUrlScheme("iplug2");
  SetEnableDevTools(true);
  //#endif
  
  mEditorInitFunc = [&]() {
    //LoadIndexHtml(__FILE__, GetBundleID());
    LoadURL("http://localhost:5174/");
    EnableScroll(false);
  };
  
  MakePreset("One", -70.);
  MakePreset("Two", -30.);
  MakePreset("Three", 0.);
  
  did_register_MTS_Master = false;

  
}
IPlugSvelteUI::~IPlugSvelteUI(){
  MTS_DeregisterMaster();
}


struct Node{
  double f;
  IntegerVector v;
};
int comp_nodes(const void * an, const void * bn){
  double a = (*((Node*)an)).f;
  double b = (*((Node*)bn)).f;
  if (a > b) return 1;
  if (a < b) return -1;
  return 0;
};
void sort_base(Node *base, size_t sz){
  qsort(base, sz, sizeof(Node), comp_nodes);
  
  //std::cout << "base ";
  //for (int i=0;i<sz;i++){
  //  std::cout << '<' << i << '/' << base[i].v.x << ',' << base[i].v.y << '/' << base[i].f << "> ";
  //}
  //std::cout << '\n';
  
}
void generate_lattice(IntegerVector* coordinates, double g, const int period, int offset, int start_midi=60){
  Node n = {0., {0,0}};
  
  std::cout << "gen lattice " << g <<' '<< period <<' '<< offset <<' '<< start_midi << '\n';
  for (int i = 0; i< offset-1; i++){
    n.f -= g;
    n.v -= {1,0};
    while (n.f<0.) {
      n.f += 1.;
      n.v += {1,1};
    }
  }
  std::cout << "n " << n.f <<' '<< n.v.x <<' '<< n.v.y << '\n';
  
  Node base[period];
  for (int i = 0; i< period; i++){
    while (n.f>=1. - 1e-6) {
      n.f -= 1.;
      n.v -= {1,1};
    }
    Node _n = {n.f, {n.v.x, n.v.y}};
    base[i] = _n;
    n.f += g;
    n.v += {1,0};
  }
  sort_base(base, period);
  
  
  for (int i = 0; i< period; i++){
    std::cout << "b[" << i << "]=(" << base[i].v.x << ',' << base[i].v.y << ")=" << 1200 * base[i].f << '\n';
  }
  
  IntegerVector oct_v = IntegerVector(1,1);
  for (int i = 0; i<128; i++){
    int idx = (i-start_midi + 64*period)%period;
    int oct = (i - start_midi + 64*period) / period - 64;
    coordinates[i] = base[idx].v + oct_v * oct;
  }
}

void tune_lattice(double* frequencies, ConsistentTuning* tuning, IntegerVector* coordinates, double base_tune, double octave_exp, double g){
  tuning->setParams({1,1}, pow(2,octave_exp), {1,0}, pow(2,octave_exp * g));
  double base_freq = 440. * pow(2, base_tune - .75);
  for (int i=0; i<128; i++){
    frequencies[i] = base_freq * tuning->vecToFreqRatio(coordinates[i]);
  }
  
  for (int i=60;i<=72;i++){
    std::cout << "f["<<i<<"]=(" << coordinates[i].x << ','  << coordinates[i].y << ")=" << 1200.*log2(frequencies[i]/base_freq) <<'\n';
  }
}

void IPlugSvelteUI::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  if (!did_register_MTS_Master){
    
    if (MTS_CanRegisterMaster()){
      MTS_RegisterMaster();
      std::cout << "did register MTS\n";
      did_register_MTS_Master = true;
    }else{
      std::cout << "could not register MTS\n";
    }
    
  }
  
  const double base_tune = GetParam(kBaseTune)->Value();
  const double octave_exp = GetParam(kOctaveExp)->Value();
  const double g = GetParam(kG)->Value();
  const int period = GetParam(kPeriod)->Int();
  int offset = GetParam(kOffset)->Int();
  if (period != period_last){
    GetParam(kOffset)->InitGain("offset", 5, 1, period);
    if (offset > period){
      offset = period;
      GetParam(kOffset)->Set(offset);
    }
  }
    
  bool dirty = false;
  if (g != g_last || period != period_last || offset != offset_last){
    double g_corrected = 1.0/(1.0+tan((1-g)*3.14159265359*.5)); // real g parameter
    
    //std::cout << "gen "
    //  << g << " " << g_last << " "
    //  << period << " " << period_last << " "
    //  << offset << " " << offset_last << " "
    //  <<"\n";
    
    // regenerate lattice
    generate_lattice(coordinates, g_corrected, period, offset);
    
    //std::cout << "did regen "
    //<< '(' << coordinates[0].x << ',' << coordinates[0].y << ')'
    //<< '(' << coordinates[60].x << ',' << coordinates[60].y << ')'
    //<< '(' << coordinates[72].x << ',' << coordinates[72].y << ')'
    //<< '(' << coordinates[127].x << ',' << coordinates[127].y << ')'
    //  << "\n";
    dirty = true;
    g_last = g;
    period_last = period;
    offset_last = offset;
  }
  if (dirty || base_tune != base_tune_last || octave_exp != octave_exp_last){
    // retune existing lattice
    double g_corrected = 1.0/(1.0+tan((1-g)*3.14159265359*.5)); // real g parameter
    tune_lattice(frequencies, &tuning, coordinates, base_tune, octave_exp, g_corrected);
    MTS_SetNoteTunings(frequencies);
    
    double min_freq = 1e10;
    double max_freq = -1e10;
    for (int i=0; i<128; i++){
      if (frequencies[i]<min_freq)min_freq=frequencies[i];
      if (frequencies[i]>max_freq)max_freq=frequencies[i];
    }
    
    //std::cout << "did retune "
    //<< min_freq << " "
    //<< frequencies[0] << " "
    //<< frequencies[60] << " "
    //<< frequencies[72] << " "
    //<< frequencies[127] << " "
    //<< max_freq << " "
    //  <<"\n";
    
    //std::cout << "connected clients " <<  MTS_GetNumClients() << "\n";
    //std::cout << "MTS_CanRegisterMaster() " <<  MTS_CanRegisterMaster() << "\n";
    //std::cout << "MTS_HasIPC() " <<  MTS_HasIPC() << "\n";
    base_tune_last = base_tune;
    octave_exp_last = octave_exp;
    dirty = false;
    
    //test
    //mSender.PushData({kCtrlTagNoteOnOff, {64}, 1, 0});
  }
  
  //mOscillator.ProcessBlock(inputs[0], nFrames); // comment for audio in

  //for (int s = 0; s < nFrames; s++)
  //{
  //  outputs[0][s] = inputs[0][s] * mGainSmoother.Process(0.);
  //  outputs[1][s] = outputs[0][s]; // copy left
  //}
  
  //mPeakSender.ProcessBlock(outputs, nFrames, kCtrlTagMeter);
  
  
}

//void IPlugSvelteUI::OnReset()
//{
//  auto sr = GetSampleRate();
//  mOscillator.SetSampleRate(sr);
//  mGainSmoother.SetSmoothTime(20., sr);
//}

void IPlugSvelteUI::OnIdle()
{
  //mPeakSender.TransmitData(*this);
  mSender.TransmitData(*this);
}


void IPlugSvelteUI::ProcessMidiMsg(const IMidiMsg& msg)
{
  TRACE;
  
  int status = msg.StatusMsg();
  char val;
  switch (status)
  {
    case IMidiMsg::kNoteOn:
      val = msg.NoteNumber() % 128;
      mSender.PushData({kCtrlTagNoteOnOff, {val}, 1, 0});
      break;
    case IMidiMsg::kNoteOff:
      val = msg.NoteNumber() % 128;
      val += 128;
      mSender.PushData({kCtrlTagNoteOnOff, {val}, 1, 0});
      break;
    case IMidiMsg::kPolyAftertouch:
    case IMidiMsg::kControlChange:
    case IMidiMsg::kProgramChange:
    case IMidiMsg::kChannelAftertouch:
    case IMidiMsg::kPitchWheel:
    {
      goto handle;
    }
    default:
      return;
  }
  
handle:
  std::cout << "MIDI message\n";
  SendMidiMsg(msg);
}
