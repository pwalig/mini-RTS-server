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

        std::vector<char> boardStateMessage() const;
        
        field& getField(const unsigned int& xpos, const unsigned int& ypos);

        std::vector<field*> resourceFields(bool resource);
        std::vector<field*> emptyFields(bool empty);
        field* randomField();
        field* randomEmptyField(bool empty);
        field* randomResourceField(bool resource);

        void spawnResource();
        void spawnResources(const unsigned int& amount);
    };
}