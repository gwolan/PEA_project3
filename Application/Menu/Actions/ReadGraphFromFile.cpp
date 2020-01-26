#include <iostream>
#include <Application/Menu/Actions/ReadGraphFromFile.hpp>
#include <Graph/GraphMatrix.hpp>


ReadGraphFromFile::ReadGraphFromFile(const std::string& actionName)
    : BaseAction(actionName)
    , graph(nullptr)
    , tspDataFileContent()
    , fileName()
    , tspDataFile()
    , vertexCount(0)
{ }

void ReadGraphFromFile::init(std::unique_ptr<GraphMatrix>& graphMatrix)
{
    graph = &graphMatrix;

    openFile();
    readVertexCountIfPossible();
}

void ReadGraphFromFile::run()
{
    if(tspDataFile.is_open())
    {
        *graph = std::make_unique<GraphMatrix>(vertexCount);

        fillGraphAdjacencyMatrix();
        tspDataFile.close();

        std::cout << "Graf pomyslnie wczytany z pliku. Liczba wezlow: " << vertexCount << std::endl;
        std::cout << std::endl;
    }
    else
    {
        std::cout << "Plik \"" << fileName << "\" nie istnieje!" << std::endl;
        std::cout << std::endl;
    }
}

void ReadGraphFromFile::openFile()
{
    std::cout << "Podaj nazwe pliku: ";
    std::cin >> fileName;

    tspDataFile.open(fileName);
}

void ReadGraphFromFile::readVertexCountIfPossible()
{
    if(tspDataFile.is_open())
    {
        tspDataFile >> tspDataFileContent;
        vertexCount = std::atoi(tspDataFileContent.c_str());
    }
}

uint32_t ReadGraphFromFile::readWeight()
{
    tspDataFile >> tspDataFileContent;
    return std::atoi(tspDataFileContent.c_str());
}

void ReadGraphFromFile::fillGraphAdjacencyMatrix()
{
    for(std::size_t row = 0; row < vertexCount; ++row)
    {
        for(std::size_t column = 0; column < vertexCount; ++column)
        {
            uint32_t weight = readWeight();
            if(row != column)
            {
                (*graph)->addWeightToMatrix(row, column, weight);
            }
        }
    }
}
