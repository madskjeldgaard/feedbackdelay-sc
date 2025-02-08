// PluginFeedbackDelay.hpp
// Mads Kjeldgaard (madskjeldgaardnielsen@gmail.com)

// This was originally inspired by
// https://github.com/supercollider/example-plugins

#pragma once

#include "../DelayLine.hpp"
#include "SC_PlugIn.hpp"

namespace FeedbackDelay {

class FeedbackDelay : public SCUnit {
public:
  FeedbackDelay();
  ~FeedbackDelay();

private:
  void next(int nSamples);

  enum paramNames {
    AUDIOINPUT,
    MAX_DELAYTIME,
    DELAYTIME,
    FEEDBACK,
  };

  float delayTimePast, feedbackPast;
  bool isDelayTimeAudioRate;
  bool isFeedbackAudioRate;

  float *delayBuffer;
  int bufsize;

  using FeedbackFunc = std::function<float(float, float)>;
  std::unique_ptr<DelayLine<FeedbackFunc>> delayLine;
};

} // namespace FeedbackDelay
;
