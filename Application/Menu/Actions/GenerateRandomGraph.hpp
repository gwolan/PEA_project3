#pragma once

#include <memory>
#include <Application/Menu/Actions/BaseAction.hpp>
#include <Miscellanous/Dice.hpp>


class GraphMatrix;

class GenerateRandomGraph : public BaseAction
{
    public:
    GenerateRandomGraph(const std::string& actionName);
    ~GenerateRandomGraph() = default;

    void run();
    void init(std::unique_ptr<GraphMatrix>& graphMatrix);


    private:
    void fillGraphAdjacencyMatrix();

    std::unique_ptr<GraphMatrix>* graph;
    Dice dice;
    uint32_t vertexCount;
};
