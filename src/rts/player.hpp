#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include <net/client.hpp>
#include <msg/handler.hpp>

namespace rts {
    class game;
    class unit;
    class field;

    class player {
    private:
        unsigned int nextId = 0;
        std::string _name = "";
        client* _client;
        message::handler mh;
        game* _game;

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
        game* getGame() const;
        std::string getName() const;

        static bool nameTaken(const std::string& name);

        void newUnit(field* field_);

        void removeAllUnits();
    };
}