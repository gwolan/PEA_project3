#include <iostream>
#include <Application/Menu/IOhandler.hpp>


IOhandler::IOhandler(const std::string& menuContent)
    : currentSelection()
    , menu(menuContent)
{ }

void IOhandler::printMenu()
{
    std::cout << menu;
}

std::string IOhandler::getCurrentMenuSelection()
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
