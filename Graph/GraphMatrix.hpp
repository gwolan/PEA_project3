#pragma once

#include <vector>
#include <cstdint>


class GraphMatrix
{
    public:
    GraphMatrix(uint32_t verticies);
    ~GraphMatrix() = default;

    void addWeightToMatrix(uint32_t row, uint32_t column, uint32_t weight);
    uint32_t getWeight(uint32_t row, uint32_t column);
    uint32_t getVertexCount();
    void displayGraph();


    private:
    bool areCoordsValid(uint32_t row, uint32_t column);
    void initializeEmptyMatrix();

    uint32_t vertexCount;
    std::vector<std::vector<uint32_t>> graphMatrix;
};
