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
        std::unordered_set<player*> allPlayers;
        std::unordered_set<player*> activePlayers;
        std::deque<player*> queuedPlayers;

        unsigned int millis = 1000;
        unsigned int maxPlayers = 16;
        unsigned int boardX = 256;
        unsigned int boardY = 256;
        unsigned int unitsToWin = 50;
        unsigned int startResources = 25;
        unsigned int resourceHp = 100;
        unsigned int unitHp = 100;
        unsigned int unitDamage = 10;
        std::string allowedNameCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_";

        unsigned int nextUnitId = 0;

        std::vector<char> configMessage() const;
        std::vector<char> boardStateMessage() const;

        void handleNewClient(client* client_);
        void loopLogic();
        void clearRoom();
        void startGame();

        void addPlayerToRoom(player* pl);
        void addPlayerToQueue(player* pl);
        void moveQueuedPlayerToRoom();

        static std::unordered_map<std::string, std::function<void(game*, std::ifstream&)>> configValueHandlers;
        static void sendToPlayers(const std::unordered_set<player*>& players, const std::vector<char>& message);

    public:
        board _board;
        
        game(const char *port, const char* configFile);

        void run();

        // @brief response to client 'j' (join) request
        void tryJoin(player* pl);

        // @brief response to client 'q' (quit) request
        void removePlayerFromRoomOrQueue(player* pl);

        // @brief handles player disconnecting
        void deletePlayer(player* pl);

        // @brief handles situation when player lost all units
        void playerLostAllUnits(player* pl);

        // @brief call when player aquires new unit
        // 
        // checks if player won and if so ends the game
        void tryWin(player* pl);

        unsigned int getUnitHp() const;
        unsigned int getUnitDamage() const;
        unsigned int getNextUnitId();
        bool nameValid(const std::string& name) const;

        ~game();
    };
}