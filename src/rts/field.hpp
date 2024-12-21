#pragma once

namespace rts {
    class unit;

    class field {
        int resourceHp = 0;

    public:
        field(const unsigned int& xpos, const unsigned int& ypos);

        bool empty() const;
        bool hasResource() const;

        // @returns this field
        rts::field* spawnResource(unsigned int hp);
        
        // @returns this field
        rts::field* mine(int dmg);

        int getHp() const;

        unsigned int distance(const field& other) const;
        
        const unsigned int x;
        const unsigned int y;
        unit* _unit = nullptr;
    };
}