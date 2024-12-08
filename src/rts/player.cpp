#include "player.h"
#include <cassert>

std::unordered_map<std::string, rts::player*> rts::player::playersByName;

rts::player::player(client* client_) : _client(client_) {}

void rts::player::name(const std::string& name) {
    assert(playersByName.find(name) == playersByName.end());
    _name = name;
    playersByName.insert({_name, this});
}

void rts::player::reName(const std::string& name) {
    assert(playersByName.find(name) != playersByName.end());
    playersByName.erase(_name);
    _name = name;
    playersByName.insert({_name, this});
}