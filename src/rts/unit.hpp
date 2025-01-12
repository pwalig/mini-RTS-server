#pragma once

namespace rts {
    class player;
    class field;

    class unit {
    public:
        const unsigned int id;
        unsigned int hp;
        field* f;
        field* lastField;
        player* const owner;
        bool movedThisRound = false;

        unit(player* owner_, field* field_, unsigned int id_);

        unit(const unit& other) = delete;
        unit(unit&& other) = delete;
        unit& operator=(const unit& other) = delete;
        unit& operator=(unit&& other) = delete;
        
        void mine();
        void move(field* field_);
        void attack(unit* target);

        void recvDamage(unsigned int dmg);
        ~unit();
    };
}