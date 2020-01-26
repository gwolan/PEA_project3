#include <numeric>
#include <algorithm>
#include <Algorithms/BranchAndBound/BranchAndBound.hpp>
#include <Graph/GraphMatrix.hpp>


BranchAndBound::BranchAndBound()
    : graph(nullptr)
    , INFINITY(-1)
    , STARTING_VERTEX(0)
{ }

std::vector<uint32_t> BranchAndBound::performBranchAndBoundOnGraph(std::unique_ptr<GraphMatrix>& graphMatrix)
{
    graph = &graphMatrix;

    // resource allocation

    // maps best solutions cost to path
    BestSolutionHolder bestSolutionHolder;
    bestSolutionHolder.first = INFINITY;

    // will hold partial path from the root of the solutions tree, first element contains path cost
    std::vector<uint32_t> currentBestPartialPath;

    // holds verticies indexes
    std::vector<uint32_t> verticies((*graph)->getVertexCount());
    std::iota(verticies.begin(), verticies.end(), STARTING_VERTEX);

    // solutions tree is represented by priority queue (best first solution)
    PathQueue solutionsTree;

    startAlgorithm(verticies, currentBestPartialPath, solutionsTree, bestSolutionHolder);

    return bestSolutionHolder.second;
}

void BranchAndBound::startAlgorithm(std::vector<uint32_t>& verticies,
                                    std::vector<uint32_t>& currentBestPartialPath, BranchAndBound::PathQueue& solutionsTree,
                                                                                   BranchAndBound::BestSolutionHolder& bestSolutionHolder)
{
    // solutions tree root will hold basic path with starting point only
    currentBestPartialPath.push_back(0);
    currentBestPartialPath.push_back(STARTING_VERTEX);
    solutionsTree.push(currentBestPartialPath);

    // algorithm will loop until no better solution can be found
    while(!solutionsTree.empty())
    {
        // load currently best solution, ignore all other solutions
        currentBestPartialPath = solutionsTree.top();
        solutionsTree.pop();

        // if better solution can be found, start calculations
        if(isGivenPathPromising(currentBestPartialPath.front(), bestSolutionHolder.first))
        {
            performCalculations(verticies, currentBestPartialPath, solutionsTree, bestSolutionHolder);
        }
        else
        {
            // best solution already found, terminate
            break;
        }
    }
}

void BranchAndBound::performCalculations(std::vector<uint32_t>& verticies,
                                         std::vector<uint32_t>& currentBestPartialPath, BranchAndBound::PathQueue& solutionsTree,
                                                                                        BranchAndBound::BestSolutionHolder& bestSolutionHolder)
{
    for(auto nextPossibleVertex = verticies.begin(); nextPossibleVertex != verticies.end(); ++nextPossibleVertex)
    {
        // hot looping to evaluate not visited vertex
        if(wasVertexAlreadyChecked(std::next(currentBestPartialPath.cbegin()), 
                                             currentBestPartialPath.cend(), *nextPossibleVertex))
        {
            continue;
        }

        // new partial paths contain next possible vertex to choose
        std::vector<uint32_t> newPartialPath = currentBestPartialPath;
        newPartialPath.push_back(*nextPossibleVertex);

        if(isCurrentlyEvaluatedPathEnding(newPartialPath, verticies))
        {
            // calculate overall Hamilton cycle cost and compare path to currently best known solution
            finalizeCalculations(newPartialPath, bestSolutionHolder);
        }
        else
        {
            // current path is not complete, continue calculations
            performNextIteration(verticies, currentBestPartialPath, newPartialPath);

            // push promising partial path on the tree
            if(isGivenPathPromising(newPartialPath.front(), bestSolutionHolder.first))
            {
                solutionsTree.push(newPartialPath);
            }
        }
    }
}

void BranchAndBound::performNextIteration(std::vector<uint32_t>& verticies,
                                          std::vector<uint32_t>& currentBestPartialPath,
                                          std::vector<uint32_t>& newPartialPath)
{
    // calculate path cost after adding next possible vertex
    newPartialPath.front() = calculatePathsCost(newPartialPath);

    // calculate overall minimal possible solution from current partial solution
    for(auto edgeBegin = std::next(verticies.begin()); edgeBegin != verticies.end(); ++edgeBegin)
    {
        // hot looping to evaluate not visited vertex
        if(wasVertexAlreadyChecked(std::next(currentBestPartialPath.cbegin()),
                                             currentBestPartialPath.cend(), *edgeBegin))
        {
            continue;
        }

        // find minimal possible solution (paths cost) for current partial path
        // result - find most promising partial path
        int32_t cheapestEdgeCost = INFINITY;
        for(auto edgeEnd = verticies.begin(); edgeEnd != verticies.end(); ++edgeEnd)
        {
            // hot looping if:
            // - calculated edge joins current partial path with path end (starting point)
            // - edge end defines vertex belonging to current partial path
            // - both verticies do not create an edge
            if( isEvaluatedEdgeClosingHamiltonCycle(*edgeBegin, *edgeEnd, newPartialPath.back())                ||
                wasVertexAlreadyChecked(std::next(newPartialPath.cbegin(), 2), newPartialPath.cend(), *edgeEnd) ||
               !doVerticiesCreateEdge(*edgeBegin, *edgeEnd))
            {
                continue;
            }

            // calculate the cheapest edge for potential paths next possible steps
            uint32_t currentEdgeCost = (*graph)->getWeight(*edgeBegin, *edgeEnd);
            if(isGivenPathPromising(currentEdgeCost, cheapestEdgeCost))
            {
                cheapestEdgeCost = currentEdgeCost;
            }
        }

        // add cheapest potential edge cost to current partial solution
        newPartialPath.front() += cheapestEdgeCost;
    }
}

void BranchAndBound::finalizeCalculations(std::vector<uint32_t>& pathWithSolution, BranchAndBound::BestSolutionHolder& bestSolutionHolder)
{
    pathWithSolution.push_back(STARTING_VERTEX);
    pathWithSolution.front() = calculatePathsCost(pathWithSolution);

    assignNewBestPathIfPossible(pathWithSolution, bestSolutionHolder);
}

void BranchAndBound::assignNewBestPathIfPossible(std::vector<uint32_t>& possibleBestPath, BranchAndBound::BestSolutionHolder& bestSolutionHolder)
{
    if(isGivenPathPromising(possibleBestPath.front(), bestSolutionHolder.first))
    {
        bestSolutionHolder.first = possibleBestPath.front();
        possibleBestPath.erase(possibleBestPath.begin());
        bestSolutionHolder.second = possibleBestPath;
    }
}

template<class Iterator>
bool BranchAndBound::wasVertexAlreadyChecked(Iterator begin, Iterator end, const uint32_t vertex)
{
    return std::find(begin, end, vertex) != end;
}

bool BranchAndBound::isCurrentlyEvaluatedPathEnding(const std::vector<uint32_t>& evaluatedPath,
                                                    const std::vector<uint32_t>& verticies)
{
    return evaluatedPath.size() > verticies.size();
}

uint32_t BranchAndBound::calculatePathsCost(const std::vector<uint32_t>& path)
{
    // path cost = sum of all weights of the edges between nodes
    uint32_t cost = 0;

    for(auto vertexIt  = std::next(path.cbegin());
             vertexIt != std::prev(path.cend());
           ++vertexIt)
    {
        uint32_t row = *vertexIt;
        uint32_t column = *std::next(vertexIt);

        cost += (*graph)->getWeight(row, column);
    }

    return cost;
}

bool BranchAndBound::isEvaluatedEdgeClosingHamiltonCycle(const uint32_t edgeBegin, const uint32_t edgeEnd, const uint32_t lastVertexInPartialPath)
{
    return (edgeBegin == lastVertexInPartialPath) && (edgeEnd == STARTING_VERTEX);
}

bool BranchAndBound::doVerticiesCreateEdge(const uint32_t edgeBegin, const uint32_t edgeEnd)
{
    return edgeEnd != edgeBegin;
}

bool BranchAndBound::isGivenPathPromising(const uint32_t currentCost, const uint32_t upperBound)
{
    return (upperBound == INFINITY) || (currentCost < upperBound);
}
