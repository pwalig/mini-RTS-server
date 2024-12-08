#include "player.h"

#include <cassert>

#include "../msg/name.h"

std::unordered_map<std::string, rts::player*> rts::player::playersByName;

rts::player::player(client* client_) : _client(client_), mh(client_) {
    mh.onNewMessage = std::bind(&rts::player::handleNewMessage, this, std::placeholders::_1);
}

void rts::player::handleNewMessage(const message::base* msg) {
    if (msg->typ == message::type::name){
        const message::name* cmsg = dynamic_cast<const message::name*>(msg);
        assert(cmsg);
        if (cmsg->_name != _name && nameTaken(cmsg->_name)) { // other player has that name
            _client->sendToClient({'n'});
        }
        else if (_name == "") name(cmsg->_name);
        else reName(cmsg->_name);
    }
    else if (msg->typ == message::type::invalid) {
        _client->sendToClient({'i'});
    }
}

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

bool rts::player::nameTaken(const std::string& name) {
    return (playersByName.find(name) != playersByName.end());
}