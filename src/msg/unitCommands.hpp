#pragma once

#include <msg/addressUnit.hpp>

namespace message {
    class move : public addressUnitByCoordinates {
        public:
        const unsigned int destX;
        const unsigned int destY;

        move(
            unsigned int dx, unsigned int dy,
            unsigned int sx, unsigned int sy
            ) : addressUnitByCoordinates(sx, sy), destX(dx), destY(dy) {}
    };
    
    class attack : public addressUnitByCoordinates {
        public:
        const unsigned int destX;
        const unsigned int destY;

        attack(
            unsigned int dx, unsigned int dy,
            unsigned int sx, unsigned int sy
            ) : addressUnitByCoordinates(sx, sy), destX(dx), destY(dy) {}
    };
    
    class mine : public addressUnitByCoordinates {
        public:

        mine(
            unsigned int sx, unsigned int sy
            ) : addressUnitByCoordinates(sx, sy) {}
    };
}