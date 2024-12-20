#pragma once

#include<vector>
#include <random>

#include <rts/field.hpp>

namespace rts {
    class board {
        std::vector<std::vector<field>> fields;

        std::mt19937 gen; // mersenne_twister_engine seeded with rd()

        public:
        board(unsigned int x = 256, unsigned int y = 256);
        
        field* getField(const unsigned int& xpos, const unsigned int& ypos);
        const field* getConstField(const unsigned int& xpos, const unsigned int& ypos) const;

        std::vector<field*> resourceFields(bool resource);
        std::vector<field*> emptyFields(bool empty);
        field* randomField();
        field* randomEmptyField(bool empty);
        field* randomResourceField(bool resource);
        field* closestEmptyField(const field* source);

        void spawnResource(unsigned int hp);
        void spawnResources(unsigned int amount, unsigned int hp);

        unsigned int getXdim() const;
        unsigned int getYdim() const;
    };
}