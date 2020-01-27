#pragma once

#include <memory>
#include <Application/Menu/Actions/BaseAction.hpp>
#include <Algorithms/Genetic/Utils/GeneticConfiguration.hpp>


class GraphMatrix;

class ModifyGeneticConfiguration : public BaseAction
{
    public:
    ModifyGeneticConfiguration(const std::string& actionName, GeneticConfiguration& geneticConfig);
    ~ModifyGeneticConfiguration() = default;

    void run();
    void init(std::unique_ptr<GraphMatrix>& graphMatrix);


    private:
    bool handleMenuSelection(std::string choice);
    void drawSubMenu();
    void setInversionMutation();
    void setMutationCoefficient();
    void setCrossoverCoefficient();
    void setPopulationSize();
    void setGeneticInterval();

    GeneticConfiguration& geneticConfiguration;
    std::string subMenuContent;
};
