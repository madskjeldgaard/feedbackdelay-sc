#pragma once

#include <algorithm>
#include <array>
#include <functional>
#include <memory>
#include <cmath>

template <typename FeedbackFunc>
class DelayLine {
public:
    DelayLine(float sampleRate, float maxDelayTime, FeedbackFunc feedbackFunc)
        : sampleRate(sampleRate), maxDelayTime(maxDelayTime), feedbackFunc(feedbackFunc) {
        bufsize = nextPowerOfTwo(sampleRate * maxDelayTime);
        mask = bufsize - 1;
        buf = std::make_unique<float[]>(bufsize);
        std::fill(buf.get(), buf.get() + bufsize, 0.0f);
    }

    float process(float input, float delayTime, float feedback) {
        delayTime = std::min(delayTime, maxDelayTime);
        float delaySamples = sampleRate * delayTime;
        int offset = static_cast<int>(delaySamples);
        float frac = delaySamples - offset;

        std::array<int, 4> phases = { writePhase + 1 - offset, writePhase - offset, writePhase - 1 - offset, writePhase - 2 - offset };
        std::array<float, 4> delays = { buf[phases[0] & mask], buf[phases[1] & mask], buf[phases[2] & mask], buf[phases[3] & mask] };

        float delayed = cubicinterp(frac, delays[0], delays[1], delays[2], delays[3]);
        float feedbackSignal = feedbackFunc(feedback, delayed);
        float delaySignal = input + feedbackSignal;

        buf[writePhase] = delaySignal;
        writePhase = (writePhase + 1) & mask;

        return zapgremlins(delaySignal);
    }

private:

    static int nextPowerOfTwo(float x) {
        return std::pow(2, std::ceil(std::log2(x)));
    }

    float sampleRate;
    float maxDelayTime;
    int bufsize;
    int mask;
    int writePhase = 0;
    std::unique_ptr<float[]> buf;
    FeedbackFunc feedbackFunc;

    static float cubicinterp(float frac, float d0, float d1, float d2, float d3) {
        float a0 = d3 - d2 - d0 + d1;
        float a1 = d0 - d1 - a0;
        float a2 = d2 - d0;
        float a3 = d1;
        return ((a0 * frac + a1) * frac + a2) * frac + a3;
    }

    static float zapgremlins(float x) {
        if (!std::isfinite(x)) return 0.0f;
        return x;
    }
};
