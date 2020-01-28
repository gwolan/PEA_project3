#pragma once

#include <cstdint>


struct GeneticMeasureResult
{
    GeneticMeasureResult();
    ~GeneticMeasureResult() = default;

    bool inversionMutation;
    bool bestPopulationResults;
    bool constantCrossover;
    bool constantMutation;
    double timeInSeconds;
    double relativeError;
    uint32_t pathCost;
    uint32_t populationSize;
};
