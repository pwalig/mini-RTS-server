#include "game.h"

#include <cassert>

#include "player.h"
#include "unit.h"
#include <string.h>
#include "../msg/stringBuffer.h"

rts::game::game(const char *port) : _server(port) {
    _server.onNewClient = std::bind(&rts::game::handleNewClient, this, std::placeholders::_1);
}

void rts::game::handleNewClient(client* client_) {
    player* pl = new player(this, client_);
    allPlayers.insert(pl);
}

void rts::game::loopLogic(){
    // spawn resource
    if (rand() % 10 == 0) _board.spawnResource();

    // sent updates to clients
    std::vector<char> buff = {'p', ' '};
    message::appendNumberWDelim(buff, activePlayers.size(), '\n'); // amount of players

    // players
    for (player* p : activePlayers) {
        message::appendStringWDelim(buff, p->getName(), ' '); // player name
        message::appendNumberWDelim(buff, p->units.size(), '\n'); // amount of units
        
        for (unit* u : p->units) {
            message::appendNumberWDelim(buff, u->f->x, ' ');
            message::appendNumberWDelim(buff, u->f->y, ' ');
            message::appendNumberWDelim(buff, u->hp, '\n');
        }
    }

    // resources
    buff.push_back('r');
    buff.push_back(' ');
    std::vector<rts::field*> resources = _board.resourceFields(true);
    message::appendNumberWDelim(buff, resources.size(), '\n'); // amount of resources
    for (field* f : resources) {
        message::appendNumberWDelim(buff, f->x, ' ');
        message::appendNumberWDelim(buff, f->y, ' ');
        message::appendNumberWDelim(buff, f->getHp(), '\n');
    }

    for (player* p : activePlayers){
        p->getClient()->sendToClient(buff);
    }
}

void rts::game::clearRoom() {
    for (player* pl : activePlayers) { // remove all player from game room
        activePlayers.erase(pl);
        pl->removeAllUnits();
    }
    _board = board(); // reset board
    _server.loopLogic = [](){};
}

void rts::game::startGame() {
    _board.spawnResources(startResources);
    while(!queuedPlayers.empty() && activePlayers.size() < maxPlayers){
        moveQueuedPlayerToRoom();
    }
    _server.loopLogic = std::bind(&rts::game::loopLogic, this);
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
        if (activePlayers.empty()) clearRoom();
    }
    auto it = std::find(queuedPlayers.begin(), queuedPlayers.end(), pl);
    if (it != queuedPlayers.end()) {
        queuedPlayers.erase(it);
    }
}

void rts::game::run() {
    _server.loop(millis);
}

void rts::game::tryJoin(player* pl){
    if (pl->getName() == "") {
        pl->getClient()->sendToClient({'n'}); // client unnamed
    }
    else if (activePlayers.size() < maxPlayers) {
        if (activePlayers.empty()) startGame();
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