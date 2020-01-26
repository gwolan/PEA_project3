#include <Algorithms/TabuSearch/Utils/TabuConfiguration.hpp>


// default values for Tabu Search algorithm
TabuConfiguration::TabuConfiguration()
    : tabuInterval(60.0)
    , tabuDiversification(true)
    , tabuNumberOfIterations(10000)
    , tabuCadencyLength(0)
{ }

void TabuConfiguration::setTabuInterval(double intervalInSeconds)
{
    tabuInterval = intervalInSeconds;
}

void TabuConfiguration::enableDiversification()
{
    tabuDiversification = true;
}

void TabuConfiguration::disableDiversification()
{
    tabuDiversification = false;
}

void TabuConfiguration::setDiversificationCriteria(uint32_t numberOfIterations)
{
    tabuNumberOfIterations = numberOfIterations;
}

void TabuConfiguration::setTabuCadencyLength(uint32_t cadency)
{
    tabuCadencyLength = cadency;
}

bool TabuConfiguration::isDiversificationEnabled() const
{
    return tabuDiversification;
}

double TabuConfiguration::getTabuInterval() const
{
    return tabuInterval;
}

uint32_t TabuConfiguration::getNumberOfIterations() const
{
    return tabuNumberOfIterations;
}

uint32_t TabuConfiguration::getCadencyLength() const
{
    return tabuCadencyLength;
}
