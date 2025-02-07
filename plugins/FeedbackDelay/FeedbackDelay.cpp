#include "FeedbackDelay.hpp"
#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace FeedbackDelay {

FeedbackDelay::FeedbackDelay() {
  delayTimePast = in0(DELAYTIME);
  feedbackPast = in0(FEEDBACK);

  float maxDelayTime = in0(MAX_DELAYTIME);
  isDelayTimeAudioRate = isAudioRateIn(DELAYTIME);
  isFeedbackAudioRate = isAudioRateIn(FEEDBACK);

  // Just pass through the audio with an amount of feedback
  auto feedbackFunc = [](float feedback, float delayed) {
    return feedback * delayed;
  };

  delayLine = std::make_unique<DelayLine<FeedbackFunc>>(
      this->sampleRate(), maxDelayTime, feedbackFunc);

  mCalcFunc = make_calc_function<FeedbackDelay, &FeedbackDelay::next>();
  next(1);
}

void FeedbackDelay::next(int nSamples) {
  const float *input = in(AUDIOINPUT);
  float *outbuf = out(0);

  for (int i = 0; i < nSamples; ++i) {
    float delayTime = isDelayTimeAudioRate
                          ? in(DELAYTIME)[i]
                          : makeSlope(in0(DELAYTIME), delayTimePast).consume();
    float feedback = isFeedbackAudioRate
                         ? in(FEEDBACK)[i]
                         : makeSlope(in0(FEEDBACK), feedbackPast).consume();

    outbuf[i] = delayLine->process(input[i], delayTime, feedback);
  }

  delayTimePast = in0(DELAYTIME);
  feedbackPast = in0(FEEDBACK);
}

} // namespace FeedbackDelay

PluginLoad(FeedbackDelayUGens) {
  ft = inTable;
  registerUnit<FeedbackDelay::FeedbackDelay>(ft, "FeedbackDelay", false);
}
