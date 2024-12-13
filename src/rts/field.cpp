#include <rts/field.hpp>

#include <cassert>
#include <limits>
#include <algorithm>

#include <rts/unit.hpp>

rts::field::field(const unsigned int& xpos, const unsigned int& ypos) : x(xpos), y(ypos) {}

bool rts::field::empty() const {
    return _unit == nullptr;
}

bool rts::field::hasResource() const {
    return (resourceHp > 0);
}

void rts::field::spawnResource(unsigned int hp) {
    assert(!hasResource());
    resourceHp = (int)hp;
}

void rts::field::mine(int dmg) {
    resourceHp -= dmg;
}

int rts::field::getHp() const {
    return resourceHp;
}

unsigned int rts::field::distance(const field& other) const {
    return std::abs((int)x - (int)other.x) + std::abs((int)y - (int)other.y);
}