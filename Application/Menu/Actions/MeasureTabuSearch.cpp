#include <iostream>
#include <vector>
#include <cstdint>
#include <numeric>
#include <Application/Menu/Actions/MeasureTabuSearch.hpp>
#include <Graph/GraphMatrix.hpp>


MeasureTabuSearch::MeasureTabuSearch(const std::string& actionName)
    : BaseAction(actionName)
    , graph(nullptr)
    , tabuConfiguration()
    , tabuSearch(tabuConfiguration)
    , intervals({5, 10, 15, 20})
    , numberOfMeasurements(5)
    , iterations(2000)
    , defaultCadency(0)
    , INFINITY(-1)
    , problemOptimalResult(0)
    , resultsFile()
    , resultsFileName("TabuSearch_results.txt")
{ }

MeasureTabuSearch::~MeasureTabuSearch()
{
    resultsFile.close();
}

void MeasureTabuSearch::init(std::unique_ptr<GraphMatrix>& graphMatrix)
{
    graph = &graphMatrix;

    std::cout << "Podaj optymalne rozwiazanie dla danego problemu: ";
    std::cin >> problemOptimalResult;

    resultsFile.open(resultsFileName, std::ofstream::app);
}

void MeasureTabuSearch::run()
{
    if(resultsFile.is_open() && graph->get())
    {
        performMeasurements(false);
        performMeasurements(true);
        saveResultsToFile();

        std::cout << "Pomiary skonczone." << std::endl << std::endl;
    }
    else
    {
        std::cout << "Graf jest pusty lub blad przy operacji na plikach." << std::endl << std::endl;
    }

}

void MeasureTabuSearch::performMeasurements(bool useDiversification)
{
    std::cout << std::endl;
    std::cout << "Rozpoczecie pomiarow dla TabuSearch " << (useDiversification ? "z dywersyfikacją." : 
                                                                                 "bez dywersyfikacji.") << std::endl;

    for(auto interval = intervals.begin(); interval != intervals.end(); ++interval)
    {
        int32_t lowestCost = INFINITY;
        double lowestCostRelativeError = 0.0;

        setTabuConfig(*interval, useDiversification, iterations, defaultCadency);

        for(uint32_t counter = 0; counter < numberOfMeasurements; ++counter)
        {
            std::cout << "Pomiar " << static_cast<uint32_t>(*interval) << "s (" << counter + 1 << " z " << numberOfMeasurements << ")..." << std::endl;
            PathWithCost path = tabuSearch.findBestPossibleRoute(*graph);

            if(isGivenPathPromising(path.first, lowestCost))
            {
                lowestCost = path.first;
                lowestCostRelativeError = calculateRelativeError(path.first);
            }
        }

        TabuSearchMeasureResult result;
        result.timeInSeconds = *interval;
        result.relativeError = lowestCostRelativeError;
        result.pathCost = lowestCost;
        result.diversification = tabuConfiguration.isDiversificationEnabled();

        results.push_back(result);
        printResults(result);
    }
}

void MeasureTabuSearch::setTabuConfig(double tabuInterval, bool tabuDiversification, uint32_t tabuNumberOfIterations, uint32_t tabuCadencyLength)
{
    tabuConfiguration.setTabuInterval(tabuInterval);
    tabuConfiguration.setDiversificationCriteria(tabuNumberOfIterations);

    if(tabuCadencyLength == 0)
    {
        tabuConfiguration.setTabuCadencyLength(getTabuCadencyForGraph((*graph)->getVertexCount()));
    }
    else
    {
        tabuConfiguration.setTabuCadencyLength(tabuCadencyLength);
    }

    if(tabuDiversification)
    {
        tabuConfiguration.enableDiversification();
    }
    else
    {
        tabuConfiguration.disableDiversification();
    }
}

bool MeasureTabuSearch::isGivenPathPromising(const uint32_t currentCost, const uint32_t upperBound)
{
    return (upperBound == INFINITY) || (currentCost < upperBound);
}

uint32_t MeasureTabuSearch::getTabuCadencyForGraph(const uint32_t verticiesCount)
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

double MeasureTabuSearch::calculateRelativeError(uint32_t pathCost)
{
    double pathCostAsDouble = static_cast<double>(pathCost);
    return ((pathCost - problemOptimalResult) / problemOptimalResult) * 100.0;
}

void MeasureTabuSearch::saveResultsToFile()
{
    resultsFile << "Czas" << "\t" << "Blad" << "\t" << "Koszt" << "\t" << "Dywersyfikacja" << std::endl;

    for(auto result : results)
    {
        resultsFile << result.timeInSeconds << "\t" << result.relativeError << "\t" << result.pathCost << "\t" << (result.diversification ? "Tak" : "Nie") << std::endl;
    }
    resultsFile << std::endl;

    resultsFile.close();
    std::cout << "Wyniki pomyślnie zapisano do pliku." << std::endl;
}

void MeasureTabuSearch::printResults(const TabuSearchMeasureResult& result)
{
    std::cout << "Wyniki dla " << static_cast<uint32_t>(result.timeInSeconds) << "s:" << std::endl;

    std::cout << "\t- blad wzgledny: " << result.relativeError << "%" << std::endl;
    std::cout << "\t- koszt trasy: " << result.pathCost << std::endl << std::endl;
}
