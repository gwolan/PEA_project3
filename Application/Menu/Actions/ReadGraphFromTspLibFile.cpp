#include <iostream>
#include <Application/Menu/Actions/ReadGraphFromTspLibFile.hpp>
#include <Graph/GraphMatrix.hpp>


ReadGraphFromTspLibFile::ReadGraphFromTspLibFile(const std::string& actionName)
    : BaseAction(actionName)
    , graph(nullptr)
    , parser()
{ }

void ReadGraphFromTspLibFile::init(std::unique_ptr<GraphMatrix>& graphMatrix)
{
    graph = &graphMatrix;

    std::cout << "Podaj nazwe pliku: ";
    std::cin >> fileName;
}

void ReadGraphFromTspLibFile::run()
{
    if(parser.parse(fileName))
    {
        *graph = std::make_unique<GraphMatrix>(parser.getVertexCount());

        fillGraphAdjacencyMatrix();

        std::cout << "Graf pomyslnie wczytany z pliku. Liczba wezlow: " << parser.getVertexCount() << std::endl;
        std::cout << std::endl;
    }
    else
    {
        std::cout << "Blad w odczycie pliku!" << std::endl;
        std::cout << std::endl;
    }
}

void ReadGraphFromTspLibFile::fillGraphAdjacencyMatrix()
{
    for(std::size_t row = 0; row < parser.getVertexCount(); ++row)
    {
        for(std::size_t column = 0; column < parser.getVertexCount(); ++column)
        {
            uint32_t weight = parser.getWeight(row, column);

            if(row != column)
            {
                (*graph)->addWeightToMatrix(row, column, weight);
            }
        }
    }
}
