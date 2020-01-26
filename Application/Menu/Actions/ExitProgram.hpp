#pragma once

#include <Application/Menu/Actions/BaseAction.hpp>


class ExitProgram : public BaseAction
{
    public:
    ExitProgram(const std::string& actionName);

    void run();
    void init(std::unique_ptr<GraphMatrix>& graphMatrix);
};
