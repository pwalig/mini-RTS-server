#include <rts/game.hpp>

#include <cassert>

#include <rts/player.hpp>
#include <rts/unit.hpp>
#include <string.h>
#include <msg/stringBuffer.hpp>
#include <fstream>

std::unordered_map<std::string, std::function<void(rts::game*, std::ifstream&)>> rts::game::configValueHandlers = {
    {"millis", [](rts::game* g, std::ifstream& f){ f >> g->millis; }},
    {"maxPlayers", [](rts::game* g, std::ifstream& f){ f >> g->maxPlayers; }},
    {"boardX", [](rts::game* g, std::ifstream& f){ f >> g->boardX; }},
    {"boardY", [](rts::game* g, std::ifstream& f){ f >> g->boardY; }},
    {"unitsToWin", [](rts::game* g, std::ifstream& f){ f >> g->unitsToWin; }},
    {"startResources", [](rts::game* g, std::ifstream& f){ f >> g->startResources; }},
    {"resourceHp", [](rts::game* g, std::ifstream& f){ f >> g->resourceHp; }},
    {"unitHp", [](rts::game* g, std::ifstream& f){ f >> g->unitHp; }},
    {"unitDamage", [](rts::game* g, std::ifstream& f){ f >> g->unitDamage; }},
    {"allowedNameCharacters", [](rts::game* g, std::ifstream& f){ f >> g->allowedNameCharacters; }}
}; // i wish there was reflection system in c++

rts::game::game(const char *port, const char* configFile) : _server(port) {
    _server.onNewClient = std::bind(&rts::game::handleNewClient, this, std::placeholders::_1);

    if (configFile != nullptr) {
        std::ifstream fin(configFile);
        std::string name;
        if (fin.is_open()) {
            while (!fin.eof()) {
                fin >> name;
                if (configValueHandlers.find(name) != configValueHandlers.end())
                    configValueHandlers[name](this, fin);
                else throw std::logic_error("invalid property name");
            }
        }
    }
}

// ========== MESSAGES =========

std::vector<char> rts::game::configMessage() const {
    std::vector<char> buff = {'c'};
    message::appendNumberWDelim(buff, millis, ' ');
    message::appendNumberWDelim(buff, maxPlayers, ' ');
    message::appendNumberWDelim(buff, boardX, ' ');
    message::appendNumberWDelim(buff, boardY, ' ');
    message::appendNumberWDelim(buff, unitsToWin, ' ');
    message::appendNumberWDelim(buff, startResources, ' ');
    message::appendNumberWDelim(buff, resourceHp, ' ');
    message::appendNumberWDelim(buff, unitHp, ' ');
    message::appendNumberWDelim(buff, unitDamage, ' ');
    message::appendStringWDelim(buff, allowedNameCharacters, '\n');
    return buff;
}

std::vector<char> rts::game::boardStateMessage() const {
    std::vector<char> buff = {'p'};
    message::appendNumberWDelim(buff, activePlayers.size(), ';'); // amount of players

    // players
    for (player* p : activePlayers) {
        message::appendStringWDelim(buff, p->getName(), ' '); // player name
        message::appendNumberWDelim(buff, p->units.size(), ','); // amount of units
        
        for (unit* u : p->units) {
            message::appendNumberWDelim(buff, u->id, ' ');
            message::appendNumberWDelim(buff, u->f->x, ' ');
            message::appendNumberWDelim(buff, u->f->y, ' ');
            message::appendNumberWDelim(buff, u->hp, ',');
        }

        buff.push_back(';');
    }
    buff.push_back('\n');
    // resources
    buff.push_back('r');
    std::vector<const rts::field*> resources = _board.constResourceFields(true);
    message::appendNumberWDelim(buff, resources.size(), ';'); // amount of resources
    for (const field* f : resources) {
        message::appendNumberWDelim(buff, f->x, ' ');
        message::appendNumberWDelim(buff, f->y, ' ');
        message::appendNumberWDelim(buff, f->getHp(), ';');
    }
    buff.push_back('\n');
    return buff;
}

std::vector<char> rts::game::newPlayerMessage(const player* p) const{
    std::vector<char> buff = {'j'};
    
    message::appendStringWDelim(buff, p->getName(), '\n'); // player name

    return buff;
}

std::vector<char> rts::game::playerLeftMessage(const player* p) const{
    std::vector<char> buff = {'l'};
    
    message::appendStringWDelim(buff, p->getName(), '\n'); // player name

    return buff;
}

std::vector<char> rts::game::newResourceMessage(const field* f) const{
    std::vector<char> buff = {'f'};
    message::appendNumberWDelim(buff, f->x, ' ');
    message::appendNumberWDelim(buff, f->y, ' ');
    message::appendNumberWDelim(buff, f->getHp(), '\n');
    return buff;
}

// ========== GAME LOGIC ==========

void rts::game::handleNewClient(client* client_) {
    player* pl = new player(this, client_);
    allPlayers.insert(pl);
    pl->getClient()->sendToClient(configMessage());
}

void rts::game::loopLogic(){
    // spawn resource and inform players
    if (rand() % 10 == 0) {
        field* f = _board.spawnResource(resourceHp);
        if (f) {
            sendToPlayers(activePlayers, newResourceMessage(f));
        }
    }
    
    // allow units to move again
    for (player* p : activePlayers) {
        for (unit* u : p->units){
            u->movedThisRound = false;
        }
    }

    // send next tick message to room players
    sendToPlayers(activePlayers, {'t', '\n'}); // next game tick
}

void rts::game::clearRoom() {
    for (player* pl : activePlayers) { // remove all player from game room
        pl->removeAllUnits();
    }
    activePlayers.clear();
    nextUnitId = 0;
    _server.loopLogic = [](){};
    printf("room cleared\n");
}

void rts::game::startGame() {
    _board = board(boardX, boardY); // reset board
    printf("game start\n");
    _board.spawnResources(startResources, resourceHp);
    while(!queuedPlayers.empty() && activePlayers.size() < maxPlayers){
        moveQueuedPlayerToRoom();
    }
    _server.loopLogic = std::bind(&rts::game::loopLogic, this);
}

void rts::game::addPlayerToRoom(player* pl) {
    assert(activePlayers.size() < maxPlayers);
    sendToPlayers(activePlayers, newPlayerMessage(pl));
    pl->newUnit(_board.randomEmptyField(true)); // add first unit for the player to control
    activePlayers.insert(pl);
    pl->getClient()->sendToClient(boardStateMessage());
}

void rts::game::addPlayerToQueue(player* pl) {
    queuedPlayers.push_back(pl);
    pl->getClient()->sendToClient({'q','\n'}); // put into queue
}

void rts::game::moveQueuedPlayerToRoom() {
    assert(!queuedPlayers.empty());
    addPlayerToRoom(queuedPlayers.front());
    queuedPlayers.pop_front();
}

void rts::game::removePlayerFromRoomOrQueue(player* pl) {
    // delete from room
    if (activePlayers.find(pl) != activePlayers.end()) {
        activePlayers.erase(pl);
        pl->removeAllUnits();
        sendToPlayers(activePlayers, playerLeftMessage(pl));
        if (!queuedPlayers.empty()) moveQueuedPlayerToRoom();
        if (activePlayers.empty()) clearRoom();
    }

    // delete from queue
    else {
        auto it = std::find(queuedPlayers.begin(), queuedPlayers.end(), pl);
        if (it != queuedPlayers.end()) {
            queuedPlayers.erase(it);
        }
    }
}

void rts::game::sendToPlayers(const std::unordered_set<rts::player*>& players, const std::vector<char>& message) {
    for (player* p : players){
        p->getClient()->sendToClient(message);
    }
}

void rts::game::sendToPlayers(const std::vector<char>& message) const {
    sendToPlayers(activePlayers, message);
}

void rts::game::run() {
    _server.loop(millis);
}

void rts::game::tryJoin(player* pl){
    if (pl->getName() == ""
    || activePlayers.find(pl) != activePlayers.end()
    || std::find(queuedPlayers.begin(), queuedPlayers.end(), pl) != queuedPlayers.end()
    ) {
        pl->getClient()->sendToClient({'n','\n'}); // client unnamed
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

void rts::game::playerLostAllUnits(player* pl) {
    assert(pl);
    assert(activePlayers.find(pl) != activePlayers.end());
    pl->getClient()->sendToClient({'L','\n'});
    removePlayerFromRoomOrQueue(pl);
}

void rts::game::tryWin(player* pl){
    if (pl->units.size() >= unitsToWin) {
        pl->getClient()->sendToClient({'W','\n'});
        for (player* p : activePlayers){
            if (p != pl) p->getClient()->sendToClient({'L','\n'});
        }

        clearRoom();

        if (!queuedPlayers.empty()) startGame();
    }
}

// ========== GETTERS ==========

unsigned int rts::game::getUnitDamage() const {return unitDamage;}
unsigned int rts::game::getUnitHp() const {return unitHp;}

unsigned int rts::game::getNextUnitId() {
    return nextUnitId++;
}

bool rts::game::nameValid(const std::string& name) const {
    for (char c : name){
        if (allowedNameCharacters.find(c) == std::string::npos) return false;
    }
    return true;
}

rts::game::~game() {
    for (player* p : allPlayers){
        delete p;
    }
}