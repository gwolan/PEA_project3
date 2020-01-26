#include <algorithm>
#include <Algorithms/TabuSearch/TabuSearch.hpp>
#include <Graph/GraphMatrix.hpp>


TabuSearch::TabuSearch(const TabuConfiguration& tabuConfig)
    : localSearch()
    , timer()
    , INFINITY(-1)
    , graph(nullptr)
    , iterationsWithoutImprovements(0)
    , tabuList()
    , tabuConfiguration(tabuConfig)
    , bestSolution()
    , intensification(false)
    , cadencyDivisor(1)
{ }

void TabuSearch::reset()
{
    // set all class members to default values
    tabuList.clear();

    bestSolution.first = INFINITY;
    bestSolution.second.clear();

    iterationsWithoutImprovements = 0;

    intensification = false;

    cadencyDivisor = 1;
}

TabuSearch::PathWithCost TabuSearch::findBestPossibleRoute(std::unique_ptr<GraphMatrix>& graphMatrix)
{
    graph = &graphMatrix;

    // first path is selected using greedy algorithm
    std::vector<uint32_t> startingPath = localSearch.findStartingPathGreedyStyle(*graph);

    // reset environment and start internal timer
    reset();
    timer.start();

    // keep searching until time is up
    while(!isTimeUp())
    {
        runTabuSearch(startingPath);

        // try to intensify search in given area if solution was improved
        if(tabuConfiguration.isDiversificationEnabled())
        {
            intensify(startingPath);
        }

        // reset iteration counter for another run
        iterationsWithoutImprovements = 0;
    }

    return bestSolution;
}

void TabuSearch::intensify(std::vector<uint32_t>& startingPath)
{
    if(intensification)
    {
        // intensification selects current best solution
        // and reduces cadency in Tabu table
        startingPath = bestSolution.second;
        cadencyDivisor = 2;
    }
    else
    {
        // if solution was not improved generate another permutation
        // and try again
        startingPath = localSearch.findStartingPathRandomly(*graph);
        cadencyDivisor = 1;
    }

    disableIntesification();
}

void TabuSearch::runTabuSearch(std::vector<uint32_t>& promisingPath)
{
    do
    {
        // find best neighbourhood for given path
        TabuMove tabuMove;
        PathWithCost bestNeighbourhood = findBestNeighbourPathUsingSwap(promisingPath, tabuMove);


        // it is possible that all swaps will be blocked by Tabu table
        // then swapping algorithm will return unassigned path
        if(isCurrentNeighbourPathValid(bestNeighbourhood))
        {
            assignNewBestSolutionIfPossible(bestNeighbourhood);
            promisingPath = bestNeighbourhood.second;
        }

        // update Tabu table by reducing cadency and pushing performed swap if possible
        updateTabuList(tabuMove);
        iterationsWithoutImprovements++;
    }
    while(!shouldAlgorithmStop());
}

void TabuSearch::assignNewBestSolutionIfPossible(PathWithCost& bestNeighbourhood)
{
    if(isGivenPathPromising(bestNeighbourhood.first, bestSolution.first))
    {
        // intensify search in given area during next iteration
        // if recent search made an improvement to solution
        if(bestNeighbourhood.first > bestSolution.first)
        {
            enableIntesification();
        }

        bestSolution.first = bestNeighbourhood.first;
        bestSolution.second = bestNeighbourhood.second;
        iterationsWithoutImprovements = 0;
    }
}

bool TabuSearch::shouldAlgorithmStop()
{
    if(tabuConfiguration.isDiversificationEnabled())
    {
        return wereIterationsExceeded() || isTimeUp();
    }
    else
    {
        return isTimeUp();
    }
}

bool TabuSearch::wereIterationsExceeded()
{
    return iterationsWithoutImprovements > tabuConfiguration.getNumberOfIterations();
}

bool TabuSearch::isTimeUp()
{
    timer.stop();

    return timer.getTime() > tabuConfiguration.getTabuInterval();
}

void TabuSearch::enableIntesification()
{
    intensification = true;
}

void TabuSearch::disableIntesification()
{
    intensification = false;
}

TabuSearch::PathWithCost TabuSearch::findBestNeighbourPathUsingSwap(const std::vector<uint32_t>& promisingPath, TabuMove& tabuMove)
{
    // initialize swapping result
    PathWithCost bestNeighbourhood;
    bestNeighbourhood.first = INFINITY;

    std::vector<uint32_t> neighbourPath;
    neighbourPath.resize(promisingPath.size());


    for(auto vertex = std::next(neighbourPath.begin()); vertex != std::prev(std::prev(neighbourPath.end())); ++vertex)
    {
        for(auto nextVertex = std::next(vertex); nextVertex != std::prev(neighbourPath.end()); ++nextVertex)
        {
            TabuMove currentMove;
            std::copy(promisingPath.begin(), promisingPath.end(), neighbourPath.begin());

            // define move for swap
            currentMove.vertexA = *vertex;
            currentMove.vertexB = *nextVertex;

            // perform swap on promising path
            std::iter_swap(vertex, nextVertex);

            // assign new neighbourhood if Tabu allows it or current neighbour path is aspiring
            //
            // if all moves are Tabu or no improvement can be achieved this function may not assign
            // better neighbourhood at all
            if(!isGivenMoveTabu(currentMove) || isAspirationCriteriaFulfilled(neighbourPath))
            {
                assignNewBestNeighbourhoodIfPossible(neighbourPath, bestNeighbourhood, tabuMove, currentMove);
            }
        }
    }

    // may be returned empty if all swaps are Tabu or no improvement were achieved
    return bestNeighbourhood;
}

void TabuSearch::assignNewBestNeighbourhoodIfPossible(const std::vector<uint32_t>& neighbourPath,
                                                      TabuSearch::PathWithCost& bestNeighbourhood,
                                                      TabuMove& tabuMove,
                                                      const TabuMove& currentMove)
{
    uint32_t neighbouringPathCost = calculatePathsCost(neighbourPath);

    if(isGivenPathPromising(neighbouringPathCost, bestNeighbourhood.first))
    {
        bestNeighbourhood.first = neighbouringPathCost;
        bestNeighbourhood.second = neighbourPath;
        tabuMove.vertexA = currentMove.vertexA;
        tabuMove.vertexB = currentMove.vertexB;
    }
}

bool TabuSearch::isAspirationCriteriaFulfilled(const std::vector<uint32_t>& neighbourPath)
{
    // aspiration critera are fulfilled when current path beats currently best solution
    uint32_t neighbouringPathCost = calculatePathsCost(neighbourPath);

    if(neighbouringPathCost < bestSolution.first)
    {
        return true;
    }

    return false;
}

bool TabuSearch::isCurrentNeighbourPathValid(const TabuSearch::PathWithCost& bestNeighbourhood)
{
    return bestNeighbourhood.first != INFINITY;
}

uint32_t TabuSearch::calculatePathsCost(const std::vector<uint32_t>& path)
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

bool TabuSearch::isGivenPathPromising(const uint32_t currentCost, const uint32_t upperBound)
{
    return (upperBound == INFINITY) || (currentCost < upperBound);
}

void TabuSearch::updateTabuList(TabuMove& newTabuMove)
{
    // reduce cadency
    for(auto tabuMove = tabuList.begin(); tabuMove != tabuList.end();)
    {
        tabuMove->cadency--;

        if(tabuMove->cadency == 0)
        {
            tabuList.erase(tabuMove);
        }
        else
        {
            ++tabuMove;
        }
    }

    // add new Tabu move if swap was performed
    if(!newTabuMove.isEmpty())
    {
        newTabuMove.cadency = tabuConfiguration.getCadencyLength() / cadencyDivisor;
        tabuList.push_back(newTabuMove);
    }
}

bool TabuSearch::isGivenMoveTabu(const TabuMove& tabuMove)
{
    return std::find(tabuList.begin(), tabuList.end(), tabuMove) != tabuList.end();
}
