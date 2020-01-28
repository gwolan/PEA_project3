#include <iostream>
#include <vector>
#include <cstdint>
#include <numeric>
#include <Application/Menu/Actions/MeasureGenetic.hpp>
#include <Graph/GraphMatrix.hpp>


MeasureGenetic::MeasureGenetic(const std::string& actionName)
    : BaseAction(actionName)
    , graph(nullptr)
    , geneticConfiguration()
    , genetic(geneticConfiguration)
    , intervals({0.1, 0.2, 0.5})
    , populationSizes({25, 50, 100})
    , crossoverCoefficientForTest(0.8)
    , mutationCoefficientsForTest({0.02, 0.05, 0.1})
    , mutationCoefficientForTest(0.01)
    , crossoverCoefficientsForTest({0.5, 0.7, 0.9})
    , numberOfMeasurements(3)
    , INFINITY(-1)
    , problemOptimalResult(0)
    , bestPopulationSolution(0, INFINITY)
    , resultsFile()
    , resultsFileName("Genetic_results.txt")
{ }

MeasureGenetic::~MeasureGenetic()
{
    resultsFile.close();
}

void MeasureGenetic::init(std::unique_ptr<GraphMatrix>& graphMatrix)
{
    graph = &graphMatrix;

    std::cout << "Podaj optymalne rozwiazanie dla danego problemu: ";
    std::cin >> problemOptimalResult;

    resultsFile.open(resultsFileName, std::ofstream::app);
}

void MeasureGenetic::run()
{
    if(resultsFile.is_open() && graph->get())
    {
        performMeasurements(false);
        performMeasurements(true);
        performMeasurementsForBestPopulation(false);
        performMeasurementsForBestPopulation(true);
        saveResultsToFile();

        std::cout << "Pomiary skonczone." << std::endl << std::endl;
    }
    else
    {
        std::cout << "Graf jest pusty lub blad przy operacji na plikach." << std::endl << std::endl;
    }

}

void MeasureGenetic::performMeasurements(bool useInversionMutation)
{
    std::cout << std::endl;
    std::cout << "Rozpoczecie pomiarow dla algorytmu genetycznego " << (useInversionMutation ? "z Inversion Mutation." : 
                                                                                               "ze Scramble Mutation.") << std::endl;

    for(auto populationSize : populationSizes)
    {
        for(auto interval = intervals.begin(); interval != intervals.end(); ++interval)
        {
            int32_t lowestCost = INFINITY;
            double lowestCostRelativeError = 0.0;

            setGeneticConfig(useInversionMutation, geneticConfiguration.getMutationCoefficient(),
                                                   geneticConfiguration.getCrossoverCoefficient(),
                                                   geneticConfiguration.getReproductionCoefficient(), *interval, populationSize);

            for(uint32_t counter = 0; counter < numberOfMeasurements; ++counter)
            {
                std::cout << "Pomiar " << populationSize << " osobnikow dla " << static_cast<uint32_t>(*interval) << "s (" << counter + 1 << " z " << numberOfMeasurements << ")..." << std::endl;
                PathWithCost path = genetic.findBestPossibleRoute(*graph);

                if(isGivenPathPromising(path.first, lowestCost))
                {
                    lowestCost = path.first;
                    lowestCostRelativeError = calculateRelativeError(path.first);
                }
            }

            GeneticMeasureResult result;
            result.timeInSeconds = *interval;
            result.relativeError = lowestCostRelativeError;
            result.inversionMutation = geneticConfiguration.isInversionMutationEnabled();
            result.pathCost = lowestCost;
            result.populationSize = populationSize;

            results.push_back(result);
            printResults(result);

            if(isGivenPathPromising(lowestCost, bestPopulationSolution.second))
            {
                bestPopulationSolution.first = populationSize;
                bestPopulationSolution.second = lowestCost;
            }
        }
    }
}

void MeasureGenetic::performMeasurementsForBestPopulation(bool useInversionMutation)
{
    std::cout << std::endl;
    std::cout << "Rozpoczecie pomiarow dla najlepszej populacji algorytmu genetycznego " << (useInversionMutation ? "z Inversion Mutation." : 
                                                                                                                    "ze Scramble Mutation.") << std::endl;

    for(auto interval = intervals.begin(); interval != intervals.end(); ++interval)
    {
        for(auto crossoverCoefficient : crossoverCoefficientsForTest)
        {
            int32_t lowestCost = INFINITY;
            double lowestCostRelativeError = 0.0;

            setGeneticConfig(useInversionMutation, mutationCoefficientForTest,
                                                   crossoverCoefficient,
                                                   geneticConfiguration.getReproductionCoefficient(), *interval, bestPopulationSolution.first);

            for(uint32_t counter = 0; counter < numberOfMeasurements; ++counter)
            {
                std::cout << "Pomiar ze stala mutacji " << mutationCoefficientForTest << " dla " << bestPopulationSolution.first << " osobnikow, wartosc krzyzowania " << crossoverCoefficient << ", czas " << static_cast<uint32_t>(*interval) << "s (" << counter + 1 << " z " << numberOfMeasurements << ")..." << std::endl;
                PathWithCost path = genetic.findBestPossibleRoute(*graph);

                if(isGivenPathPromising(path.first, lowestCost))
                {
                    lowestCost = path.first;
                    lowestCostRelativeError = calculateRelativeError(path.first);
                }
            }

            GeneticMeasureResult result;
            result.inversionMutation = geneticConfiguration.isInversionMutationEnabled();
            result.constantMutation = true;
            result.timeInSeconds = *interval;
            result.relativeError = lowestCostRelativeError;
            result.pathCost = lowestCost;
            result.populationSize = bestPopulationSolution.first;

            results.push_back(result);
            printResults(result);
        }
    }

    for(auto interval = intervals.begin(); interval != intervals.end(); ++interval)
    {
        for(auto mutationCoefficient : mutationCoefficientsForTest)
        {
            int32_t lowestCost = INFINITY;
            double lowestCostRelativeError = 0.0;

            setGeneticConfig(useInversionMutation, mutationCoefficient,
                                                   crossoverCoefficientForTest,
                                                   geneticConfiguration.getReproductionCoefficient(), *interval, bestPopulationSolution.first);

            for(uint32_t counter = 0; counter < numberOfMeasurements; ++counter)
            {
                std::cout << "Pomiar ze stala krzyzowania " << crossoverCoefficientForTest << " dla " << bestPopulationSolution.first << " osobnikow, wartosc mutacji " << mutationCoefficient << ", czas " << static_cast<uint32_t>(*interval) << "s (" << counter + 1 << " z " << numberOfMeasurements << ")..." << std::endl;
                PathWithCost path = genetic.findBestPossibleRoute(*graph);

                if(isGivenPathPromising(path.first, lowestCost))
                {
                    lowestCost = path.first;
                    lowestCostRelativeError = calculateRelativeError(path.first);
                }
            }

            GeneticMeasureResult result;
            result.inversionMutation = geneticConfiguration.isInversionMutationEnabled();
            result.constantCrossover = true;
            result.timeInSeconds = *interval;
            result.relativeError = lowestCostRelativeError;
            result.pathCost = lowestCost;
            result.populationSize = bestPopulationSolution.first;

            results.push_back(result);
            printResults(result);
        }
    }
}

void MeasureGenetic::setGeneticConfig(bool inversionMutation, double mutationCoefficient,
                                                              double crossoverCoefficient,
                                                              double reproductionCoefficient,
                                                              double geneticInterval, uint32_t populationSize)
{
    geneticConfiguration.setGeneticInterval(geneticInterval);
    geneticConfiguration.setMutationCoefficient(mutationCoefficient);
    geneticConfiguration.setCrossoverCoefficient(crossoverCoefficient);
    geneticConfiguration.setReproductionCoefficient(reproductionCoefficient);
    geneticConfiguration.setPopulationSize(populationSize);

    if(inversionMutation)
    {
        geneticConfiguration.enableInversionMutation();
    }
    else
    {
        geneticConfiguration.disableInversionMutation();
    }
}

bool MeasureGenetic::isGivenPathPromising(const uint32_t currentCost, const uint32_t upperBound)
{
    return (upperBound == INFINITY) || (currentCost < upperBound);
}

double MeasureGenetic::calculateRelativeError(uint32_t pathCost)
{
    double pathCostAsDouble = static_cast<double>(pathCost);
    return ((pathCost - problemOptimalResult) / problemOptimalResult) * 100.0;
}

void MeasureGenetic::saveResultsToFile()
{
    resultsFile << "Czas" << "\t" << "Blad" << "\t" << "Koszt" << "\t" << "Populacja" << "\t" << "Inversion" << "\t" << "Krzyzowanie" << "\t" << "Mutacja" << std::endl;

    for(auto result : results)
    {
        resultsFile << result.timeInSeconds << "\t" << result.relativeError << "\t" << result.pathCost << "\t" << result.populationSize << "\t" << (result.inversionMutation ? "Tak" : "Nie") << "\t" << (result.constantCrossover ? "Tak" : "Nie") << "\t" << (result.constantMutation ? "Tak" : "Nie") << std::endl;
    }
    resultsFile << std::endl;

    resultsFile.close();
    std::cout << "Wyniki pomyślnie zapisano do pliku." << std::endl;
}

void MeasureGenetic::printResults(const GeneticMeasureResult& result)
{
    std::cout << "Wyniki dla " << result.populationSize << "osobnikow w czasie " << static_cast<uint32_t>(result.timeInSeconds) << "s:" << std::endl;

    std::cout << "\t- blad wzgledny: " << result.relativeError << "%" << std::endl;
    std::cout << "\t- koszt trasy: " << result.pathCost << std::endl << std::endl;
}
