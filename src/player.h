#pragma once

#include <string>
#include <unordered_map>

#include "client.h"

namespace rts {
    class player {
    private:
        client* _client;
        std::string _name = "";

        static std::unordered_map<std::string, player*> playersByName;
    public:
        player(client* client_);
        void name(const std::string& name);
        void reName(const std::string& name);
    };
}