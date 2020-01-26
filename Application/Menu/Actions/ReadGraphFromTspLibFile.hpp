#pragma once

#include <memory>
#include <Application/Menu/Actions/BaseAction.hpp>
#include <Miscellanous/TSPLIBparser.hpp>


class GraphMatrix;

class ReadGraphFromTspLibFile : public BaseAction
{
    public:
    ReadGraphFromTspLibFile(const std::string& actionName);
    ~ReadGraphFromTspLibFile() = default;

    void run();
    void init(std::unique_ptr<GraphMatrix>& graphMatrix);


    private:
    void fillGraphAdjacencyMatrix();

    std::unique_ptr<GraphMatrix>* graph;
    TSPLIBparser parser;
    std::string fileName;
};
