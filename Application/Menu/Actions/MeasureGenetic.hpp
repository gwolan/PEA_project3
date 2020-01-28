#pragma once

#include <memory>
#include <vector>
#include <fstream>
#include <cstdint>
#include <Application/Menu/Actions/BaseAction.hpp>
#include <Algorithms/Genetic/Genetic.hpp>
#include <Algorithms/Genetic/Utils/GeneticConfiguration.hpp>
#include <Application/Menu/Actions/Helpers/GeneticMeasureResult.hpp>


class GraphMatrix;

class MeasureGenetic : public BaseAction
{
    public:
    MeasureGenetic(const std::string& actionName);
    ~MeasureGenetic();

    void run();
    void init(std::unique_ptr<GraphMatrix>& graphMatrix);


    private:
    using PathWithCost = std::pair<uint32_t, std::vector<uint32_t>>;

    void performMeasurements(bool useInversionMutation);
    void performMeasurementsForBestPopulation(bool useInversionMutation);
    void setGeneticConfig(bool inversionMutation, double mutationCoefficient,
                                                  double crossoverCoefficient,
                                                  double reproductionCoefficient,
                                                  double geneticInterval, uint32_t populationSize);
    double calculateRelativeError(uint32_t pathCost);
    void saveResultsToFile();
    bool isGivenPathPromising(const uint32_t currentCost, const uint32_t upperBound);
    void printResults(const GeneticMeasureResult& result);

    std::unique_ptr<GraphMatrix>* graph;
    GeneticConfiguration geneticConfiguration;
    Genetic genetic;

    std::vector<double> intervals;
    std::vector<uint32_t> populationSizes;
    const double crossoverCoefficientForTest;
    const std::vector<double> mutationCoefficientsForTest;
    const double mutationCoefficientForTest;
    const std::vector<double> crossoverCoefficientsForTest;
    uint32_t numberOfMeasurements;
    const int32_t INFINITY;

    double problemOptimalResult;
    std::pair<uint32_t, uint32_t> bestPopulationSolution;
    std::vector<GeneticMeasureResult> results;
    std::ofstream resultsFile;
    std::string resultsFileName;
};
