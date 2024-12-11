#include <msg/unitCommands.hpp>

message::move::move(unsigned int dx, unsigned int dy, unsigned int sx, unsigned int sy) :
base(message::type::move), destX(dx), destY(dy), sourceX(sx), sourceY(sy) {}

message::attack::attack(unsigned int dx, unsigned int dy, unsigned int sx, unsigned int sy) :
base(message::type::attack), destX(dx), destY(dy), sourceX(sx), sourceY(sy) {}

message::mine::mine(unsigned int sx, unsigned int sy) :
base(message::type::mine), sourceX(sx), sourceY(sy) {}