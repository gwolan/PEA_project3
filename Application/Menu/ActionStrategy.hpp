#pragma once

#include <memory>
#include <Graph/GraphMatrix.hpp>
#include <Application/Menu/Actions/BaseAction.hpp>
#include <Algorithms/TabuSearch/Utils/TabuConfiguration.hpp>
#include <Algorithms/Genetic/Utils/GeneticConfiguration.hpp>


class ActionStrategy
{
    public:
    ActionStrategy(std::unique_ptr<GraphMatrix>& graphMatrix, TabuConfiguration& tabuConfig, GeneticConfiguration& geneticConfig);

    void executeAction();
    bool selectAction(std::string choice);


    public:
    std::unique_ptr<GraphMatrix>& graph;
    std::unique_ptr<BaseAction> selectedAction;
    TabuConfiguration& tabuConfiguration;
    GeneticConfiguration& geneticConfiguration;
};
