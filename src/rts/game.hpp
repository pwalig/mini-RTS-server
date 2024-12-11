#pragma once

#include <unordered_set>
#include <deque>

#include <net/server.hpp>
#include <rts/board.hpp>

namespace rts {
    class player;

    class game {
    private:
        server _server;
        unsigned int maxPlayers = 16;
        std::unordered_set<player*> allPlayers;
        std::unordered_set<player*> activePlayers;
        std::deque<player*> queuedPlayers;

        unsigned int millis = 1000;
        unsigned int startResources = 25;

        void handleNewClient(client* client_);
        void loopLogic();
        void clearRoom();
        void startGame();

        void addPlayerToRoom(player* pl);
        void addPlayerToQueue(player* pl);
        void moveQueuedPlayerToRoom();

    public:
        board _board;
        
        game(const char *port);

        void run();
        void tryJoin(player* pl);
        void removePlayerFromRoomOrQueue(player* pl);
        void deletePlayer(player* pl);

        ~game();
    };
}