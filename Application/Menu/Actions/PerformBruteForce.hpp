#pragma once

#include <memory>
#include <Application/Menu/Actions/BaseAction.hpp>
#include <Algorithms/BruteForce/BruteForce.hpp>
#include <Miscellanous/Timer.hpp>


class GraphMatrix;

class PerformBruteForce : public BaseAction
{
    public:
    PerformBruteForce(const std::string& actionName);
    ~PerformBruteForce() = default;

    void run();
    void init(std::unique_ptr<GraphMatrix>& graphMatrix);


    private:
    void printResults(const std::vector<uint32_t>& resultPath);
    void printPath(const std::vector<uint32_t>& resultPath);
    void printPathCost(const std::vector<uint32_t>& resultPath);

    std::unique_ptr<GraphMatrix>* graph;
    BruteForce bruteForce;
    Timer timer;
};
