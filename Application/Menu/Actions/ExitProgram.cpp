#include <Application/Menu/Actions/ExitProgram.hpp>
#include <iostream>


ExitProgram::ExitProgram(const std::string& actionName)
    : BaseAction(actionName)
{ }

void ExitProgram::run() { }

void ExitProgram::init(std::unique_ptr<GraphMatrix>& graphMatrix) { }
