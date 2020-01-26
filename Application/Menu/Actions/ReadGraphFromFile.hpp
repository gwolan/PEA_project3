#pragma once

#include <fstream>
#include <memory>
#include <Application/Menu/Actions/BaseAction.hpp>


class GraphMatrix;

class ReadGraphFromFile : public BaseAction
{
    public:
    ReadGraphFromFile(const std::string& actionName);
    ~ReadGraphFromFile() = default;

    void run();
    void init(std::unique_ptr<GraphMatrix>& graphMatrix);


    private:
    void openFile();
    void readVertexCountIfPossible();
    uint32_t readWeight();
    void fillGraphAdjacencyMatrix();

    std::unique_ptr<GraphMatrix>* graph;
    std::string tspDataFileContent;
    std::string fileName;
    std::ifstream tspDataFile;
    uint32_t vertexCount;
};
