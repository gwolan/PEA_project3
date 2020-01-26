#include <Application/Menu/Actions/BaseAction.hpp>


BaseAction::BaseAction(const std::string& actionName)
    : name(actionName)
{ }

std::string BaseAction::getActionName()
{
    return name;
}
