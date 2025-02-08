// PluginVAStateVariableFilter.hpp
// Mads Kjeldgaard (madskjeldgaardnielsen@gmail.com)

// This was originally inspired by
// https://github.com/supercollider/example-plugins

#pragma once

#include "../VASVF.hpp"
#include "SC_PlugIn.hpp"
#include <memory>

namespace VAStateVariableFilter {

class VAStateVariableFilter : public SCUnit {
public:
  VAStateVariableFilter();
  ~VAStateVariableFilter();

private:
  void next(int nSamples);

  enum paramNames {
    AUDIOINPUT,
    CUTOFF,
    RESONANCE,
    FILTER_TYPE,
  };

  float cutoffPast, resPast;
  int filterType;
  bool isCutoffAudioRate;
  bool isResAudioRate;

  std::unique_ptr<VASVF> filter;
};

} // namespace VAStateVariableFilter
;
