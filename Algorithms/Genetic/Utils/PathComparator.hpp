#pragma once

#include <vector>
#include <cstdint>


struct PathComparator
{
    private:
    using PathWithCost = std::pair<uint32_t, std::vector<uint32_t>>;

    public:
    bool operator() (const PathWithCost& lhs, const PathWithCost& rhs) const;
};