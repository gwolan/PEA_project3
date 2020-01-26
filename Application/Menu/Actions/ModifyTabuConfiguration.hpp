#pragma once

#include <memory>
#include <Application/Menu/Actions/BaseAction.hpp>
#include <Algorithms/TabuSearch/Utils/TabuConfiguration.hpp>


class GraphMatrix;

class ModifyTabuConfiguration : public BaseAction
{
    public:
    ModifyTabuConfiguration(const std::string& actionName, TabuConfiguration& tabuConfig);
    ~ModifyTabuConfiguration() = default;

    void run();
    void init(std::unique_ptr<GraphMatrix>& graphMatrix);


    private:
    bool handleMenuSelection(char selection);
    void drawSubMenu();
    void setDiversification();
    void setDiversificationCriteria();
    void setCadency();
    void setTabuInterval();

    TabuConfiguration& tabuConfiguration;
    std::string subMenuContent;
};
