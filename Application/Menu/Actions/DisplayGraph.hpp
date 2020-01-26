#pragma once

#include <memory>
#include <Application/Menu/Actions/BaseAction.hpp>


class GraphMatrix;

class DisplayGraph : public BaseAction
{
    public:
    DisplayGraph(const std::string& actionName);
    ~DisplayGraph() = default;

    void run();
    void init(std::unique_ptr<GraphMatrix>& graphMatrix);


    private:
    std::unique_ptr<GraphMatrix>* graph;
};
