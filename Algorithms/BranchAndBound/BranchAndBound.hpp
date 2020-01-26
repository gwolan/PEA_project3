#pragma once

#include <vector>
#include <memory>
#include <cstdint>
#include <queue>
#include <Algorithms/BranchAndBound/Utils/GreaterPathComparator.hpp>


class GraphMatrix;

class BranchAndBound
{
    public:
    BranchAndBound();
    ~BranchAndBound() = default;

    std::vector<uint32_t> performBranchAndBoundOnGraph(std::unique_ptr<GraphMatrix>& graphMatrix);


    private:
    using PathQueue = std::priority_queue<std::vector<uint32_t>, std::vector<std::vector<uint32_t>>, GreaterPathComparator>;
    using BestSolutionHolder = std::pair<uint32_t, std::vector<uint32_t>>;

    // general algorithm steps
    void startAlgorithm(std::vector<uint32_t>& verticies,
                        std::vector<uint32_t>& currentBestPartialPath, PathQueue& pathQueue,
                                                                       BestSolutionHolder& bestSolutionHolder);

    void performCalculations(std::vector<uint32_t>& verticies,
                             std::vector<uint32_t>& currentBestPartialPath, PathQueue& pathQueue,
                                                                            BestSolutionHolder& bestSolutionHolder);

    void performNextIteration(std::vector<uint32_t>& verticies,
                              std::vector<uint32_t>& currentBestPartialPath,
                              std::vector<uint32_t>& newPartialPath);

    void finalizeCalculations(std::vector<uint32_t>& pathWithSolution, BestSolutionHolder& bestSolutionHolder);

    // helpers
    template<class Iterator>
    bool wasVertexAlreadyChecked(Iterator begin, Iterator end, const uint32_t vertex);
    bool isCurrentlyEvaluatedPathEnding(const std::vector<uint32_t>& evaluatedPath,
                                        const std::vector<uint32_t>& verticies);
    uint32_t calculatePathsCost(const std::vector<uint32_t>& path);
    void assignNewBestPathIfPossible(std::vector<uint32_t>& possibleBestPath, BestSolutionHolder& bestSolutionHolder);
    bool isGivenPathPromising(const uint32_t currentCost, const uint32_t upperBound);
    bool isEvaluatedEdgeClosingHamiltonCycle(const uint32_t edgeBegin, const uint32_t edgeEnd, const uint32_t lastVertexInPartialPath);
    bool doVerticiesCreateEdge(const uint32_t edgeBegin, const uint32_t edgeEnd);


    std::unique_ptr<GraphMatrix>* graph;
    const int32_t INFINITY;
    const int32_t STARTING_VERTEX;
};
