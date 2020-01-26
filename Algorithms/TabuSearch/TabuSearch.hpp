#pragma once

#include <Algorithms/TabuSearch/LocalSearch.hpp>
#include <Algorithms/TabuSearch/Utils/TabuConfiguration.hpp>
#include <Algorithms/TabuSearch/Utils/TabuMove.hpp>
#include <Miscellanous/Timer.hpp>


class GraphMatrix;

class TabuSearch
{
    private:
    using PathWithCost = std::pair<uint32_t, std::vector<uint32_t>>;

    public:
    TabuSearch(const TabuConfiguration& tabuConfig);
    ~TabuSearch() = default;

    PathWithCost findBestPossibleRoute(std::unique_ptr<GraphMatrix>& graphMatrix);
    void reset();


    private:
    void runTabuSearch(std::vector<uint32_t>& promisingPath);
    void intensify(std::vector<uint32_t>& startingPath);
    PathWithCost findBestNeighbourPathUsingSwap(const std::vector<uint32_t>& promisingPath, TabuMove& tabuMove);

    bool isGivenMoveTabu(const TabuMove& tabuMove);
    bool isAspirationCriteriaFulfilled(const std::vector<uint32_t>& neighbourPath);
    bool isCurrentNeighbourPathValid(const PathWithCost& bestNeighbourhood);
    bool shouldAlgorithmStop();
    bool wereIterationsExceeded();
    bool isTimeUp();
    bool isGivenPathPromising(const uint32_t currentCost, const uint32_t upperBound);

    uint32_t calculatePathsCost(const std::vector<uint32_t>& path);
    void enableIntesification();
    void disableIntesification();
    void updateTabuList(TabuMove& newTabuMove);
    void assignNewBestSolutionIfPossible(PathWithCost& bestNeighbourhood);
    void assignNewBestNeighbourhoodIfPossible(const std::vector<uint32_t>& neighbourPath,
                                             PathWithCost& bestNeighbourhood, TabuMove& tabuMove, const TabuMove& currentMove);

    LocalSearch localSearch;
    Timer timer;
    const int32_t INFINITY;
    std::unique_ptr<GraphMatrix>* graph;

    uint32_t iterationsWithoutImprovements;
    std::vector<TabuMove> tabuList;
    const TabuConfiguration& tabuConfiguration;
    PathWithCost bestSolution;
    bool intensification;
    uint32_t cadencyDivisor;
};
