#include <Algorithms/Genetic/Utils/GeneticConfiguration.hpp>


// default values for genetic algorithm
GeneticConfiguration::GeneticConfiguration()
    : mutateEdges(true)
    , mutationCoefficient(0.01)
    , crossoverCoefficient(0.8)
    , geneticInterval(15.0)
    , populationSize(20)
{ }

void GeneticConfiguration::setGeneticInterval(double intervalInSeconds)
{
    geneticInterval = intervalInSeconds;
}

void GeneticConfiguration::enableEdgeMutation()
{
    mutateEdges = true;
}

void GeneticConfiguration::disableEdgeMutation()
{
    mutateEdges = false;
}

void GeneticConfiguration::setMutationCoefficient(double mutationCoeff)
{
    mutationCoefficient = mutationCoeff;
}

void GeneticConfiguration::setCrossoverCoefficient(double crossoverCoeff)
{
    crossoverCoefficient = crossoverCoeff;
}

void GeneticConfiguration::setPopulationSize(uint32_t population)
{
    populationSize = population;
}

bool GeneticConfiguration::isEdgeMutationEnabled() const
{
    return mutateEdges;
}

double GeneticConfiguration::getGeneticInterval() const
{
    return geneticInterval;
}

double GeneticConfiguration::getMutationCoefficient() const
{
    return mutationCoefficient;
}

double GeneticConfiguration::getCrossoverCoefficient() const
{
    return crossoverCoefficient;
}

uint32_t GeneticConfiguration::getPopulationSize() const
{
    return populationSize;
}
