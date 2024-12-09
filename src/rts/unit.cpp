#include "unit.h"

#include "stdio.h"
#include <cassert>

#include "field.h"
#include "player.h"

rts::unit::unit(player* owner_, field* field_) : owner(owner_), f(field_) {
    printf("%s got new unit\n", owner->getName().c_str());
    assert(f->empty());
    f->placeUnit(this);
}

void rts::unit::mine(){
    if (f->hasResource()) {
        f->mine();
        owner->getClient()->sendToClient({'y'});
    } else {
        owner->getClient()->sendToClient({'n'});
    }
}
void rts::unit::move(field* field_){
    if (std::abs((int)(this->f->x - field_->x)) + std::abs((int)(this->f->y - field_->y)) <= 1
    && field_->empty()) {
        f->removeUnit(this);
        field_->placeUnit(this);
        owner->getClient()->sendToClient({'y'});
    } else {
        owner->getClient()->sendToClient({'n'});
    }
}
void rts::unit::attack(unit* target){
    if (std::abs((int)(this->f->x - target->f->x)) + std::abs((int)(this->f->y - target->f->y)) <= 1) {
        target->hp -= 10;
        owner->getClient()->sendToClient({'y'});
    } else {
        owner->getClient()->sendToClient({'n'});
    }
}

rts::unit::~unit() {
    printf("%s lost a unit\n", owner->getName().c_str());
    f->removeUnit(this);
}