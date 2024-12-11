#pragma once

#include "base.h"

namespace message {
    class move : public base {
        public:
        const unsigned int destX;
        const unsigned int destY;
        const unsigned int sourceX;
        const unsigned int sourceY;

        move(unsigned int dx, unsigned int dy, unsigned int sx, unsigned int sy);
    };
    
    class attack : public base {
        public:
        const unsigned int destX;
        const unsigned int destY;
        const unsigned int sourceX;
        const unsigned int sourceY;

        attack(unsigned int dx, unsigned int dy, unsigned int sx, unsigned int sy);
    };
    
    class mine : public base {
        public:
        const unsigned int sourceX;
        const unsigned int sourceY;

        mine(unsigned int sx, unsigned int sy);
    };
}