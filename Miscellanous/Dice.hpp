#pragma once

#include <random>
#include <cstdint>


class Dice
{
    public:
    Dice(uint32_t lowerBound, uint32_t upperBound);

    uint32_t roll();


    private:
    std::random_device randomDevice;
    std::default_random_engine randomEngine;
    std::uniform_int_distribution<uint32_t> uniformIntDistribution;
};
