#pragma once

#include <cstdint>


class GeneticConfiguration
{
    public:
    GeneticConfiguration();
    ~GeneticConfiguration() = default;

    void setGeneticInterval(double intervalInSeconds);
    void enableInversionMutation();
    void disableInversionMutation();
    void setMutationCoefficient(double mutationCoeff);
    void setCrossoverCoefficient(double crossoverCoeff);
    void setReproductionCoefficient(double reporoductionCoeff);
    void setPopulationSize(uint32_t population);

    bool isInversionMutationEnabled() const;
    double getGeneticInterval() const;
    double getMutationCoefficient() const;
    double getCrossoverCoefficient() const;
    double getReproductionCoefficient() const;
    uint32_t getPopulationSize() const;


    private:
    bool inversionMutation;
    double mutationCoefficient;
    double crossoverCoefficient;
    double reporoductionCoefficient;
    double geneticInterval;
    uint32_t populationSize;
};
