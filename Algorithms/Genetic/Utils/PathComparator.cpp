#include <Algorithms/Genetic/Utils/PathComparator.hpp>


bool PathComparator::operator() (const PathComparator::PathWithCost& lhs, const PathComparator::PathWithCost& rhs) const
{
    return lhs.first < rhs.first;
}