#include <algorithm>
#include <numeric>
#include <Algorithms/TabuSearch/LocalSearch.hpp>
#include <Graph/GraphMatrix.hpp>
#include <Miscellanous/Dice.hpp>


LocalSearch::LocalSearch()
    : INFINITE_VALUE(-1)
{ }

std::vector<uint32_t> LocalSearch::findStartingPathGreedyStyle(std::unique_ptr<GraphMatrix>& graphMatrix)
{
    // choose starting vertex randomly
    Dice dice(0, graphMatrix->getVertexCount() - 1);
    uint32_t startingVertex = dice.roll();

    // holds result
    std::vector<uint32_t> startingPath;
    startingPath.push_back(startingVertex);

    // list of verticies just for reference
    std::vector<uint32_t> verticies(graphMatrix->getVertexCount());
    std::iota(verticies.begin(), verticies.end(), 0);

    // path should contain cycle with N+1 nodes (starting node repeats itself at the beginning and end)
    // so N-1 nodes should be checked (we are excluding starting node) to create best possible path in a greedy way
    for(auto it = verticies.begin(); it != std::prev(verticies.end()); ++it)
    {
        int32_t cheapestEdge = INFINITE_VALUE;
        uint32_t nextNode;

        // keep adding new nodes to starting path starting from the cheapest one
        for(auto vertex = verticies.begin(); vertex != verticies.end(); ++vertex)
        {
            // pass this iteration if given node was already added to the path (Hamilton cycle requirement)
            if(wasVertexAlreadyChecked(startingPath.begin(), startingPath.end(), *vertex))
            {
                continue;
            }

            // check which neighbouring node is the best choice for next step to create a cycle
            uint32_t promisingEdge = graphMatrix->getWeight(startingPath.back(), *vertex);
            if(isGivenEdgePromising(promisingEdge, cheapestEdge))
            {
                cheapestEdge = promisingEdge;
                nextNode = *vertex;
            }
        }

        startingPath.push_back(nextNode);
    }

    // close Hamilton cycle and return
    startingPath.push_back(startingVertex);
    return startingPath;
}

std::vector<uint32_t> LocalSearch::findStartingPathRandomly(std::unique_ptr<GraphMatrix>& graphMatrix)
{
    // initialize generator
    Dice dice(0, graphMatrix->getVertexCount() - 1);

    // holds result
    std::vector<uint32_t> startingPath;
    startingPath.push_back(dice.roll());

    // keep rolling until unique random permutation of verticies will be created
    do
    {
        uint32_t nextNode = dice.roll();

        if(!wasVertexAlreadyChecked(startingPath.begin(), startingPath.end(), nextNode))
        {
            startingPath.push_back(nextNode);
        }
    }
    while(startingPath.size() < graphMatrix->getVertexCount());

    // close Hamilton cycle and return
    startingPath.push_back(startingPath.front());
    return startingPath;
}

template<class Iterator>
bool LocalSearch::wasVertexAlreadyChecked(Iterator begin, Iterator end, const uint32_t vertex)
{
    return std::find(begin, end, vertex) != end;
}

bool LocalSearch::isGivenEdgePromising(const uint32_t edgeWeight, const uint32_t upperBound)
{
    return (upperBound == INFINITE_VALUE) || (edgeWeight < upperBound);
}
