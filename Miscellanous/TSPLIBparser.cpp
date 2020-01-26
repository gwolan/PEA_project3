#include <iostream>
#include <Miscellanous/TSPLIBparser.hpp>


TSPLIBparser::TSPLIBparser()
    : graph(nullptr)
    , dataName()
    , tspLibDataFileContent()
    , tspLibDataFile()
    , vertexCount(0)
{ }

bool TSPLIBparser::parse(const std::string& fileName)
{
    if(doesFileExist(fileName) && hasCorrectFileFormat())
    {
        readVertexCount();
        graph = std::make_unique<GraphMatrix>(vertexCount);

        fillGraphAdjacencyMatrix();

        tspLibDataFile.close();
        return true;
    }
    else
    {
        std::cout << "PARSER ERROR: Plik \"" << fileName << "\" nie istnieje lub posiada zły format!" << std::endl;
        return false;
    }
}

bool TSPLIBparser::doesFileExist(const std::string& fileName)
{
    tspLibDataFile.open(fileName);

    if(tspLibDataFile.is_open())
    {
        dataName = fileName.substr(0, fileName.find('.'));
        return true;
    }

    return false;
}

bool TSPLIBparser::hasCorrectFileFormat()
{
    tspLibDataFile >> tspLibDataFileContent;

    if(tspLibDataFileContent == "NAME:")
    {
        tspLibDataFile >> tspLibDataFileContent;
        return dataName == tspLibDataFileContent;
    }
    else
    {
        return false;
    }
}

void TSPLIBparser::readVertexCount()
{
    setCarriageToGraphDimension();

    tspLibDataFile >> tspLibDataFileContent;
    vertexCount = std::stoi(tspLibDataFileContent);
}

void TSPLIBparser::fillGraphAdjacencyMatrix()
{
    setCarriageToGraphWeights();

    for(std::size_t row = 0; row < vertexCount; ++row)
    {
        for(std::size_t column = 0; column < vertexCount; ++column)
        {
            tspLibDataFile >> tspLibDataFileContent;
            uint32_t weight = std::stoi(tspLibDataFileContent);

            if(row != column)
            {
                graph->addWeightToMatrix(row, column, weight);
            }
        }
    }
}

void TSPLIBparser::setCarriageToGraphDimension()
{
    do
    {
        tspLibDataFile >> tspLibDataFileContent;
    }
    while(tspLibDataFileContent != "DIMENSION:");
}

void TSPLIBparser::setCarriageToGraphWeights()
{
    do
    {
        tspLibDataFile >> tspLibDataFileContent;
    }
    while(tspLibDataFileContent != "EDGE_WEIGHT_SECTION");
}

uint32_t TSPLIBparser::getVertexCount()
{
    return vertexCount;
}

uint32_t TSPLIBparser::getWeight(uint32_t row, uint32_t column)
{
    return graph->getWeight(row, column);
}
