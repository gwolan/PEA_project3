#pragma once

#include <cstdint>


struct TabuMove
{
    TabuMove();

    bool isEmpty();
    bool operator==(const TabuMove& rhs);
    bool operator!=(const TabuMove& rhs);


    uint32_t cadency;
    uint32_t vertexA;
    uint32_t vertexB;
};
