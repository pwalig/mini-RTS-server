#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../net/client.h"
#include "../msg/handler.h"

namespace rts {
    class game;
    class unit;

    class player {
    private:
        game* _game;
        message::handler mh;
        std::string _name = "";
        client* _client;

        void handleNewMessage(const message::base* msg);
        
        void setName(const std::string& name);
        void reName(const std::string& name);

        static std::unordered_map<std::string, player*> playersByName;
        static void removeName(const std::string& name);

    public:
        std::unordered_set<unit*> units;

        player(game* game_, client* client_);
        ~player();

        client* getClient() const;
        std::string getName() const;

        static bool nameTaken(const std::string& name);

        void removeAllUnits();
    };
}