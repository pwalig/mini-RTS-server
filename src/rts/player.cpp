#include "player.h"

#include <cassert>

#include "../msg/name.h"
#include "game.h"

std::unordered_map<std::string, rts::player*> rts::player::playersByName;

rts::player::player(game* game_, client* client_) : _client(client_), mh(client_), _game(game_) {
    mh.onNewMessage = std::bind(&rts::player::handleNewMessage, this, std::placeholders::_1);
    printf("new player created from: ");
    _client->printName();
    printf("\n");
}

void rts::player::handleNewMessage(const message::base* msg) {
    if (msg->typ == message::type::name){
        const message::name* cmsg = dynamic_cast<const message::name*>(msg);
        assert(cmsg);
        if (cmsg->_name != _name && nameTaken(cmsg->_name)) { // other player has that name
            _client->sendToClient({'t', '\n'}); // name taken
        }
        else if (_name == "") setName(cmsg->_name);
        else reName(cmsg->_name);
    }
    else if (msg->typ == message::type::invalid) {
        _client->sendToClient({'i', '\n'}); // invalid message
    }
    else if (msg->typ == message::type::disconnect) {
        _game->deletePlayer(this);
    }
    else if (msg->typ == message::type::joinRequest) {
        _game->tryJoin(this);
    }
    else if (msg->typ == message::type::quit) {
        _game->removePlayerFromRoomOrQueue(this);
    }
}

void rts::player::setName(const std::string& name) {
    assert(playersByName.find(name) == playersByName.end());
    _name = name;
    playersByName.insert({_name, this});
    _client->printName();
    printf(" named self: %s\n", _name.c_str());
    _client->sendToClient({'y', '\n'}); // rename succesfull
}

void rts::player::reName(const std::string& name) {
    printf("%s renamed self to: %s\n", _name.c_str(), name.c_str());
    removeName(_name);
    _name = name;
    playersByName.insert({_name, this});
    _client->sendToClient({'y', '\n'}); // rename succesfull
}

std::string rts::player::getName() const { return _name; }

client* rts::player::getClient() const { return _client; }

void rts::player::removeName(const std::string& name) {
    assert(playersByName.find(name) != playersByName.end());
    playersByName.erase(name);
}

bool rts::player::nameTaken(const std::string& name) {
    return (playersByName.find(name) != playersByName.end());
}

rts::player::~player(){
    if (_name != "") removeName(_name);
    printf("%s left\n", _name.c_str());
}