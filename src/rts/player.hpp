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

        void newUnit(field* field_);

        void removeAllUnits();
    };
}