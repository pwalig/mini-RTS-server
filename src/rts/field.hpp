#pragma once

namespace rts {
    class unit;

    class field {
        int resourceHp = 0;

    public:
        field(const unsigned int& xpos, const unsigned int& ypos);

        bool empty() const;
        bool hasResource() const;
        void spawnResource();
        void mine();

        int getHp() const;
        
        const unsigned int x;
        const unsigned int y;
        unit* _unit = nullptr;

        static field invalid;
    };
}