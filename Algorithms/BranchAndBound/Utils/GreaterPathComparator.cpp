#include <Algorithms/BranchAndBound/Utils/GreaterPathComparator.hpp>


bool GreaterPathComparator::operator()(const std::vector<uint32_t>& lhs,
                                       const std::vector<uint32_t>& rhs) const
{
    return (lhs.at(0) > rhs.at(0));
}
