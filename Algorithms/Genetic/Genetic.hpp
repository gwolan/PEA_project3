#pragma once

#include <vector>
#include <cstdint>
#include <memory>
#include <Miscellanous/Timer.hpp>
#include <Algorithms/Genetic/GreedyGenerationCreator.hpp>
#include <Algorithms/Genetic/Utils/GeneticConfiguration.hpp>


class Genetic
{
    private:
    using PathWithCost = std::pair<uint32_t, std::vector<uint32_t>>;

    public:
    Genetic(const GeneticConfiguration& geneticConfig);
    ~Genetic() = default;

    PathWithCost findBestPossibleRoute(std::unique_ptr<GraphMatrix>& graphMatrix);


    private:
    bool isTimeUp();

    Timer timer;
    GreedyGenerationCreator greedyGenerationCreator;
    const GeneticConfiguration& geneticConfiguration;
};
