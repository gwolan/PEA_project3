#include <Application/ApplicationFacade.hpp>


ApplicationFacade::ApplicationFacade(const std::string& menuContent)
    : graph(nullptr)
    , ioHandler(menuContent)
    , tabuConfiguration()
    , actionStrategy(graph, tabuConfiguration)
{ }

void ApplicationFacade::printMenu()
{
    ioHandler.printMenu();
}

bool ApplicationFacade::readMenuSelection()
{
    return ioHandler.readMenuSelection();
}

char ApplicationFacade::getCurrentMenuSelection()
{
    return ioHandler.getCurrentMenuSelection();
}

void ApplicationFacade::run()
{
    if(actionStrategy.selectAction(ioHandler.getCurrentMenuSelection()))
    {
        actionStrategy.executeAction();
    }
}
