#pragma once

#include<vector>
#include <random>

#include <rts/field.hpp>

namespace rts {
    class board {
        std::vector<std::vector<field>> fields;

        std::mt19937 gen;

        public:
        board(unsigned int x = 256, unsigned int y = 256);
        
        field* getField(unsigned int xpos, unsigned int ypos);
        const field* getField(unsigned int xpos, unsigned int ypos) const;
        
        std::vector<field*> resourceFields(bool resource);
        std::vector<const field*> constResourceFields(bool resource) const;
        std::vector<field*> emptyFields(bool empty);
        field* randomField();
        field* randomEmptyField(bool empty);
        field* randomResourceField(bool resource);
        field* closestEmptyField(const field* source);

        field* spawnResource(unsigned int hp);
        void spawnResources(unsigned int amount, unsigned int hp);

        unsigned int getXdim() const;
        unsigned int getYdim() const;
    };
}