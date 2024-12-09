#pragma once

#include<vector>

#include "field.h"

namespace rts {
    class board {
        std::vector<std::vector<field>> fields;

        public:
        std::vector<char> boardStateMessage() const;
        bool empty(const unsigned int& xpos, const unsigned int& ypos) const;
        bool hasResource(const unsigned int& xpos, const unsigned int& ypos) const;
        bool canAttack(unit* u, unit* target) const;
        bool canMine(unit* u) const;

        void move(unit* u, const unsigned int& xpos, const unsigned int& ypos);
        void mine(unit* u);
        void attack(unit* u, unit* target);
    };
}