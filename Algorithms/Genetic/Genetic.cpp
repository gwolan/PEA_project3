#include <Algorithms/Genetic/Genetic.hpp>
#include <Graph/GraphMatrix.hpp>


Genetic::Genetic(const GeneticConfiguration& geneticConfig)
    : timer()
    , greedyGenerationCreator()
    , geneticConfiguration(geneticConfig)
{ }

Genetic::PathWithCost Genetic::findBestPossibleRoute(std::unique_ptr<GraphMatrix>& graphMatrix)
{
    return PathWithCost();
}

bool Genetic::isTimeUp()
{
    timer.stop();

    return timer.getTime() > geneticConfiguration.getGeneticInterval();
}
