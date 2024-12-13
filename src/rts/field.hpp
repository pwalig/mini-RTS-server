#pragma once

namespace rts {
    class unit;

    class field {
        int resourceHp = 0;

    public:
        field(const unsigned int& xpos, const unsigned int& ypos);

        bool empty() const;
        bool hasResource() const;
        void spawnResource(unsigned int hp);
        void mine(int dmg);

        int getHp() const;

        unsigned int distance(const field& other) const;
        
        const unsigned int x;
        const unsigned int y;
        unit* _unit = nullptr;
    };
}