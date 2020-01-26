#include <iostream>
#include <vector>
#include <cstdint>
#include <Application/Menu/Actions/PerformBruteForce.hpp>
#include <Graph/GraphMatrix.hpp>


PerformBruteForce::PerformBruteForce(const std::string& actionName)
    : BaseAction(actionName)
    , graph(nullptr)
    , bruteForce()
    , timer()
{ }

void PerformBruteForce::init(std::unique_ptr<GraphMatrix>& graphMatrix)
{
    graph = &graphMatrix;
}

void PerformBruteForce::run()
{
    if(graph->get())
    {
        timer.start();
        std::vector<uint32_t> resultPath = bruteForce.performBruteForceOnGraph(*graph);
        timer.stop();

        printResults(resultPath);
    }
    else
    {
        std::cout << "Graf jest pusty!" << std::endl << std::endl;
    }
}

void PerformBruteForce::printResults(const std::vector<uint32_t>& resultPath)
{
    std::cout << "Algorytm wykonal sie w czasie: " << timer.getTime() << " [s]" << std::endl << std::endl;

    printPath(resultPath);
    printPathCost(resultPath);

    std::cout << std::endl;
}

void PerformBruteForce::printPath(const std::vector<uint32_t>& resultPath)
{
    std::cout << "Minimalna trasa: ";
    for(auto& vertex : resultPath)
    {
        std::cout << vertex << " ";

        if(&vertex != &resultPath.back())
        {
            std::cout << "-> ";
        }
    }
    std::cout << std::endl;
}

void PerformBruteForce::printPathCost(const std::vector<uint32_t>& resultPath)
{
    uint32_t pathCost = 0;

    for(auto vertexIt  = std::next(resultPath.cbegin()); 
             vertexIt != resultPath.cend(); 
           ++vertexIt)
    {
        pathCost += (*graph)->getWeight(*std::prev(vertexIt), *vertexIt);
    }
    std::cout << "Koszt trasy: " << pathCost << std::endl;
}
