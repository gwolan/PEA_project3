#pragma once

#include <string>
#include <memory>
#include <Graph/GraphMatrix.hpp>
#include <Application/Menu/IOhandler.hpp>
#include <Application/Menu/ActionStrategy.hpp>
#include <Algorithms/TabuSearch/Utils/TabuConfiguration.hpp>


class ApplicationFacade
{
    public:
    ApplicationFacade(const std::string& menuContent);
    ~ApplicationFacade() = default;

    void run();
    void printMenu();
    bool readMenuSelection();
    std::string getCurrentMenuSelection();


    private:
    std::unique_ptr<GraphMatrix> graph;
    IOhandler ioHandler;
    TabuConfiguration tabuConfiguration;
    ActionStrategy actionStrategy;
};
