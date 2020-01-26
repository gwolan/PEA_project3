#pragma once

#include <vector>
#include <cstdint>
#include <memory>
#include <Miscellanous/Timer.hpp>
#include <Algorithms/Genetic/GreedyPathMutator.hpp>
#include <Algorithms/Genetic/Utils/GeneticConfiguration.hpp>


class Genetic
{
    private:
    using PathWithCost = std::pair<uint32_t, std::vector<uint32_t>>;

    public:
    Genetic(const GeneticConfiguration& geneticConfig);
    ~Genetic() = default;

    PathWithCost findBestPossibleRoute(std::unique_ptr<GraphMatrix>& graphMatrix);
    void reset();


    private:
    bool isTimeUp();
    void assignNewBestSolutionIfPossible(PathWithCost& bestPath);
    uint32_t calculatePathsCost(const std::vector<uint32_t>& path);
    void generateStartingPopulation();
    std::pair<PathWithCost, PathWithCost> generateSiblings(std::vector<uint32_t>& firstParent,
                                                           std::vector<uint32_t>& secondParent);
    std::pair<uint32_t, uint32_t> rollRange();
    std::pair<std::vector<uint32_t>::iterator,
              std::vector<uint32_t>::iterator> convertIndexesToIterators(std::vector<uint32_t>& path, uint32_t beginIndex,
                                                                                                      uint32_t endIndex);
    std::pair<PathWithCost, PathWithCost> convertSiblingsToResult(std::pair<std::vector<uint32_t>,
                                                                            std::vector<uint32_t>>& siblings);
    template<class Iterator>
    bool wasVertexAlreadyChecked(Iterator begin, Iterator end, const uint32_t vertex);
    bool isGivenPathPromising(const uint32_t currentCost, const uint32_t upperBound);
    void fillRestOfTheSiblingPathDuringOX(std::pair<std::vector<uint32_t>::iterator, std::vector<uint32_t>::iterator>& parentIterators,
                                          std::pair<std::vector<uint32_t>::iterator, std::vector<uint32_t>::iterator>& siblingIterators,
                                          std::vector<uint32_t>& parent,
                                          std::vector<uint32_t>& sibling);

    Timer timer;
    GreedyPathMutator greedyPathMutator;
    const GeneticConfiguration& geneticConfiguration;

    PathWithCost bestSolution;
    std::vector<PathWithCost> population;

    const int32_t INFINITE_VALUE;
    std::unique_ptr<GraphMatrix>* graph;
};
