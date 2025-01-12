#include <msg/unitCommands.hpp>
#include <rts/field.hpp>
#include <rts/game.hpp>
#include <rts/unit.hpp>

rts::unit* message::command::getUnit(rts::game* game) const{
    return source->getUnit(game);
}

message::command::command(
    unsigned int sourceId
) : source(new addressUnitById(sourceId)) {}

message::command::command(
    unsigned int sourceX, unsigned int sourceY
) : source(new addressUnitByCoordinates(sourceX, sourceY)) {}

message::command::~command() {
    delete source;
}


rts::field* message::move::getDestinationField(rts::game* game) const {
    return game->_board.getField(destX, destY);
}

message::move::move(
    unsigned int sx, unsigned int sy,
    unsigned int dx, unsigned int dy
) : command(sx, sy), destX(dx), destY(dy) {}

message::move::move(
    unsigned int si,
    unsigned int dx, unsigned int dy
) : command(si), destX(dx), destY(dy) {}


rts::unit* message::attack::getTarget(rts::game* game) const {
    return target->getUnit(game);
}

message::attack::attack(
    unsigned int sx, unsigned int sy,
    unsigned int tx, unsigned int ty
) : command(sx, sy), target(new addressUnitByCoordinates(tx, ty)) {}
            
message::attack::attack(
    unsigned int si,
    unsigned int ti
) : command(si), target(new addressUnitById(ti)) {}

message::attack::~attack(){
    delete target;
}


message::mine::mine(
    unsigned int sx, unsigned int sy
) : command(sx, sy) {}

message::mine::mine(
    unsigned int si
) : command(si) {}
