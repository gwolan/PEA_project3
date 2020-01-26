#include <iostream>
#include <Application/Menu/Actions/ModifyGeneticConfiguration.hpp>
#include <Application/Menu/IOhandler.hpp>


ModifyGeneticConfiguration::ModifyGeneticConfiguration(const std::string& actionName, GeneticConfiguration& geneticConfig)
    : BaseAction(actionName)
    , geneticConfiguration(geneticConfig)
    , subMenuContent()
{ }

void ModifyGeneticConfiguration::init(std::unique_ptr<GraphMatrix>& graphMatrix)
{
    drawSubMenu();
}

void ModifyGeneticConfiguration::run()
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

bool ModifyGeneticConfiguration::handleMenuSelection(std::string choice)
{
    std::cout << std::endl;

    uint32_t selection = std::atoi(choice.c_str());

    switch(selection)
    {
        case 1:
        {
            setEdgeMutation();
        }
        break;

        case 2:
        {
            setMutationCoefficient();
        }
        break;

        case 3:
        {
            setCrossoverCoefficient();
        }
        break;

        case 4:
        {
            setPopulationSize();
        }
        break;

        case 5:
        {
            setGeneticInterval();
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

void ModifyGeneticConfiguration::setEdgeMutation()
{
    if(geneticConfiguration.isEdgeMutationEnabled())
    {
        geneticConfiguration.disableEdgeMutation();
    }
    else
    {
        geneticConfiguration.enableEdgeMutation();
    }
}

void ModifyGeneticConfiguration::setMutationCoefficient()
{
    double mutationCoefficient = 0.0;

    std::cout << "Podaj wspolczynnik mutacji: ";
    std::cin >> mutationCoefficient;

    geneticConfiguration.setMutationCoefficient(mutationCoefficient);
}

void ModifyGeneticConfiguration::setCrossoverCoefficient()
{
    double crossoverCoefficient = 0.0;

    std::cout << "Podaj wspolczynnik krzyzowania: ";
    std::cin >> crossoverCoefficient;

    geneticConfiguration.setCrossoverCoefficient(crossoverCoefficient);
}

void ModifyGeneticConfiguration::setPopulationSize()
{
    uint32_t populationSize = 0;

    std::cout << "Podaj wielkosc populacji: ";
    std::cin >> populationSize;

    geneticConfiguration.setPopulationSize(populationSize);
}

void ModifyGeneticConfiguration::setGeneticInterval()
{
    double geneticInterval = 0.0;

    std::cout << "Podaj czas dzialania algorytmu [s]: ";
    std::cin >> geneticInterval;

    geneticConfiguration.setGeneticInterval(geneticInterval);
}

void ModifyGeneticConfiguration::drawSubMenu()
{
    std::string title(           "Menu konfiguracyjne algorytmu genetycznego:\n");
    std::string optionOne(       "1. Zmien mutacje: " + std::string(geneticConfiguration.isEdgeMutationEnabled() ? "Zamiana krawedzi\n" : "Zamiana wierzcholkow\n"));
    std::string optionTwo(       "2. Ustaw wspolczynnik mutacji: " + std::to_string(geneticConfiguration.getMutationCoefficient()) + " \n");
    std::string optionThree(     "3. Ustaw wspolczynnik krzyzowania: " + std::to_string(geneticConfiguration.getCrossoverCoefficient()) + std::string("\n"));
    std::string optionFour(      "4. Ustaw wielkosc populacji: " + std::to_string(geneticConfiguration.getPopulationSize()) + std::string("\n"));
    std::string optionFive(      "5. Ustaw czas zatrzymania algorytmu [s]: " + std::to_string(geneticConfiguration.getGeneticInterval()) + std::string("\n"));
    std::string returnToMainMenu("0. Powrot do menu glownego\n");
    std::string selection(       "Wybor: ");

    subMenuContent = std::string(title +
                                 optionOne +
                                 optionTwo +
                                 optionThree +
                                 optionFour +
                                 optionFive +
                                 returnToMainMenu +
                                 selection);
}