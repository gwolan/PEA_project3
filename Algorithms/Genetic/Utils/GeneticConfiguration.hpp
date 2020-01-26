#pragma once

#include <cstdint>


class GeneticConfiguration
{
    public:
    GeneticConfiguration();
    ~GeneticConfiguration() = default;

    void setGeneticInterval(double intervalInSeconds);
    void enableEdgeMutation();
    void disableEdgeMutation();
    void setMutationCoefficient(double mutationCoeff);
    void setCrossoverCoefficient(double crossoverCoeff);
    void setPopulationSize(uint32_t population);

    bool isEdgeMutationEnabled() const;
    double getGeneticInterval() const;
    double getMutationCoefficient() const;
    double getCrossoverCoefficient() const;
    uint32_t getPopulationSize() const;


    private:
    bool mutateEdges;
    double mutationCoefficient;
    double crossoverCoefficient;
    double geneticInterval;
    uint32_t populationSize;
};
