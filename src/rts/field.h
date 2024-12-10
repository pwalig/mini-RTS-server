#pragma once

namespace rts {
    class unit;

    class field {
        int resourceHp = 0;
        unit* _unit = nullptr;

    public:
        field(const unsigned int& xpos, const unsigned int& ypos);

        bool empty() const;
        bool hasResource() const;

        void placeUnit(unit* u);
        void removeUnit(unit* u);
        void spawnResource();
        void mine();

        int getHp() const;
        
        const unsigned int x;
        const unsigned int y;
    };
}