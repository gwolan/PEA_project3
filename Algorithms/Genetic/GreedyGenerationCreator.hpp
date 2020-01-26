#pragma once

#include <vector>
#include <cstdint>
#include <memory>


class GraphMatrix;

class GreedyGenerationCreator
{
    public:
    GreedyGenerationCreator();
    ~GreedyGenerationCreator() = default;

    std::vector<uint32_t> createGenerationGreedyStyle(std::unique_ptr<GraphMatrix>& graphMatrix);


    private:
    template<class Iterator>
    bool wasVertexAlreadyChecked(Iterator begin, Iterator end, const uint32_t vertex);
    bool isGivenEdgePromising(const uint32_t edgeWeight, const uint32_t upperBound);
    void generatePathBeginning(std::unique_ptr<GraphMatrix>& graphMatrix, std::vector<uint32_t>& path);

    const int32_t INFINITE_VALUE;
    const uint32_t randomMutationPrefixLength;
};
