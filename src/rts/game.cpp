#include "game.h"

#include <cassert>

#include "player.h"
#include "unit.h"
#include <string.h>

rts::game::game(const char *port) : _server(port) {
    _server.onNewClient = std::bind(&rts::game::handleNewClient, this, std::placeholders::_1);
    _server.loopLogic = std::bind(&rts::game::loopLogic, this);
}

void rts::game::handleNewClient(client* client_) {
    player* pl = new player(this, client_);
    allPlayers.insert(pl);
}

void rts::game::loopLogic(){
    // spawn resource
    if (rand() % 10 == 0) _board.spawnResource();

    // sent updates to clients
    std::vector<char> buff = {'s', 'p'};
    buff.push_back((char)(activePlayers.size() + 48));

    // players
    for (player* p : activePlayers) {
        unsigned long it = buff.size();
        std::string name = p->getName();
        buff.resize(buff.size() + name.size());
        memcpy(&buff.data()[it], (void*)name.c_str(), name.size());
        buff.push_back('\n');
        
        buff.push_back((char)(p->units.size() + 48));
        for (unit* u : p->units) {
            buff.push_back('u');
            buff.push_back((char)u->f->x);
            buff.push_back((char)u->f->y);
            buff.push_back((char)u->hp);
            buff.push_back('\n');
        }
    }

    // resources
    for (field* f : _board.resourceFields(true)) {
        buff.push_back('f');
        buff.push_back((char)f->x);
        buff.push_back((char)f->y);
        buff.push_back((char)f->getHp());
        buff.push_back('\n');
    }

    buff.push_back('s');

    for (player* p : activePlayers){
        p->getClient()->sendToClient(buff);
    }
}

void rts::game::addPlayerToRoom(player* pl) {
    assert(activePlayers.size() < maxPlayers);
    activePlayers.insert(pl);
    pl->newUnit(_board.randomEmptyField(true)); // add first unit for the player to control
    pl->getClient()->sendToClient({'a'}); // joined active group
}

void rts::game::addPlayerToQueue(player* pl) {
    queuedPlayers.push_back(pl);
    pl->getClient()->sendToClient({'q'}); // put into queue
}

void rts::game::moveQueuedPlayerToRoom() {
    assert(!queuedPlayers.empty());
    addPlayerToRoom(queuedPlayers.front());
    queuedPlayers.pop_front();
}

void rts::game::removePlayerFromRoomOrQueue(player* pl) {
    if (activePlayers.find(pl) != activePlayers.end()) {
        activePlayers.erase(pl);
        pl->removeAllUnits();
        if (!queuedPlayers.empty()) moveQueuedPlayerToRoom();
    }
    auto it = std::find(queuedPlayers.begin(), queuedPlayers.end(), pl);
    if (it != queuedPlayers.end()) {
        queuedPlayers.erase(it);
    }
}

void rts::game::run() {
    _server.loop(1000);
}

void rts::game::tryJoin(player* pl){
    if (pl->getName() == "") {
        pl->getClient()->sendToClient({'n'}); // client unnamed
    }
    else if (activePlayers.size() < maxPlayers) {
        addPlayerToRoom(pl);
    }
    else {
        addPlayerToQueue(pl);
    }
}

void rts::game::deletePlayer(player* pl){
    allPlayers.erase(pl);
    removePlayerFromRoomOrQueue(pl);
    delete pl;
}

rts::game::~game() {
    for (player* p : allPlayers){
        delete p;
    }
}