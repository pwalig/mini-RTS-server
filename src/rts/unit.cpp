#include <rts/unit.hpp>

#include <cstdio>
#include <cassert>

#include <rts/field.hpp>
#include <rts/player.hpp>

rts::unit::unit(player* owner_, field* field_) : owner(owner_), f(field_) {
    printf("%s got new unit\n", owner->getName().c_str());
    assert(f->empty());
    f->_unit = this;
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
        this->f->_unit = nullptr;
        this->f = field_;
        field_->_unit = this;
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
    f->_unit = nullptr;
}