#include <algorithm>
#include <numeric>
#include <Algorithms/Genetic/GreedyGenerationCreator.hpp>
#include <Graph/GraphMatrix.hpp>
#include <Miscellanous/Dice.hpp>


GreedyGenerationCreator::GreedyGenerationCreator()
    : INFINITE_VALUE(-1)
    , randomMutationPrefixLength(3)
{ }

std::vector<uint32_t> GreedyGenerationCreator::createGenerationGreedyStyle(std::unique_ptr<GraphMatrix>& graphMatrix)
{
    // when creating a generation - random mutation is involved to mutate first veticies
    std::vector<uint32_t> startingPath;
    generatePathBeginning(graphMatrix, startingPath);

    // list of verticies just for reference
    std::vector<uint32_t> verticies(graphMatrix->getVertexCount());
    std::iota(verticies.begin(), verticies.end(), 0);

    // path should contain cycle with N+1 nodes (starting node repeats itself at the beginning and end)
    // so N-1 nodes should be checked (we are excluding starting node) to create best possible path in a greedy way
    while(startingPath.size() < graphMatrix->getVertexCount())
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
    startingPath.push_back(startingPath.front());
    return startingPath;
}

void GreedyGenerationCreator::generatePathBeginning(std::unique_ptr<GraphMatrix>& graphMatrix, std::vector<uint32_t>& path)
{
    // choose starting vertex randomly
    Dice dice(0, graphMatrix->getVertexCount() - 1);
    uint32_t startingVertex = dice.roll();
    path.push_back(startingVertex);

    // chhose two additional verticies for generation
    while(path.size() < randomMutationPrefixLength)
    {
        uint32_t nextVertex = dice.roll();

        if(std::find(path.begin(), path.end(), nextVertex) == path.end())
        {
            path.push_back(nextVertex);
        }
    }
}

template<class Iterator>
bool GreedyGenerationCreator::wasVertexAlreadyChecked(Iterator begin, Iterator end, const uint32_t vertex)
{
    return std::find(begin, end, vertex) != end;
}

bool GreedyGenerationCreator::isGivenEdgePromising(const uint32_t edgeWeight, const uint32_t upperBound)
{
    return (upperBound == INFINITE_VALUE) || (edgeWeight < upperBound);
}
