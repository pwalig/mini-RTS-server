#include <rts/unit.hpp>

#include <cstdio>
#include <cassert>

#include <rts/field.hpp>
#include <rts/player.hpp>
#include <rts/game.hpp>

rts::unit::unit(player* owner_, field* field_) :
    owner(owner_),
    f(field_),
    hp(owner_->getGame()->getUnitHp())
{
    printf("%s got new unit\n", owner->getName().c_str());
    assert(f->empty());
    f->_unit = this;
}

void rts::unit::mine(){
    if (f->hasResource()) {
        f->mine();
        if (f->getHp() <= 0) {
            field* nf = owner->getGame()->_board.closestEmptyField(f);
            if (nf) owner->newUnit(nf);
        }
    }
}
void rts::unit::move(field* field_){
    if (f->distance(*field_) <= 1 && field_->empty()) {
        this->f->_unit = nullptr;
        this->f = field_;
        field_->_unit = this;
    }
}
void rts::unit::attack(unit* target){
    if (target == nullptr) return;
    if (f->distance(*(target->f)) <= 1) {
        target->recvDamage(owner->getGame()->getUnitDamage());
    }
}

void rts::unit::recvDamage(unsigned int dmg){
    hp -= dmg;
    if (hp <= 0) {
        owner->units.erase(this);
        delete this;
    }
}

rts::unit::~unit() {
    printf("%s lost a unit\n", owner->getName().c_str());
    f->_unit = nullptr;
}