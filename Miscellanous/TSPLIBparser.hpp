#pragma once

#include <fstream>
#include <string>
#include <cstdint>
#include <memory>
#include <Graph/GraphMatrix.hpp>


class TSPLIBparser
{
    public:
    TSPLIBparser();
    ~TSPLIBparser() = default;

    bool parse(const std::string& fileName);
    uint32_t getWeight(uint32_t row, uint32_t column);
    uint32_t getVertexCount();


    private:
    bool doesFileExist(const std::string& fileName);
    bool hasCorrectFileFormat();
    void readVertexCount();
    void setCarriageToGraphDimension();
    void setCarriageToGraphWeights();
    void fillGraphAdjacencyMatrix();

    std::unique_ptr<GraphMatrix> graph;
    std::string dataName;
    std::string tspLibDataFileContent;
    std::ifstream tspLibDataFile;
    uint32_t vertexCount;
};
