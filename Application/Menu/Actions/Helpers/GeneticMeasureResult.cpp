#include <Application/Menu/Actions/Helpers/GeneticMeasureResult.hpp>


GeneticMeasureResult::GeneticMeasureResult()
    : inversionMutation(false)
    , bestPopulationResults(false)
    , constantCrossover(false)
    , constantMutation(false)
    , timeInSeconds(0.0)
    , relativeError(0.0)
    , pathCost(0)
    , populationSize(0)
{ }
