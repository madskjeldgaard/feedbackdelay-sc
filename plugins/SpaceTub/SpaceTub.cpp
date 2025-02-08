#include "SpaceTub.hpp"
#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace SpaceTub {

SpaceTub::SpaceTub() {
  // Clipper
  useSoftClip = static_cast<bool>(in0(USE_SOFTCLIP));
  clipper = std::make_unique<SoftClipper>();

  // Limiter
  limiterOn = static_cast<bool>(in0(LIMITER_ON));
  limiter = std::make_unique<safety_limiter::SafetyLimiter>(this->sampleRate());
  limiter->setReleaseTime(in0(LIMITER_RELEASE));
  limiter->setHoldTime(in0(LIMITER_HOLDTIME));

  // DC Filter
  dcFilter = std::make_unique<DCFilter>();

  // Setup filters
  isCutoffAudioRate = isAudioRateIn(CUTOFF);
  isResAudioRate = isAudioRateIn(RESONANCE);

  cutoffPast = in0(CUTOFF);
  resPast = in0(RESONANCE);
  filterType = in0(FILTER_TYPE);

  filter = std::make_unique<VASVF>();

  filter->setSampleRate(this->sampleRate());
  filter->setIsActive(true);
  filter->setFilterType(filterType);
  filter->setCutoffFreq(cutoffPast);
  filter->setResonance(resPast);

  // Setup delays
  delayTimePast = in0(DELAYTIME);
  feedbackPast = in0(FEEDBACK);

  float maxDelayTime = in0(MAX_DELAYTIME);
  isDelayTimeAudioRate = isAudioRateIn(DELAYTIME);
  isFeedbackAudioRate = isAudioRateIn(FEEDBACK);

  // Allocate buffer
  bufsize = NEXTPOWEROFTWO(this->sampleRate() * maxDelayTime);
  delayBuffer = (float *)RTAlloc(this->mWorld, bufsize * sizeof(float));

  if (delayBuffer == nullptr) {
    Print("SpaceTub: Could not allocate delay buffer\n");
    ClearUnitOutputs(this, 1);

    mCalcFunc = ft->fClearUnitOutputs;
    return;
  }

  // Just pass through the audio with an amount of feedback
  auto feedbackFunc = [this](float feedbackAmount, float delayed) {
    // DC Filter
    delayed = dcFilter->process(delayed);

    // Filter
    delayed = filter->processAudioSample(delayed);

    // Soft Clip
    if (useSoftClip) {
      delayed = clipper->process(delayed);
    }

    // Limiter
    if (limiterOn) {
      delayed = limiter->process(delayed);
    }

    return (feedbackAmount * delayed);
  };

  delayLine = std::make_unique<DelayLine<FeedbackFunc>>(
      this->sampleRate(), maxDelayTime, feedbackFunc);

  memset(delayBuffer, bufsize, 0.f);
  delayLine->setBuffer(delayBuffer, bufsize);

  mCalcFunc = make_calc_function<SpaceTub, &SpaceTub::next>();
  next(1);
}

SpaceTub::~SpaceTub() { RTFree(this->mWorld, delayBuffer); }

void SpaceTub::next(int nSamples) {
  const float *input = in(AUDIOINPUT);
  float *outbuf = out(0);
  auto slopedDelayTime = makeSlope(in0(DELAYTIME), delayTimePast);
  auto slopedFeedback = makeSlope(in0(FEEDBACK), feedbackPast);
  auto slopedCutoff = makeSlope(in0(CUTOFF), cutoffPast);
  auto slopedResonance = makeSlope(in0(RESONANCE), resPast);
  auto slopedLimiterRelease = makeSlope(in0(LIMITER_RELEASE), limiterRelease);
  auto slopedLimiterHoldTime =
      makeSlope(in0(LIMITER_HOLDTIME), limiterHoldTime);

  filterType = in0(FILTER_TYPE);
  filter->setFilterType(filterType);

  limiterOn = static_cast<bool>(in0(LIMITER_ON));

  useSoftClip = static_cast<bool>(in0(USE_SOFTCLIP));

  for (int i = 0; i < nSamples; ++i) {

    limiter->setReleaseTime(slopedLimiterRelease.consume());
    limiter->setHoldTime(slopedLimiterHoldTime.consume());

    float delayTime =
        isDelayTimeAudioRate ? in(DELAYTIME)[i] : slopedDelayTime.consume();
    float feedback =
        isFeedbackAudioRate ? in(FEEDBACK)[i] : slopedFeedback.consume();
    float cutoff = isCutoffAudioRate ? in(CUTOFF)[i] : slopedCutoff.consume();
    float resonance =
        isResAudioRate ? in(RESONANCE)[i] : slopedResonance.consume();

    filter->setCutoffFreq(cutoff);
    filter->setResonance(resonance);

    outbuf[i] = delayLine->process(input[i], delayTime, feedback);
  }

  delayTimePast =
      isDelayTimeAudioRate ? in(DELAYTIME)[nSamples - 1] : delayTimePast;
  feedbackPast =
      isFeedbackAudioRate ? in(FEEDBACK)[nSamples - 1] : feedbackPast;
  cutoffPast =
      isCutoffAudioRate ? in(CUTOFF)[nSamples - 1] : slopedCutoff.value;
  resPast =
      isResAudioRate ? in(RESONANCE)[nSamples - 1] : slopedResonance.value;
}

} // namespace SpaceTub

PluginLoad(SpaceTubUGens) {
  ft = inTable;
  registerUnit<SpaceTub::SpaceTub>(ft, "SpaceTub", false);
}
