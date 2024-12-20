#include <msg/addressUnit.hpp>
#include <rts/game.hpp>
#include <rts/unit.hpp>

message::addressUnitByCoordinates::addressUnitByCoordinates(
    unsigned int xpos, unsigned int ypos
    ) : addressUnit(), x(xpos), y(ypos) {}

rts::unit* message::addressUnitByCoordinates::getUnit(const rts::game* g) const{
    const rts::field* f = g->_board.getConstField(x, y);
    if (!f) return nullptr;
    return f->_unit;
}

message::addressUnitById::addressUnitById(
    unsigned int id
    ) : addressUnit(), unitId(id) {}