#include <Algorithms/TabuSearch/Utils/TabuMove.hpp>


TabuMove::TabuMove()
    : cadency(0)
    , vertexA(0)
    , vertexB(0)
{ }

bool TabuMove::operator==(const TabuMove& rhs)
{
    return (vertexA == rhs.vertexA && vertexB   == rhs.vertexB) ||
           (vertexB   == rhs.vertexA && vertexA == rhs.vertexB);
}

bool TabuMove::operator!=(const TabuMove& rhs)
{
    return !(*this == rhs);
}

bool TabuMove::isEmpty()
{
    return vertexA == 0 && vertexB == 0;
}
