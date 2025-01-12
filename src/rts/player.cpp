#include <rts/player.hpp>

#include <cassert>

#include <msg/name.hpp>
#include <msg/unitCommands.hpp>
#include <rts/game.hpp>
#include <rts/unit.hpp>
#include <msg/state.hpp>
#include <msg/stringBuffer.hpp>

std::unordered_map<std::string, rts::player*> rts::player::playersByName;

rts::player::player(game* game_, client* client_) : _client(client_), mh(client_), _game(game_) {
    mh.onNewMessage = std::bind(&rts::player::handleNewMessage, this, std::placeholders::_1);

    printf("new player created from: ");
    _client->printName();
    printf("\n");
}

void rts::player::handleNewMessage(const message::base* msg) {
    if (const message::name* cmsg = dynamic_cast<const message::name*>(msg)){
        if (cmsg->_name != _name && nameTaken(cmsg->_name)) { // other player has that name
            _client->sendToClient({'n','\n'}); // name taken
        }
        else if (!_game->nameValid(cmsg->_name)) _client->sendToClient({'n','\n'}); // invalid name
        else if (_name == "") setName(cmsg->_name);
        else reName(cmsg->_name);
    }
    else if (const message::state* cmsg = dynamic_cast<const message::state*>(msg)) {
        if (cmsg->act == message::state::action::disconnect) {
            _game->deletePlayer(this);
        }
        else if (cmsg->act == message::state::action::joinRequest) {
            _game->tryJoin(this);
        }
        else if (cmsg->act == message::state::action::quit) {
            _game->removePlayerFromRoomOrQueue(this);
        }
    }
    else if (const message::command* cmsg = dynamic_cast<const message::command*>(msg)) {
        if (unit* u = cmsg->getUnit(_game)){
            if (u->owner == this) {

                if (const message::move* mmsg = dynamic_cast<const message::move*>(cmsg)) {
                    if (field* f = mmsg->getDestinationField(_game)) u->move(f);
                }
                else if (const message::attack* amsg = dynamic_cast<const message::attack*>(cmsg)) {
                    if (unit* t = amsg->getTarget(_game)) u->attack(t);
                }
                else if (dynamic_cast<const message::mine*>(cmsg)) {
                    u->mine();
                }
            }
        }
    }
}

void rts::player::setName(const std::string& name) {
    assert(playersByName.find(name) == playersByName.end());
    _name = name;
    playersByName.insert({_name, this});
    _client->printName();
    printf(" named self: %s\n", _name.c_str());
    _client->sendToClient({'y','\n'}); // rename succesfull
}

void rts::player::reName(const std::string& name) {
    printf("%s renamed self to: %s\n", _name.c_str(), name.c_str());
    removeName(_name);
    _name = name;
    playersByName.insert({_name, this});
    _client->sendToClient({'y','\n'}); // rename succesfull
}

std::string rts::player::getName() const { return _name; }

client* rts::player::getClient() const { return _client; }
rts::game* rts::player::getGame() const { return _game; }

void rts::player::removeName(const std::string& name) {
    assert(playersByName.find(name) != playersByName.end());
    playersByName.erase(name);
}

bool rts::player::nameTaken(const std::string& name) {
    return (playersByName.find(name) != playersByName.end());
}

void rts::player::removeAllUnits(){
    for (unit* u : units) delete u;
    units.clear();
}

void rts::player::newUnit(field* field_){
    unit* u = new unit(this, field_, _game->getNextUnitId());
    units.insert(u);
    
    std::vector<char> buff = {'u'};
    message::appendStringWDelim(buff, _name, ' ');
    message::appendNumberWDelim(buff, u->id, ' ');
    message::appendNumberWDelim(buff, u->f->x, ' ');
    message::appendNumberWDelim(buff, u->f->y, '\n');
    _game->sendToPlayers(buff);

    _game->tryWin(this);
}

rts::player::~player(){
    removeAllUnits();
    if (_name != "") removeName(_name);
    printf("%s left\n", _name.c_str());
}