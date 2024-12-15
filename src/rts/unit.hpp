#pragma once

namespace rts {
    class player;
    class field;

    class unit {
    public:
        const unsigned int id;
        unsigned int hp;
        field* f;
        player* const owner;
        bool movedThisRound = false;

        unit(player* owner_, field* field_, unsigned int id_);
        void mine();
        void move(field* field_);
        void attack(unit* target);

        void recvDamage(unsigned int dmg);
        ~unit();
    };
}