#pragma once

#include <string>
#include <unordered_map>

#include "../net/client.h"
#include "../msg/handler.h"

namespace rts {
    class game;

    class player {
    private:
        game* _game;
        message::handler mh;
        std::string _name = "";

        void handleNewMessage(const message::base* msg);

        static std::unordered_map<std::string, player*> playersByName;
        static void removeName(const std::string& name);
    public:
        client* _client;

        player(game* game_, client* client_);
        void name(const std::string& name);
        void reName(const std::string& name);
        static bool nameTaken(const std::string& name);
        ~player();
    };
}