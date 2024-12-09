#pragma once

#include "player.h"

namespace rts {
    class unit {
    public:
        unsigned int hp = 100;
        unsigned int xpos = 0;
        unsigned int ypos = 0;
        player* owner;
    };
}