#include <iostream>
#include <Application/Menu/IOhandler.hpp>


IOhandler::IOhandler(const std::string& menuContent)
    : currentSelection(0)
    , menu(menuContent)
{ }

void IOhandler::printMenu()
{
    std::cout << menu;
}

char IOhandler::getCurrentMenuSelection()
{
    return currentSelection;
}

bool IOhandler::readMenuSelection()
{
    std::cin >> currentSelection;
}

void IOhandler::setMenuContent(const std::string& menuContent)
{
    menu = menuContent;
}
