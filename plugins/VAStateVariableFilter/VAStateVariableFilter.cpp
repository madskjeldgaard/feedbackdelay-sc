#include "VAStateVariableFilter.hpp"
#include "SC_PlugIn.hpp"

static InterfaceTable *ft;

namespace VAStateVariableFilter {

VAStateVariableFilter::VAStateVariableFilter() {
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

  mCalcFunc =
      make_calc_function<VAStateVariableFilter, &VAStateVariableFilter::next>();
  next(1);
}

VAStateVariableFilter::~VAStateVariableFilter() {}

void VAStateVariableFilter::next(int nSamples) {
  const float *input = in(AUDIOINPUT);
  float *outbuf = out(0);

  auto slopedCutoff = makeSlope(in0(CUTOFF), cutoffPast);
  auto slopedResonance = makeSlope(in0(RESONANCE), resPast);
  filterType = in0(FILTER_TYPE);

  filter->setFilterType(filterType);

  for (int i = 0; i < nSamples; ++i) {
    float cutoff = isCutoffAudioRate ? in(CUTOFF)[i] : slopedCutoff.consume();
    float resonance =
        isResAudioRate ? in(RESONANCE)[i] : slopedResonance.consume();

    filter->setCutoffFreq(cutoff);
    filter->setResonance(resonance);

    outbuf[i] = filter->processAudioSample(input[i]);
  }

  cutoffPast =
      isCutoffAudioRate ? in(CUTOFF)[nSamples - 1] : slopedCutoff.value;
  resPast =
      isResAudioRate ? in(RESONANCE)[nSamples - 1] : slopedResonance.value;
}

} // namespace VAStateVariableFilter

PluginLoad(VAStateVariableFilterUGens) {
  ft = inTable;
  registerUnit<VAStateVariableFilter::VAStateVariableFilter>(
      ft, "VAStateVariableFilter", false);
}
