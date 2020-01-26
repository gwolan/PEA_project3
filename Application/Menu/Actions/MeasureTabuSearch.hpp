#pragma once

#include <memory>
#include <vector>
#include <fstream>
#include <cstdint>
#include <Application/Menu/Actions/BaseAction.hpp>
#include <Algorithms/TabuSearch/TabuSearch.hpp>
#include <Algorithms/TabuSearch/Utils/TabuConfiguration.hpp>
#include <Application/Menu/Actions/Helpers/TabuSearchMeasureResult.hpp>


class GraphMatrix;

class MeasureTabuSearch : public BaseAction
{
    public:
    MeasureTabuSearch(const std::string& actionName);
    ~MeasureTabuSearch();

    void run();
    void init(std::unique_ptr<GraphMatrix>& graphMatrix);


    private:
    using PathWithCost = std::pair<uint32_t, std::vector<uint32_t>>;

    void performMeasurements(bool useDiversification);
    void setTabuConfig(double tabuInterval,
                       bool   tabuDiversification, uint32_t tabuNumberOfIterations,
                                                   uint32_t tabuCadencyLength);
    double calculateRelativeError(uint32_t pathCost);
    void saveResultsToFile();
    bool isGivenPathPromising(const uint32_t currentCost, const uint32_t upperBound);
    uint32_t getTabuCadencyForGraph(const uint32_t verticiesCount);
    void printResults(const TabuSearchMeasureResult& result);

    std::unique_ptr<GraphMatrix>* graph;
    TabuConfiguration tabuConfiguration;
    TabuSearch tabuSearch;
    std::vector<double> intervals;
    uint32_t numberOfMeasurements;
    uint32_t iterations;
    uint32_t defaultCadency;
    const int32_t INFINITY;

    double problemOptimalResult;
    std::vector<TabuSearchMeasureResult> results;
    std::ofstream resultsFile;
    std::string resultsFileName;
};
