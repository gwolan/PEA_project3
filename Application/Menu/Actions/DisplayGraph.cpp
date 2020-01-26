#include <iostream>
#include <Application/Menu/Actions/DisplayGraph.hpp>
#include <Graph/GraphMatrix.hpp>


DisplayGraph::DisplayGraph(const std::string& actionName)
    : BaseAction(actionName)
    , graph(nullptr)
{ }

void DisplayGraph::init(std::unique_ptr<GraphMatrix>& graphMatrix)
{
    graph = &graphMatrix;
}

void DisplayGraph::run()
{
    if(graph->get())
    {
        (*graph)->displayGraph();
    }
    else
    {
        std::cout << "Graf jest pusty!" << std::endl;
    }

    std::cout << std::endl;
}
