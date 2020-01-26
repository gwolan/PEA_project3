#include <iostream>
#include <Application/Menu/Actions/ModifyTabuConfiguration.hpp>
#include <Application/Menu/IOhandler.hpp>


ModifyTabuConfiguration::ModifyTabuConfiguration(const std::string& actionName, TabuConfiguration& tabuConfig)
    : BaseAction(actionName)
    , tabuConfiguration(tabuConfig)
    , subMenuContent()
{ }

void ModifyTabuConfiguration::init(std::unique_ptr<GraphMatrix>& graphMatrix)
{
    drawSubMenu();
}

void ModifyTabuConfiguration::run()
{
    IOhandler ioHandler(subMenuContent);

    while(ioHandler.getCurrentMenuSelection() != "0")
    {
        drawSubMenu();
        ioHandler.setMenuContent(subMenuContent);
        ioHandler.printMenu();
        ioHandler.readMenuSelection();

        handleMenuSelection(ioHandler.getCurrentMenuSelection());
    }
}

bool ModifyTabuConfiguration::handleMenuSelection(std::string choice)
{
    std::cout << std::endl;

    uint32_t selection = std::atoi(choice.c_str());

    switch(selection)
    {
        case 1:
        {
            setDiversification();
        }
        break;

        case 2:
        {
            setDiversificationCriteria();
        }
        break;

        case 3:
        {
            setCadency();
        }
        break;

        case 4:
        {
            setTabuInterval();
        }
        break;

        case 0: break;

        default:
        {
            std::cout << "Wskazana opcja jest nieprawidłowa." << std::endl;
        }
    }

    std::cout << std::endl;
}

void ModifyTabuConfiguration::setDiversification()
{
    if(tabuConfiguration.isDiversificationEnabled())
    {
        tabuConfiguration.disableDiversification();
    }
    else
    {
        tabuConfiguration.enableDiversification();
    }
}

void ModifyTabuConfiguration::setDiversificationCriteria()
{
    uint32_t newIterations = 0;

    std::cout << "Podaj ilosc iteracji dla kryterium dywersyfikacji: ";
    std::cin >> newIterations;

    tabuConfiguration.setDiversificationCriteria(newIterations);
}

void ModifyTabuConfiguration::setCadency()
{
    uint32_t newCadency = 0;

    std::cout << "Podaj kadencje (0 = auto): ";
    std::cin >> newCadency;

    tabuConfiguration.setTabuCadencyLength(newCadency);
}

void ModifyTabuConfiguration::setTabuInterval()
{
    double newInterval = 0.0;

    std::cout << "Podaj czas dzialania algorytmu [s]: ";
    std::cin >> newInterval;

    tabuConfiguration.setTabuInterval(newInterval);
}

void ModifyTabuConfiguration::drawSubMenu()
{
    std::string title(           "Menu konfiguracyjne Tabu Search:\n");
    std::string optionOne(       "1. Zmien dywersyfikacje: " + std::string(tabuConfiguration.isDiversificationEnabled() ? "Wlaczona\n" : "Wylaczona\n"));
    std::string optionTwo(       "2. Ustaw kryterium dywersyfikacji: " + std::to_string(tabuConfiguration.getNumberOfIterations()) + " iteracji\n");
    std::string optionThree(     "3. Ustaw kadencje na liście Tabu (0 - ustawienia automatyczne): " + std::to_string(tabuConfiguration.getCadencyLength()) + std::string("\n"));
    std::string optionFour(      "4. Ustaw czas zatrzymania algorytmu [s]: " + std::to_string(tabuConfiguration.getTabuInterval()) + std::string("\n"));
    std::string returnToMainMenu("0. Powrot do menu glownego\n");
    std::string selection(       "Wybor: ");

    subMenuContent = std::string(title +
                                 optionOne +
                                 optionTwo +
                                 optionThree +
                                 optionFour +
                                 returnToMainMenu +
                                 selection);
}
