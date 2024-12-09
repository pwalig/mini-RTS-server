#include "board.h"

#include <cassert>


std::vector<char> rts::board::boardStateMessage() const {
    return std::vector<char>(); // TO DO
}
bool rts::board::empty(const unsigned int& xpos, const unsigned int& ypos) const {
    return fields[xpos][ypos].empty();
}
bool rts::board::hasResource(const unsigned int& xpos, const unsigned int& ypos) const {
    return fields[xpos][ypos].hasResource();
}
bool rts::board::canAttack(unit* u, unit* target) const{
    assert(u);
    assert(target);
    return (std::abs((int)(u->xpos - target->xpos)) + std::abs((int)(u->ypos - target->ypos)) <= 1);
}
bool rts::board::canMine(unit* u) const {
    assert(u);
    return fields[u->xpos][u->ypos].hasResource();
}

void rts::board::move(unit* u, const unsigned int& xpos, const unsigned int& ypos) {
    assert(u);
    assert(empty(xpos, ypos));
    fields[u->xpos][u->ypos].removeUnit(u);
    u->xpos = xpos;
    u->ypos = ypos;
    fields[xpos][ypos].placeUnit(u);
}
void rts::board::mine(unit* u) {
    assert(canMine(u));
    fields[u->xpos][u->ypos].mine();

}
void rts::board::attack(unit* u, unit* target) {
    assert(canAttack(u, target));
    unsigned int dist = std::abs((int)(u->xpos - target->xpos)) + std::abs((int)(u->ypos - target->ypos));
    assert(dist <= 1);
    target->hp -= 10;
}