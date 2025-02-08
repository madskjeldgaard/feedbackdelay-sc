#pragma once

// This is a rewrite of Jordan Harris' VaStateVariableFilter for JUCE:
// https://github.com/JordanTHarris/VAStateVariableFilter/blob/master/Source/Effects/VAStateVariableFilter.cpp

#include <cmath>

//==============================================================================

/** The type of filter that the State Variable Filter will output. */
enum SVFType {
  SVFLowpass = 0,
  SVFBandpass,
  SVFHighpass,
  SVFUnitGainBandpass,
  SVFBandShelving,
  SVFNotch,
  SVFAllpass,
  SVFPeak
};

//==============================================================================
class VASVF {
public:
  /** Create and initialize the filter with default values. */
  VASVF()
      : sampleRate(44100.0f), filterType(SVFLowpass), cutoffFreq(1000.0f),
        Q(resonanceToQ(0.5f)), shelfGain(0.0f), active(true), gCoeff(1.0f),
        RCoeff(1.0f), KCoeff(0.0f), z1(0.0f), z2(0.0f) {}

  //------------------------------------------------------------------------------

  /** Sets the type of the filter. */
  void setFilterType(int newType) { filterType = newType; }

  //------------------------------------------------------------------------------
  /** Sets the cutoff frequency using pitch (MIDI note #). */
  void setCutoffPitch(float newCutoffPitch) {
    if (active) {
      cutoffFreq = pitchToFreq(newCutoffPitch);
      calcFilter();
    }
  }

  //------------------------------------------------------------------------------
  /** Sets the cutoff frequency directly in Hz. */
  void setCutoffFreq(float newCutoffFreq) {
    if (active) {
      cutoffFreq = newCutoffFreq;
      calcFilter();
    }
  }

  //------------------------------------------------------------------------------
  /** Sets the resonance amount (range: 0-1). */
  void setResonance(float newResonance) {
    if (active) {
      Q = resonanceToQ(newResonance);
      calcFilter();
    }
  }

  //------------------------------------------------------------------------------
  /** Sets the Q amount directly. */
  void setQ(float newQ) {
    if (active) {
      Q = newQ;
      calcFilter();
    }
  }

  //------------------------------------------------------------------------------
  /** Sets the gain of the shelf for the BandShelving filter. */
  void setShelfGain(float newGain) {
    if (active) {
      shelfGain = newGain;
      calcFilter();
    }
  }

  //------------------------------------------------------------------------------
  /** Sets all filter parameters at once. */
  void setFilter(int newType, float newCutoffFreq, float newResonance,
                 float newShelfGain) {
    filterType = newType;
    cutoffFreq = newCutoffFreq;
    Q = resonanceToQ(newResonance);
    shelfGain = newShelfGain;
    calcFilter();
  }

  //------------------------------------------------------------------------------
  /** Sets the sample rate for the filter. */
  void setSampleRate(float newSampleRate) {
    sampleRate = newSampleRate;
    calcFilter();
  }

  //------------------------------------------------------------------------------
  /** Enables or disables the filter. */
  void setIsActive(bool isActive) { active = isActive; }

  //------------------------------------------------------------------------------
  /** Processes a single sample of audio. */
  float processAudioSample(float input) {
    if (active) {
      // Filter processing:
      const float HP = (input - (2.0f * RCoeff + gCoeff) * z1 - z2) /
                       (1.0f + (2.0f * RCoeff * gCoeff) + gCoeff * gCoeff);

      const float BP = HP * gCoeff + z1;

      const float LP = BP * gCoeff + z2;

      const float UBP = 2.0f * RCoeff * BP;

      const float BShelf = input + UBP * KCoeff;

      const float Notch = input - UBP;

      const float AP = input - (4.0f * RCoeff * BP);

      const float Peak = LP - HP;

      z1 = gCoeff * HP + BP; // unit delay (state variable)
      z2 = gCoeff * BP + LP; // unit delay (state variable)

      // Selects which filter type this function will output.
      switch (filterType) {
      case SVFLowpass:
        return LP;
      case SVFBandpass:
        return BP;
      case SVFHighpass:
        return HP;
      case SVFUnitGainBandpass:
        return UBP;
      case SVFBandShelving:
        return BShelf;
      case SVFNotch:
        return Notch;
      case SVFAllpass:
        return AP;
      case SVFPeak:
        return Peak;
      default:
        return 0.0f;
      }
    } else { // If not active, return input
      return input;
    }
  }

  //------------------------------------------------------------------------------
  /** Processes a block of audio samples. */
  void processAudioBlock(float *samples, int numSamples) {
    if (active) {
      for (int i = 0; i < numSamples; ++i) {
        samples[i] = processAudioSample(samples[i]);
      }
    }
  }

  //------------------------------------------------------------------------------

private:
  //==============================================================================
  // Calculate the coefficients for the filter based on parameters.
  void calcFilter() {
    if (active) {
      // prewarp the cutoff (for bilinear-transform filters)
      float wd = cutoffFreq * 2.0f * static_cast<float>(M_PI);
      float T = 1.0f / sampleRate;
      float wa = (2.0f / T) * std::tan(wd * T / 2.0f);

      // Calculate g (gain element of integrator)
      gCoeff = wa * T / 2.0f;

      // Calculate Zavalishin's R from Q (referred to as damping parameter)
      RCoeff = 1.0f / (2.0f * Q);

      // Gain for BandShelving filter
      KCoeff = shelfGain;
    }
  }

  // Convert resonance to Q factor
  static float resonanceToQ(float resonance) {
    return 1.0f / (2.0f * (1.0f - resonance));
  }

  // Convert pitch to frequency
  static float pitchToFreq(float pitch) {
    return 440.0f * std::pow(2.0f, (pitch - 69.0f) / 12.0f);
  }

  // Parameters:
  int filterType;
  float cutoffFreq;
  float Q;
  float shelfGain;

  float sampleRate;
  bool active = true; // is the filter processing or not

  // Coefficients:
  float gCoeff; // gain element
  float RCoeff; // feedback damping element
  float KCoeff; // shelf gain element

  // State variables (z^-1)
  float z1, z2;
};
