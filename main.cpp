#include <Application/ApplicationFacade.hpp>


int main()
{
    std::string menuContent(std::string("\n====================\n") +
                            std::string("PEA Projekt nr 3: Travelling Salesman Problem\n") +
                            std::string("Autor: Grzegorz Wolanski\n\n") +
                            std::string("Menu:\n") +
                            std::string( "1. Wczytaj graf z pliku.\n") +
                            std::string( "2. Wczytaj graf z pliku TSPLIB.\n") +
                            std::string( "3. Wygeneruj losowy graf.\n") +
                            std::string( "4. Wyświetl graf.\n") +
                            std::string( "5. Wykonaj przegląd zupełny.\n") +
                            std::string( "6. Wykonaj algorytm podzialu i ograniczen.\n") +
                            std::string( "7. Wykonaj algorytm Tabu Search.\n") +
                            std::string( "8. Wykonaj algorytm genetyczny.\n") +
                            std::string( "9. Ustaw parametry dla Tabu Search.\n") +
                            std::string("10. Ustaw parametry dla algorytmu genetycznego.\n") +
                            std::string("11. Testy dla algorytmu genetycznego.\n") +
                            std::string("0. Wyjscie.\n\n") +
                            std::string("Wybor: "));


    ApplicationFacade application(menuContent);

    while(application.getCurrentMenuSelection() != "0")
    {
        application.printMenu();
        application.readMenuSelection();
        application.run();
    }

    return 0;
}
