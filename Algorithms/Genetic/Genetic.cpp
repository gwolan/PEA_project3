#include <algorithm>
#include <Algorithms/Genetic/Genetic.hpp>
#include <Graph/GraphMatrix.hpp>
#include <Miscellanous/Dice.hpp>
#include <Algorithms/Genetic/Utils/PathComparator.hpp>


Genetic::Genetic(const GeneticConfiguration& geneticConfig)
    : timer()
    , greedyPathMutator()
    , geneticConfiguration(geneticConfig)
    , bestSolution()
    , INFINITE_VALUE(-1)
    , graph(nullptr)
{
    reset();
}

void Genetic::reset()
{
    // set all class members to default values
    bestSolution.first = INFINITE_VALUE;
    bestSolution.second.clear();

    population.clear();
}

Genetic::PathWithCost Genetic::findBestPossibleRoute(std::unique_ptr<GraphMatrix>& graphMatrix)
{
    graph = &graphMatrix;

    timer.start();
    generateStartingPopulation();

    do
    {
        std::sort(population.begin(), population.end(), PathComparator());
        assignNewBestSolutionIfPossible(population.front());


    }
    while(isTimeUp());

    return bestSolution;
}

void Genetic::generateStartingPopulation()
{
    while(population.size() < geneticConfiguration.getPopulationSize())
    {
        PathWithCost path;

        path.second = greedyPathMutator.createPathMutationGreedyStyle(*graph);
        path.first = calculatePathsCost(path.second);

        population.push_back(path);
    }
}

void Genetic::assignNewBestSolutionIfPossible(Genetic::PathWithCost& bestPath)
{
    if(isGivenPathPromising(bestPath.first, bestSolution.first))
    {
        bestSolution.first = bestPath.first;
        bestSolution.second = bestPath.second;
    }
}

std::pair<Genetic::PathWithCost, Genetic::PathWithCost> Genetic::generateSiblings(std::vector<uint32_t>& firstParent,
                                                                                  std::vector<uint32_t>& secondParent)
{
    // allocate space for siblings (siblings = paths after OX crossover)
    std::pair<std::vector<uint32_t>, std::vector<uint32_t>> siblings;
    siblings.first.resize(firstParent.size());
    siblings.second.resize(secondParent.size());

    // roll random range for siblings to inherit
    std::pair<uint32_t, uint32_t> range = rollRange();

    // prepare iterators for OX crossover
    std::pair<std::vector<uint32_t>::iterator,
              std::vector<uint32_t>::iterator> firstParentIterators = convertIndexesToIterators(firstParent, range.first, range.second),
                                               secondParentIterators = convertIndexesToIterators(secondParent, range.first, range.second);

    std::pair<std::vector<uint32_t>::iterator,
              std::vector<uint32_t>::iterator> firstSiblingIterators = convertIndexesToIterators(siblings.first, range.first, range.second),
                                               secondSiblingIterators = convertIndexesToIterators(siblings.second, range.first, range.second);


    // perform OX crossover

    // inherit random range from primary parent
    std::copy(firstParentIterators.first, std::next(firstParentIterators.second), firstSiblingIterators.first);
    std::copy(secondParentIterators.first, std::next(secondParentIterators.second), secondSiblingIterators.first);

    // inherit verticies from secondary parent
    fillRestOfTheSiblingPathDuringOX(secondParentIterators, firstSiblingIterators, secondParent, siblings.first);
    fillRestOfTheSiblingPathDuringOX(firstParentIterators, secondSiblingIterators, firstParent, siblings.second);

    return convertSiblingsToResult(siblings);
}

void Genetic::fillRestOfTheSiblingPathDuringOX(std::pair<std::vector<uint32_t>::iterator, std::vector<uint32_t>::iterator>& parentIterators,
                                               std::pair<std::vector<uint32_t>::iterator, std::vector<uint32_t>::iterator>& siblingIterators,
                                               std::vector<uint32_t>& parent,
                                               std::vector<uint32_t>& sibling)
{
    std::vector<uint32_t>::iterator parentIt = std::next(parentIterators.second);
    std::vector<uint32_t>::iterator siblingIt = std::next(siblingIterators.second);

    // cycle through both paths (parent and sibling)
    // cycle around sibling path until you reach beginning of inherited range
    do
    {
        if(parentIt == parent.end())
        {
            parentIt = parent.begin();
        }

        if(siblingIt == sibling.end())
        {
            siblingIt = sibling.begin();
        }

        // assign parents vertex to empty space in siblings path given vertex is not on the path
        if(!wasVertexAlreadyChecked(sibling.begin(), sibling.end(), *parentIt))
        {
            *siblingIt = *parentIt;
            ++siblingIt;
        }

        ++parentIt;
    }
    while(siblingIt != siblingIterators.first);
}

std::pair<uint32_t, uint32_t> Genetic::rollRange()
{
    Dice dice(0, (*graph)->getVertexCount() - 1);
    uint32_t beginVertex = 0;
    uint32_t endVertex = 0;

    do
    {
        beginVertex = dice.roll();
        endVertex = dice.roll();
    }
    while(endVertex <= beginVertex);

    return std::pair<uint32_t, uint32_t>(beginVertex, endVertex);
}

std::pair<std::vector<uint32_t>::iterator,
          std::vector<uint32_t>::iterator> Genetic::convertIndexesToIterators(std::vector<uint32_t>& path, uint32_t beginIndex,
                                                                                                           uint32_t endIndex)
{
    std::vector<uint32_t>::iterator pathBegin = path.begin();
    std::vector<uint32_t>::iterator pathEnd = path.begin();
    std::advance(pathBegin, beginIndex);
    std::advance(pathEnd, endIndex);

    return std::pair<std::vector<uint32_t>::iterator,
                     std::vector<uint32_t>::iterator>(pathBegin, pathEnd);
}

std::pair<Genetic::PathWithCost, Genetic::PathWithCost> Genetic::convertSiblingsToResult(std::pair<std::vector<uint32_t>, std::vector<uint32_t>>& siblings)
{
    PathWithCost firstSibling;
    PathWithCost secondSibling;

    firstSibling.first = calculatePathsCost(siblings.first);
    firstSibling.second = siblings.first;
    secondSibling.first = calculatePathsCost(siblings.second);
    secondSibling.second = siblings.second;

    return std::pair<PathWithCost, PathWithCost>(firstSibling, secondSibling);
}

bool Genetic::isTimeUp()
{
    timer.stop();

    return timer.getTime() > geneticConfiguration.getGeneticInterval();
}

uint32_t Genetic::calculatePathsCost(const std::vector<uint32_t>& path)
{
    // path cost = sum of all weights of the edges between nodes
    uint32_t cost = 0;

    for(auto vertexIt  = path.cbegin();
             vertexIt != std::prev(path.cend());
           ++vertexIt)
    {
        uint32_t row = *vertexIt;
        uint32_t column = *std::next(vertexIt);

        cost += (*graph)->getWeight(row, column);
    }

    return cost;
}

template<class Iterator>
bool Genetic::wasVertexAlreadyChecked(Iterator begin, Iterator end, const uint32_t vertex)
{
    return std::find(begin, end, vertex) != end;
}

bool Genetic::isGivenPathPromising(const uint32_t currentCost, const uint32_t upperBound)
{
    return (upperBound == INFINITY) || (currentCost < upperBound);
}
