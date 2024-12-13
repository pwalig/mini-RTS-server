#include <rts/field.hpp>

#include <cassert>
#include <limits>

#include <rts/unit.hpp>

rts::field::field(const unsigned int& xpos, const unsigned int& ypos) : x(xpos), y(ypos) {}

bool rts::field::empty() const {
    return _unit == nullptr;
}

bool rts::field::hasResource() const {
    return (resourceHp > 0);
}

void rts::field::spawnResource() {
    assert(!hasResource());
    resourceHp = 100;
}

void rts::field::mine() {
    resourceHp -= 10;
}

int rts::field::getHp() const {
    return resourceHp;
}

rts::field rts::field::invalid = rts::field(std::numeric_limits<unsigned int>::max(), std::numeric_limits<unsigned int>::max());