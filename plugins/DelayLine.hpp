#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <memory>

template <typename FeedbackFunc> class DelayLine {
public:
  DelayLine(float sampleRate, float maxDelayTime, FeedbackFunc feedbackFunc)
      : sampleRate(sampleRate), maxDelayTime(maxDelayTime),
        feedbackFunc(feedbackFunc) {}

  // The buffer has to be handled and allocated by SuperCollider to make it
  // realtime safe
  void setBuffer(float *delayBuffer, int bufferSize) {
    bufsize = bufferSize;
    buf = delayBuffer;
    mask = bufsize - 1;
    memset(delayBuffer, bufsize, 0.f);
  }

  auto process(float input, float delayTime, float feedback) {
    delayTime = std::min(delayTime, maxDelayTime);
    auto delaySamples = sampleRate * delayTime;
    auto offset = static_cast<int>(delaySamples);
    auto frac = delaySamples - static_cast<float>(offset);

    std::array<int, 4> phases = {writePhase + 1 - offset, writePhase - offset,
                                 writePhase - 1 - offset,
                                 writePhase - 2 - offset};
    std::array<float, 4> delays = {buf[phases[0] & mask], buf[phases[1] & mask],
                                   buf[phases[2] & mask],
                                   buf[phases[3] & mask]};

    auto delayed =
        cubicinterp(frac, delays[0], delays[1], delays[2], delays[3]);
    auto feedbackSignal = feedbackFunc(feedback, delayed);
    auto delaySignal = input + feedbackSignal;

    buf[writePhase] = delaySignal;
    writePhase = (writePhase + 1) & mask;

    return zapgremlins(delaySignal);
  }

private:
  float sampleRate;
  float maxDelayTime;
  int bufsize;
  int mask;
  int writePhase = 0;
  float *buf;
  FeedbackFunc feedbackFunc;

  static float cubicinterp(float frac, float d0, float d1, float d2, float d3) {
    auto a0 = d3 - d2 - d0 + d1;
    auto a1 = d0 - d1 - a0;
    auto a2 = d2 - d0;
    auto a3 = d1;
    return ((a0 * frac + a1) * frac + a2) * frac + a3;
  }

  static float zapgremlins(float x) {
    if (!std::isfinite(x))
      return 0.0f;
    return x;
  }
};
