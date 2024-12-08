#pragma once

#include <string>
#include <unordered_map>

#include "../net/client.h"
#include "../msg/handler.h"

namespace rts {
    class player {
    private:
        client* _client;
        message::handler mh;
        std::string _name = "";

        void handleNewMessage(const message::base* msg);

        static std::unordered_map<std::string, player*> playersByName;
    public:
        player(client* client_);
        void name(const std::string& name);
        void reName(const std::string& name);
        static bool nameTaken(const std::string& name);
    };
}