#include <Algorithms/Genetic/Utils/GeneticConfiguration.hpp>


// default values for genetic algorithm
GeneticConfiguration::GeneticConfiguration()
    : inversionMutation(true)
    , mutationCoefficient(0.01)
    , crossoverCoefficient(0.8)
    , geneticInterval(15.0)
    , populationSize(20)
{ }

void GeneticConfiguration::setGeneticInterval(double intervalInSeconds)
{
    geneticInterval = intervalInSeconds;
}

void GeneticConfiguration::enableInversionMutation()
{
    inversionMutation = true;
}

void GeneticConfiguration::disableInversionMutation()
{
    inversionMutation = false;
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

bool GeneticConfiguration::isInversionMutationEnabled() const
{
    return inversionMutation;
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
