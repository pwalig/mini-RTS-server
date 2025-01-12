#include <msg/addressUnit.hpp>
#include <rts/game.hpp>
#include <rts/unit.hpp>

message::addressUnitByCoordinates::addressUnitByCoordinates(
    unsigned int xpos, unsigned int ypos
    ) : addressUnit(), x(xpos), y(ypos) {}

rts::unit* message::addressUnitByCoordinates::getUnit(const rts::game* g) const{
    const rts::field* f = g->_board.getField(x, y);
    if (!f) return nullptr;
    return f->_unit;
}

message::addressUnitById::addressUnitById(
    unsigned int id
    ) : addressUnit(), unitId(id) {}
    
rts::unit* message::addressUnitById::getUnit(const rts::game* g) const{
    if (g->unitsById.find(unitId) == g->unitsById.end()) return nullptr;
    return g->unitsById.at(unitId);
}