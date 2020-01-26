#pragma once

#include <vector>
#include <cstdint>


struct GreaterPathComparator
{
    bool operator() (const std::vector<uint32_t>& lhs, const std::vector<uint32_t>& rhs) const;
};
