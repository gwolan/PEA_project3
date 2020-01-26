#pragma once

#include <memory>
#include <Application/Menu/Actions/BaseAction.hpp>
#include <Algorithms/BranchAndBound/BranchAndBound.hpp>
#include <Miscellanous/Timer.hpp>


class GraphMatrix;

class PerformBranchAndBound : public BaseAction
{
    public:
    PerformBranchAndBound(const std::string& actionName);
    ~PerformBranchAndBound() = default;

    void run();
    void init(std::unique_ptr<GraphMatrix>& graphMatrix);


    private:
    void printResults(const std::vector<uint32_t>& resultPath);
    void printPath(const std::vector<uint32_t>& resultPath);
    void printPathCost(const std::vector<uint32_t>& resultPath);

    std::unique_ptr<GraphMatrix>* graph;
    BranchAndBound branchAndBound;
    Timer timer;
};
