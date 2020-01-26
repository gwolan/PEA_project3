#pragma once

#include <string>
#include <memory>


class GraphMatrix;

class BaseAction
{
    public:
    BaseAction(const std::string& actionName);
    ~BaseAction() = default;

    virtual void run() = 0;
    virtual void init(std::unique_ptr<GraphMatrix>& graphMatrix) = 0;
    std::string getActionName();


    protected:
    std::string name;
};
