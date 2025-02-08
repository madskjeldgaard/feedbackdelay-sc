// PluginSpaceTub.hpp
// Mads Kjeldgaard (madskjeldgaardnielsen@gmail.com)

// This was originally inspired by
// https://github.com/supercollider/example-plugins

#pragma once

#include "../DCFilter.hpp"
#include "../DelayLine.hpp"
#include "../SoftClip.hpp"
#include "../VASVF.hpp"
#include "SC_PlugIn.hpp"
#include "safety_limiter.hpp"

namespace SpaceTub {

class SpaceTub : public SCUnit {
public:
  SpaceTub();
  ~SpaceTub();

private:
  void next(int nSamples);

  enum paramNames {
    AUDIOINPUT,
    MAX_DELAYTIME,
    DELAYTIME,
    FEEDBACK,
    CUTOFF,
    RESONANCE,
    FILTER_TYPE,
    LIMITER_ON,
    LIMITER_RELEASE,
    LIMITER_HOLDTIME,
    USE_SOFTCLIP
  };

  float delayTimePast, feedbackPast, cutoffPast, resPast, limiterRelease,
      limiterHoldTime;
  bool isDelayTimeAudioRate, isFeedbackAudioRate, isCutoffAudioRate,
      isResAudioRate;
  int filterType;

  float *delayBuffer;
  int bufsize;

  bool limiterOn = true;
  bool useSoftClip = true;

  using FeedbackFunc = std::function<float(float, float)>;
  std::unique_ptr<DelayLine<FeedbackFunc>> delayLine;
  std::unique_ptr<DCFilter> dcFilter;
  std::unique_ptr<VASVF> filter;
  std::unique_ptr<safety_limiter::SafetyLimiter> limiter;
  std::unique_ptr<SoftClipper> clipper;
};

} // namespace SpaceTub
;
