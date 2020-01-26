#pragma once

#include <vector>
#include <cstdint>
#include <memory>


class GraphMatrix;

class LocalSearch
{
    public:
    LocalSearch();
    ~LocalSearch() = default;

    std::vector<uint32_t> findStartingPathGreedyStyle(std::unique_ptr<GraphMatrix>& graphMatrix);
    std::vector<uint32_t> findStartingPathRandomly(std::unique_ptr<GraphMatrix>& graphMatrix);


    private:
    template<class Iterator>
    bool wasVertexAlreadyChecked(Iterator begin, Iterator end, const uint32_t vertex);
    bool isGivenEdgePromising(const uint32_t edgeWeight, const uint32_t upperBound);

    const int32_t INFINITE_VALUE;
};
