#pragma once

#include <memory>
#include <Application/Menu/Actions/BaseAction.hpp>
#include <Algorithms/Genetic/Genetic.hpp>
#include <Algorithms/Genetic/Utils/GeneticConfiguration.hpp>
#include <Miscellanous/Timer.hpp>


class GraphMatrix;

class PerformGeneticAlgorithm : public BaseAction
{
    public:
    PerformGeneticAlgorithm(const std::string& actionName, GeneticConfiguration& geneticConfig);
    ~PerformGeneticAlgorithm() = default;

    void run();
    void init(std::unique_ptr<GraphMatrix>& graphMatrix);


    private:
    using PathWithCost = std::pair<uint32_t, std::vector<uint32_t>>;

    void printResults(const PerformGeneticAlgorithm::PathWithCost& result);
    void printPath(const std::vector<uint32_t>& resultPath);
    void printCost(const std::vector<uint32_t>& resultPath);

    std::unique_ptr<GraphMatrix>* graph;
    Genetic genetic;
    Timer timer;
};
