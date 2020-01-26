#pragma once

#include <vector>
#include <memory>
#include <cstdint>


class GraphMatrix;

class BruteForce
{
    public:
    BruteForce();
    ~BruteForce() = default;

    std::vector<uint32_t> performBruteForceOnGraph(std::unique_ptr<GraphMatrix>& graphMatrix);


    private:
    using VertexToDistanceMapping = std::pair<uint32_t, uint32_t>;
    using Path = std::vector<VertexToDistanceMapping>;

    void findAllPossiblePaths(std::vector<uint32_t>& verticies, std::vector<Path>& listOfPossiblePaths);
    Path& findShortestPath(std::vector<Path>& listOfPossiblePaths);
    uint32_t calculatePathsCost(Path& currentPath);

    std::unique_ptr<GraphMatrix>* graph;
};
