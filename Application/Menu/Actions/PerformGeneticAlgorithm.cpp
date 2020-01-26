#include <iostream>
#include <vector>
#include <cstdint>
#include <Application/Menu/Actions/PerformGeneticAlgorithm.hpp>
#include <Graph/GraphMatrix.hpp>


PerformGeneticAlgorithm::PerformGeneticAlgorithm(const std::string& actionName, GeneticConfiguration& geneticConfig)
    : BaseAction(actionName)
    , graph(nullptr)
    , genetic(geneticConfig)
    , timer()
{ }

void PerformGeneticAlgorithm::init(std::unique_ptr<GraphMatrix>& graphMatrix)
{
    graph = &graphMatrix;
}

void PerformGeneticAlgorithm::run()
{
    if(graph->get())
    {
        timer.start();
        PathWithCost result = genetic.findBestPossibleRoute(*graph);
        timer.stop();

        printResults(result);
    }
    else
    {
        std::cout << "Graf jest pusty!" << std::endl << std::endl;
    }
}

void PerformGeneticAlgorithm::printResults(const PerformGeneticAlgorithm::PathWithCost& result)
{
    std::cout << "Algorytm wykonal sie w czasie: " << timer.getTime() << " [s]" << std::endl << std::endl;

    printPath(result.second);
    printCost(result.second);
    std::cout << "Sumaryczny koszt trasy: " << result.first << std::endl;

    std::cout << std::endl;
}

void PerformGeneticAlgorithm::printPath(const std::vector<uint32_t>& resultPath)
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

void PerformGeneticAlgorithm::printCost(const std::vector<uint32_t>& resultPath)
{
    std::cout << "Koszt trasy: ";
    for(auto it = resultPath.begin(); it != resultPath.end(); ++it)
    {
        if(it == resultPath.begin())
        {
            std::cout << 0 << " -> ";
        }
        else if(std::next(it) != resultPath.end())
        {
            std::cout << (*graph)->getWeight(*it, *std::next(it)) << " -> ";
        }
        else
        {
            std::cout << (*graph)->getWeight(*it, *resultPath.begin());
        }
    }

    std::cout << std::endl;
}
