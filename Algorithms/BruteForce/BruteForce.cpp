#include <algorithm>
#include <numeric>
#include <Algorithms/BruteForce/BruteForce.hpp>
#include <Graph/GraphMatrix.hpp>


BruteForce::BruteForce()
    : graph(nullptr)
{ }

std::vector<uint32_t> BruteForce::performBruteForceOnGraph(std::unique_ptr<GraphMatrix>& graphMatrix)
{
    graph = &graphMatrix;

    std::vector<Path> listOfPossiblePaths;
    std::vector<uint32_t> verticies((*graph)->getVertexCount() - 1);

    // 'verticies' vector should contain all verticies besides the starting one
    // at the start of the algorithm verticies should be ordered for further permutations
    std::iota(verticies.begin(), verticies.end(), 1);

    // all possible paths will be collected in 'listOfPossiblePaths'
    findAllPossiblePaths(verticies, listOfPossiblePaths);

    // calculate the shortest path
    Path& shortestPath = findShortestPath(listOfPossiblePaths);

    // collect verticies sequence from the shortest path
    std::vector<uint32_t> bestSequence;
    for(auto& vertex : shortestPath)
    {
        bestSequence.push_back(vertex.first);
    }

    return bestSequence;
}

void BruteForce::findAllPossiblePaths(std::vector<uint32_t>& verticies, std::vector<Path>& listOfPossiblePaths)
{
    // this loop reads graph matrix and creates all possible paths
    // it will be a base to find the shortest route
    // loop will execute until all vertex permutations will be analysed
    do
    {
        uint32_t weight;
        listOfPossiblePaths.emplace_back();

        // border case, add starting node with its weight
        // then add the first available node
        listOfPossiblePaths.back().emplace_back(0, 0);
        weight = (*graph)->getWeight(0, verticies.front());
        listOfPossiblePaths.back().emplace_back(verticies.front(), weight);

        // add rest of the possible routes within graph matrix
        for(int i = 1; i < verticies.size(); i++)
        {
            weight = (*graph)->getWeight(verticies[i - 1], verticies[i]);
            listOfPossiblePaths.back().emplace_back(verticies[i], weight);
        }

        // border case, add route from the last node to the starting node
        weight = (*graph)->getWeight(verticies.back(), 0);
        listOfPossiblePaths.back().emplace_back(0, weight);
    }
    while(std::next_permutation(verticies.begin(), verticies.end()));
}

BruteForce::Path& BruteForce::findShortestPath(std::vector<Path>& listOfPossiblePaths)
{
    // assume that first path is the shortest
    uint32_t currentMinimum = calculatePathsCost(*listOfPossiblePaths.begin());
    Path* shortestPath = &listOfPossiblePaths.front();

    // verify above assumption with the rest of the paths
    for(auto currentPath  = std::next(listOfPossiblePaths.begin());
             currentPath != listOfPossiblePaths.end();
           ++currentPath)
    {
        uint32_t pathCost = calculatePathsCost(*currentPath);

        // reassing already chosen path with better solution if possible
        if(pathCost < currentMinimum)
        {
            currentMinimum = pathCost;
            shortestPath = &(*currentPath);
        }
    }

    return *shortestPath;
}

uint32_t BruteForce::calculatePathsCost(Path& currentPath)
{
    // path cost = sum of all weights of the edges between nodes
    uint32_t cost = 0;

    for(auto& vertexToWeight : currentPath)
    {
        cost += vertexToWeight.second;
    }

    return cost;
}
