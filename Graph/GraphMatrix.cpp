#include <Graph/GraphMatrix.hpp>
#include <iostream>


GraphMatrix::GraphMatrix(uint32_t verticies)
    : vertexCount(verticies)
{
    initializeEmptyMatrix();
}

void GraphMatrix::initializeEmptyMatrix()
{
    graphMatrix.resize(vertexCount);

    for(auto& row : graphMatrix)
    {
        row.resize(vertexCount);
    }
}

bool GraphMatrix::areCoordsValid(uint32_t row, uint32_t column)
{
    if( row >= vertexCount || column >= vertexCount)
    {
        return false;
    }

    return true;
}

void GraphMatrix::addWeightToMatrix(uint32_t row, uint32_t column, uint32_t weight)
{
    if(areCoordsValid(row, column))
    {
        graphMatrix[row][column] = weight;
    }
}

uint32_t GraphMatrix::getWeight(uint32_t row, uint32_t column)
{
    if(areCoordsValid(row, column))
    {
        return graphMatrix[row][column];
    }
    else
    {
        return 0;
    }
}

uint32_t GraphMatrix::getVertexCount()
{
    return vertexCount;
}

void GraphMatrix::displayGraph()
{
    for(auto& row : graphMatrix)
    {
        for(auto& cell : row)
        {
            std::cout << cell << '\t';
        }

        std::cout << std::endl;
    }
}
