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

  // Allocate buffer
  bufsize = NEXTPOWEROFTWO(this->sampleRate() * maxDelayTime);
  delayBuffer = (float *)RTAlloc(this->mWorld, bufsize * sizeof(float));

  // Just pass through the audio with an amount of feedback
  auto feedbackFunc = [](float feedback, float delayed) {
    return feedback * delayed;
  };

  delayLine = std::make_unique<DelayLine<FeedbackFunc>>(
      this->sampleRate(), maxDelayTime, feedbackFunc);

  delayLine->setBuffer(delayBuffer, bufsize);

  mCalcFunc = make_calc_function<FeedbackDelay, &FeedbackDelay::next>();
  next(1);
}

FeedbackDelay::~FeedbackDelay() { RTFree(this->mWorld, delayBuffer); }

void FeedbackDelay::next(int nSamples) {
  const float *input = in(AUDIOINPUT);
  float *outbuf = out(0);
  auto slopedDelayTime = makeSlope(in0(DELAYTIME), delayTimePast);
  auto slopedFeedback = makeSlope(in0(FEEDBACK), feedbackPast);

  for (int i = 0; i < nSamples; ++i) {
    float delayTime =
        isDelayTimeAudioRate ? in(DELAYTIME)[i] : slopedDelayTime.consume();
    float feedback =
        isFeedbackAudioRate ? in(FEEDBACK)[i] : slopedFeedback.consume();

    outbuf[i] = delayLine->process(input[i], delayTime, feedback);
  }

  delayTimePast =
      isDelayTimeAudioRate ? in(DELAYTIME)[nSamples - 1] : delayTimePast;
  feedbackPast =
      isFeedbackAudioRate ? in(FEEDBACK)[nSamples - 1] : feedbackPast;
}

} // namespace FeedbackDelay

PluginLoad(FeedbackDelayUGens) {
  ft = inTable;
  registerUnit<FeedbackDelay::FeedbackDelay>(ft, "FeedbackDelay", false);
}
