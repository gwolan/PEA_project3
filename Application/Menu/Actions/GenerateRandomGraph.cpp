#include <iostream>
#include <Application/Menu/Actions/GenerateRandomGraph.hpp>
#include <Graph/GraphMatrix.hpp>


GenerateRandomGraph::GenerateRandomGraph(const std::string& actionName)
    : BaseAction(actionName)
    , graph(nullptr)
    , dice(1, 999)
    , vertexCount(0)
{ }

void GenerateRandomGraph::init(std::unique_ptr<GraphMatrix>& graphMatrix)
{
    graph = &graphMatrix;

    std::cout << "Podaj liczbe wezlow: ";
    std::cin >> vertexCount;
}

void GenerateRandomGraph::run()
{
    *graph = std::make_unique<GraphMatrix>(vertexCount);
    fillGraphAdjacencyMatrix();
}

void GenerateRandomGraph::fillGraphAdjacencyMatrix()
{
    for(std::size_t row = 0; row < vertexCount; ++row)
    {
        for(std::size_t column = 0; column < vertexCount; ++column)
        {
            uint32_t weight = dice.roll();
            if(row != column)
            {
                (*graph)->addWeightToMatrix(row, column, weight);
            }
        }
    }
}
