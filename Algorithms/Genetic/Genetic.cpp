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

    graph = nullptr;
}

Genetic::PathWithCost Genetic::findBestPossibleRoute(std::unique_ptr<GraphMatrix>& graphMatrix)
{
    reset();
    graph = &graphMatrix;

    timer.start();
    generateStartingPopulation();

    do
    {
        std::vector<PathWithCost> newGeneration;

        // generate new paths using OX crossover
        breedCurrentPopulation(newGeneration);

        // mutate new paths using scramble or inversion mutation
        mutateNewGeneration(newGeneration);

        // merge current population with new generation
        addNewGenerationToCurrentPopulation(newGeneration);

        // sort path population placing shortest paths above the longest
        // resize population and throw away too long paths
        cutOffWeakPopulationMembers();

        // try to assing new optimal solution
        assignNewBestSolutionIfPossible(population.front());
    }
    while(!isTimeUp());

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

void Genetic::cutOffWeakPopulationMembers()
{
    std::sort(population.begin(), population.end(), PathComparator());
    population.resize(geneticConfiguration.getPopulationSize());
}

bool Genetic::shouldCrossoverHappen()
{
    Dice dice(1, 99);

    double rolledCrossoverVariable = static_cast<double>(dice.roll()) / 100.0;

    if(rolledCrossoverVariable < geneticConfiguration.getCrossoverCoefficient())
    {
        return true;
    }

    return false;
}

bool Genetic::shouldMutationHappen()
{
    Dice dice(1, 99);

    double rolledMutationVariable = static_cast<double>(dice.roll()) / 100.0;

    if(rolledMutationVariable < geneticConfiguration.getMutationCoefficient())
    {
        return true;
    }

    return false;
}

void Genetic::assignNewBestSolutionIfPossible(Genetic::PathWithCost& bestPath)
{
    if(isGivenPathPromising(bestPath.first, bestSolution.first))
    {
        bestSolution.first = bestPath.first;
        bestSolution.second = bestPath.second;
    }
}

void Genetic::breedCurrentPopulation(std::vector<Genetic::PathWithCost>& newGeneration)
{
    // ranking selection
    // reproduce only specified amount of best solutions
    uint32_t reproductorsCount = getReproductorsCount();
    auto endIterator = population.begin();
    std::advance(endIterator, reproductorsCount + 1);

    for(auto firstParent = population.begin(); firstParent != endIterator; ++firstParent)
    {
        if(shouldCrossoverHappen())
        {
            for(auto secondParent = population.begin(); secondParent != endIterator; ++secondParent)
            {
                if(firstParent == secondParent)
                {
                    continue;
                }

                if(shouldCrossoverHappen())
                {
                    // use OX crossover to create siblings
                    std::pair<PathWithCost, PathWithCost> siblings = generateSiblings(firstParent->second, secondParent->second);

                    newGeneration.push_back(siblings.first);
                    newGeneration.push_back(siblings.second);
                }
            }
        }
    }
}

uint32_t Genetic::getReproductorsCount()
{
    return static_cast<uint32_t>(static_cast<double>(population.size()) * geneticConfiguration.getReproductionCoefficient());
}

void Genetic::mutateNewGeneration(std::vector<Genetic::PathWithCost>& newGeneration)
{
    if(geneticConfiguration.isInversionMutationEnabled())
    {
        inversionMutation(newGeneration);
    }
    else
    {
        scrambleMutation(newGeneration);
    }
}

void Genetic::inversionMutation(std::vector<Genetic::PathWithCost>& newGeneration)
{
    for(auto newPath = newGeneration.begin(); newPath != newGeneration.end(); ++newPath)
    {
        if(shouldMutationHappen())
        {
            // choose random range and invert verticies
            auto indexes = rollRange(1, (*graph)->getVertexCount() - 1);
            auto interators = convertIndexesToIterators(newPath->second, indexes.first, indexes.second);

            auto begin = interators.first;
            auto end = interators.second;

            do
            {
                auto vertexA = begin;
                auto vertexB = end;

                std::iter_swap(vertexA, vertexB);

                begin++;
                end--;
            }
            while(begin < end);
        }
    }
}

void Genetic::scrambleMutation(std::vector<Genetic::PathWithCost>& newGeneration)
{
    for(auto newPath = newGeneration.begin(); newPath != newGeneration.end(); ++newPath)
    {
        if(shouldMutationHappen())
        {
            // choose two random verticies and swap them
            auto indexes = rollRange(1, (*graph)->getVertexCount() - 1);
            auto interators = convertIndexesToIterators(newPath->second, indexes.first, indexes.second);

            std::iter_swap(interators.first, interators.second);
        }
    }
}

void Genetic::addNewGenerationToCurrentPopulation(std::vector<Genetic::PathWithCost>& newGeneration)
{
    population.insert(population.end(), newGeneration.begin(), newGeneration.end());
}

std::pair<Genetic::PathWithCost, Genetic::PathWithCost> Genetic::generateSiblings(std::vector<uint32_t>& firstParent,
                                                                                  std::vector<uint32_t>& secondParent)
{
    // allocate space for siblings (siblings = paths after OX crossover)
    std::pair<std::vector<uint32_t>, std::vector<uint32_t>> siblings;
    siblings.first.resize(firstParent.size());
    siblings.second.resize(secondParent.size());

    // roll random range for siblings to inherit
    std::pair<uint32_t, uint32_t> range = rollRange(0, (*graph)->getVertexCount() - 1);

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

    // cycle through both paths (parent and sibling) excluding ending vertex
    // cycle around sibling path until you reach beginning of inherited range
    do
    {
        // cycle around from the end to beginning
        if(parentIt == std::prev(parent.end()))
        {
            parentIt = parent.begin();
        }

        // cycle around from the end to beginning
        if(siblingIt == std::prev(sibling.end()))
        {
            siblingIt = sibling.begin();
        }

        // assign parents vertex to empty space in siblings path if given vertex is not already on the path
        if(!wasVertexAlreadyChecked(siblingIterators.first, std::next(siblingIterators.second), *parentIt))
        {
            *siblingIt = *parentIt;
            ++siblingIt;
        }

        ++parentIt;
    }
    while(siblingIt != siblingIterators.first);

    // close Hamilton cycle
    sibling.back() = sibling.front();
}

std::pair<uint32_t, uint32_t> Genetic::rollRange(uint32_t lowerBound, uint32_t upperBound)
{
    Dice dice(lowerBound, upperBound);
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
