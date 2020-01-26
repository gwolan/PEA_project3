#pragma once

#include <memory>
#include <Graph/GraphMatrix.hpp>
#include <Application/Menu/Actions/BaseAction.hpp>
#include <Algorithms/TabuSearch/Utils/TabuConfiguration.hpp>


class ActionStrategy
{
    public:
    ActionStrategy(std::unique_ptr<GraphMatrix>& graphMatrix, TabuConfiguration& tabuConfig);

    void executeAction();
    bool selectAction(char selection);


    public:
    std::unique_ptr<GraphMatrix>& graph;
    std::unique_ptr<BaseAction> selectedAction;
    TabuConfiguration& tabuConfiguration;
};
