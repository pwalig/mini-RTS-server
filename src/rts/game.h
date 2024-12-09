#pragma once

#include <unordered_set>
#include <deque>

#include "../net/server.h"
#include "player.h"

namespace rts {
    class game {
    private:
        server _server;
        unsigned int maxPlayers = 16;
        std::unordered_set<player*> allPlayers;
        std::unordered_set<player*> activePlayers;
        std::deque<player*> queuedPlayers;

        void handleNewClient(client* client_);
        void loopLogic();

        void addPlayerToRoom(player* pl);
        void addPlayerToQueue(player* pl);
        void moveQueuedPlayerToRoom();

    public:
        game(const char *port);

        void run();
        void tryJoin(player* pl);
        void removePlayerFromRoomOrQueue(player* pl);
        void deletePlayer(player* pl);

        ~game();
    };
}