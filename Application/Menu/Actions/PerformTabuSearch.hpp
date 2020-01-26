#pragma once

#include <memory>
#include <Application/Menu/Actions/BaseAction.hpp>
#include <Algorithms/TabuSearch/TabuSearch.hpp>
#include <Algorithms/TabuSearch/Utils/TabuConfiguration.hpp>
#include <Miscellanous/Timer.hpp>


class GraphMatrix;

class PerformTabuSearch : public BaseAction
{
    public:
    PerformTabuSearch(const std::string& actionName, TabuConfiguration& tabuConfig);
    ~PerformTabuSearch() = default;

    void run();
    void init(std::unique_ptr<GraphMatrix>& graphMatrix);


    private:
    using PathWithCost = std::pair<uint32_t, std::vector<uint32_t>>;

    uint32_t getTabuCadencyForGraph(const uint32_t verticiesCount);
    void printResults(const PerformTabuSearch::PathWithCost& result);
    void printPath(const std::vector<uint32_t>& resultPath);
    void printCost(const std::vector<uint32_t>& resultPath);

    std::unique_ptr<GraphMatrix>* graph;
    TabuSearch tabuSearch;
    Timer timer;
    TabuConfiguration& tabuConfiguration;
};
