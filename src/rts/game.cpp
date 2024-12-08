#include "game.h"

rts::game::game(const char *port) : _server(port) {
    _server.onNewClient = std::bind(&rts::game::handleNewClient, this, std::placeholders::_1);
    _server.loopLogic = std::bind(&rts::game::loopLogic, this);
}

void rts::game::handleNewClient(client* client_) {
    player* pl = new player(this, client_);
    allPlayers.insert(pl);
}

void rts::game::loopLogic(){
    char msg[8] = "update\n";
    std::vector<char> buff(msg, msg + 8);

    for (player* p : activePlayers){
        p->getClient()->sendToClient(buff);
    }
}

void rts::game::run() {
    _server.loop(1000);
}

void rts::game::tryJoin(player* pl){
    if (activePlayers.size() < maxPlayers) {
        activePlayers.insert(pl);
        pl->getClient()->sendToClient({'a', '\n'}); // joined active group
    }
    else {
        queuedPlayers.push_back(pl);
        pl->getClient()->sendToClient({'q', '\n'}); // put into queue
    }
}

void rts::game::deletePlayer(player* pl){
    allPlayers.erase(pl);
    if (activePlayers.find(pl) != activePlayers.end()) {
        activePlayers.erase(pl);
    }
    auto it = std::find(queuedPlayers.begin(), queuedPlayers.end(), pl);
    if (it != queuedPlayers.end()) {
        queuedPlayers.erase(it);
    }
    delete pl;
}

rts::game::~game() {
    for (player* p : allPlayers){
        delete p;
    }
}