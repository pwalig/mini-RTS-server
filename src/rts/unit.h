#pragma once

namespace rts {
    class player;
    class field;

    class unit {
    public:
        unsigned int hp = 100;
        field* f;
        player* const owner;

        unit(player* owner_, field* field_);
        void mine();
        void move(field* field_);
        void attack(unit* target);
        ~unit();
    };
}