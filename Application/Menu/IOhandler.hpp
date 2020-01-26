#pragma once

#include <string>


class IOhandler
{
    public:
    IOhandler(const std::string& menuContent);
    ~IOhandler() = default;

    void printMenu();
    bool readMenuSelection();
    std::string getCurrentMenuSelection();
    void setMenuContent(const std::string& menuContent);


    private:
    std::string currentSelection;
    std::string menu;
};
