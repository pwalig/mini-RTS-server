#pragma once

namespace rts {
    class player;
    class field;

    class unit {
    public:
        unsigned int hp;
        field* f;
        player* const owner;
        bool movedThisRound = false;

        unit(player* owner_, field* field_);
        void mine();
        void move(field* field_);
        void attack(unit* target);

        void recvDamage(unsigned int dmg);
        ~unit();
    };
}