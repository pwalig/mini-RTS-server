#include <rts/field.hpp>

#include <cassert>

#include <rts/unit.hpp>

rts::field::field(const unsigned int& xpos, const unsigned int& ypos) : x(xpos), y(ypos) {}

bool rts::field::empty() const {
    return _unit == nullptr;
}

bool rts::field::hasResource() const {
    return (resourceHp > 0);
}

void rts::field::placeUnit(unit* u) {
    _unit = u;
    _unit->f = this;
}

void rts::field::removeUnit(unit* u) {
    assert(_unit == u);
    _unit = nullptr;
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