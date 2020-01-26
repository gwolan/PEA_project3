#pragma once

#include <string>


class IOhandler
{
    public:
    IOhandler(const std::string& menuContent);
    ~IOhandler() = default;

    void printMenu();
    bool readMenuSelection();
    char getCurrentMenuSelection();
    void setMenuContent(const std::string& menuContent);


    private:
    char currentSelection;
    std::string menu;
};
