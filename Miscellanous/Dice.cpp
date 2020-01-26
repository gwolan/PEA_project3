#include <Miscellanous/Dice.hpp>


Dice::Dice(uint32_t lowerBound, uint32_t upperBound)
    : randomDevice()
    , randomEngine(randomDevice())
    , uniformIntDistribution(lowerBound, upperBound)
{ }

uint32_t Dice::roll()
{
    return uniformIntDistribution(randomEngine);
}
