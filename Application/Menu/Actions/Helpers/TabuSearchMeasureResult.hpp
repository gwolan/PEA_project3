#pragma once

#include <cstdint>


struct TabuSearchMeasureResult
{
    TabuSearchMeasureResult() = default;
    ~TabuSearchMeasureResult() = default;

    double timeInSeconds;
    double relativeError;
    uint32_t pathCost;
    bool diversification;
};
