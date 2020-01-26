#include <iostream>
#include <vector>
#include <cstdint>
#include <Application/Menu/Actions/PerformTabuSearch.hpp>
#include <Graph/GraphMatrix.hpp>


PerformTabuSearch::PerformTabuSearch(const std::string& actionName, TabuConfiguration& tabuConfig)
    : BaseAction(actionName)
    , graph(nullptr)
    , tabuSearch(tabuConfig)
    , timer()
    , tabuConfiguration(tabuConfig)
{ }

void PerformTabuSearch::init(std::unique_ptr<GraphMatrix>& graphMatrix)
{
    graph = &graphMatrix;
}

void PerformTabuSearch::run()
{
    if(graph->get())
    {
        if(tabuConfiguration.getCadencyLength() == 0)
        {
            tabuConfiguration.setTabuCadencyLength(getTabuCadencyForGraph((*graph)->getVertexCount()));
        }

        timer.start();
        PathWithCost result = tabuSearch.findBestPossibleRoute(*graph);
        timer.stop();

        printResults(result);
    }
    else
    {
        std::cout << "Graf jest pusty!" << std::endl << std::endl;
    }
}

uint32_t PerformTabuSearch::getTabuCadencyForGraph(const uint32_t verticiesCount)
{
    uint32_t tabuCadency = ((2 * verticiesCount) / 10) * 10;

    if(tabuCadency != 0)
    {
        return tabuCadency;
    }
    else
    {
        return 10;
    }
}

void PerformTabuSearch::printResults(const PerformTabuSearch::PathWithCost& result)
{
    std::cout << "Algorytm wykonal sie w czasie: " << timer.getTime() << " [s]" << std::endl << std::endl;

    printPath(result.second);
    printCost(result.second);
    std::cout << "Sumaryczny koszt trasy: " << result.first << std::endl;

    std::cout << std::endl;
}

void PerformTabuSearch::printPath(const std::vector<uint32_t>& resultPath)
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

void PerformTabuSearch::printCost(const std::vector<uint32_t>& resultPath)
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
