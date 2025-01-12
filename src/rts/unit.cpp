#include <rts/unit.hpp>

#include <cstdio>
#include <cassert>

#include <rts/field.hpp>
#include <rts/player.hpp>
#include <rts/game.hpp>
#include <msg/stringBuffer.hpp>

rts::unit::unit(player* owner_, field* field_, unsigned int id_) :
    id(id_),
    hp(owner_->getGame()->getUnitHp()),
    f(field_), lastField(field_),
    owner(owner_)
{
    printf("%s got new unit\n", owner->getName().c_str());
    assert(f->empty());
    f->_unit = this;
    owner_->getGame()->unitsById.insert({id, this});
}

void rts::unit::mine(){
    if (!movedThisRound && f->hasResource()) {
        f->mine(owner->getGame()->getUnitDamage());

        std::vector<char> buff = {'d'};
        message::appendNumberWDelim(buff, id, ' ');
        message::appendNumberWDelim(buff, f->getHp(), '\n');
        owner->getGame()->sendToPlayers(buff);

        if (f->getHp() <= 0) {
            field* nf = owner->getGame()->_board.closestEmptyField(f);
            if (nf) owner->newUnit(nf);
        }
        movedThisRound = true;
    }
}
void rts::unit::move(field* field_){
    if (!movedThisRound && f->distance(*field_) <= 1 && field_->empty()) {
        
        std::vector<char> buff = {'m'};
        message::appendNumberWDelim(buff, id, ' ');
        message::appendNumberWDelim(buff, field_->x, ' ');
        message::appendNumberWDelim(buff, field_->y, '\n');
        owner->getGame()->sendToPlayers(buff);


        this->f->_unit = nullptr;
        this->f = field_;
        field_->_unit = this;
        movedThisRound = true;
    }
}
void rts::unit::attack(unit* target){
    if (target == nullptr) return;
    if (!movedThisRound && (f->distance(*(target->f)) <= 1 || f->distance(*(target->lastField)) <= 1)) {

        std::vector<char> buff = {'a'};
        message::appendNumberWDelim(buff, id, ' ');
        message::appendNumberWDelim(buff, target->id, ' ');
        message::appendNumberWDelim(buff, target->hp - owner->getGame()->getUnitDamage(), '\n');
        owner->getGame()->sendToPlayers(buff);

        target->recvDamage(owner->getGame()->getUnitDamage());
        movedThisRound = true;
    }
}

void rts::unit::recvDamage(unsigned int dmg){
    if (dmg >= hp) hp = 0;
    else hp -= dmg;
    if (hp <= 0) {
        owner->units.erase(this);
        if (owner->units.empty()) {
            owner->getGame()->playerLostAllUnits(owner);
        }
        delete this;
    }
}

rts::unit::~unit() {
    printf("%s lost a unit\n", owner->getName().c_str());
    f->_unit = nullptr;
    owner->getGame()->unitsById.erase(id);
}