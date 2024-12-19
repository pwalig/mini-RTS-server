#pragma once

#include <msg/base.hpp>

namespace message {
    class addressUnitById : public base {
    public:
        const unsigned int unitId;
        
        addressUnitById(unsigned int id) : base(), unitId(id) {}
    };

    class addressUnitByCoordinates : public base {
    public:
        const unsigned int x, y;

        addressUnitByCoordinates(unsigned int xpos, unsigned int ypos) : base(), x(xpos), y(ypos) {}
    };
}