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

    // algorithm
    void breedCurrentPopulation(std::vector<PathWithCost>& newGeneration);
    void mutateNewGeneration(std::vector<PathWithCost>& newGeneration);
    void inversionMutation(std::vector<PathWithCost>& newGeneration);
    void scrambleMutation(std::vector<PathWithCost>& newGeneration);
    void addNewGenerationToCurrentPopulation(std::vector<PathWithCost>& newGeneration);
    void generateStartingPopulation();
    void cutOffWeakPopulationMembers();
    bool shouldCrossoverHappen();
    bool shouldMutationHappen();
    uint32_t getReproductorsCount();
    std::pair<PathWithCost, PathWithCost> generateSiblings(std::vector<uint32_t>& firstParent,
                                                           std::vector<uint32_t>& secondParent);
    void fillRestOfTheSiblingPathDuringOX(std::pair<std::vector<uint32_t>::iterator, std::vector<uint32_t>::iterator>& parentIterators,
                                          std::pair<std::vector<uint32_t>::iterator, std::vector<uint32_t>::iterator>& siblingIterators,
                                          std::vector<uint32_t>& parent,
                                          std::vector<uint32_t>& sibling);
    std::pair<uint32_t, uint32_t> rollRange(uint32_t lowerBound, uint32_t upperBound);
    std::pair<std::vector<uint32_t>::iterator,
              std::vector<uint32_t>::iterator> convertIndexesToIterators(std::vector<uint32_t>& path, uint32_t beginIndex,
                                                                                                      uint32_t endIndex);
    std::pair<PathWithCost, PathWithCost> convertSiblingsToResult(std::pair<std::vector<uint32_t>,
                                                                            std::vector<uint32_t>>& siblings);

    // helpers
    template<class Iterator>
    bool wasVertexAlreadyChecked(Iterator begin, Iterator end, const uint32_t vertex);
    uint32_t calculatePathsCost(const std::vector<uint32_t>& path);
    bool isGivenPathPromising(const uint32_t currentCost, const uint32_t upperBound);


    Timer timer;
    GreedyPathMutator greedyPathMutator;
    const GeneticConfiguration& geneticConfiguration;

    PathWithCost bestSolution;
    std::vector<PathWithCost> population;

    const int32_t INFINITE_VALUE;
    std::unique_ptr<GraphMatrix>* graph;
};
