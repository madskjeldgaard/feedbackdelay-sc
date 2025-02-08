#pragma once
#include <cmath> // for std::tanh

class SoftClipper {
public:
  SoftClipper(float threshold = 1.0f, float gain = 1.0f)
      : threshold(threshold), gain(gain) {}

  void setThreshold(float threshold) { this->threshold = threshold; }

  void setGain(float gain) { this->gain = gain; }

  float process(float input) {
    // Apply gain to the input signal
    float amplified = input * gain;

    // Apply soft clipping using a hyperbolic tangent (tanh) function
    float clipped = threshold * std::tanh(amplified / threshold);

    return clipped;
  }

private:
  float threshold; // Soft clipping threshold (where the curve starts to round
                   // off)
  float gain;      // Input gain (amplifies the signal before clipping)
};
