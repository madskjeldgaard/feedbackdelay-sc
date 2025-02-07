// PluginFeedbackDelay.cpp
// Mads Kjeldgaard (madskjeldgaardnielsen@gmail.com)

#include "SC_PlugIn.hpp"
#include "FeedbackDelay.hpp"

static InterfaceTable* ft;

namespace FeedbackDelay {

FeedbackDelay::FeedbackDelay() {
    mCalcFunc = make_calc_function<FeedbackDelay, &FeedbackDelay::next>();
    next(1);
}

void FeedbackDelay::next(int nSamples) {

    // Audio rate input
    const float* input = in(0);

    // Control rate parameter: gain.
    const float gain = in0(1);

    // Output buffer
    float* outbuf = out(0);

    // simple gain function
    for (int i = 0; i < nSamples; ++i) {
        outbuf[i] = input[i] * gain;
    }
}

} // namespace FeedbackDelay

PluginLoad(FeedbackDelayUGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<FeedbackDelay::FeedbackDelay>(ft, "FeedbackDelay", false);
}
