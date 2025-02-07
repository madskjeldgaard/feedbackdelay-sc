// PluginFeedbackDelay.hpp
// Mads Kjeldgaard (madskjeldgaardnielsen@gmail.com)

#pragma once

#include "SC_PlugIn.hpp"

namespace FeedbackDelay {

class FeedbackDelay : public SCUnit {
public:
    FeedbackDelay();

    // Destructor
    // ~FeedbackDelay();

private:
    // Calc function
    void next(int nSamples);

    // Member variables
};

} // namespace FeedbackDelay
