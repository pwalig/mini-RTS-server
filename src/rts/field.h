#pragma once
#include "unit.h"

namespace rts {
    class field {
        int resourceHp = 0;
        unit* _unit = nullptr;

    public:
        bool empty() const;
        bool hasResource() const;

        void placeUnit(unit* u);
        void removeUnit(unit* u);
        void spawnResource();
        void mine();
    };
}