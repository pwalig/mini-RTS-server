#pragma once

#include <deque>

#include <net/client.hpp>
#include <msg/base.hpp>
#include <string>

namespace message {

    // @brief request Handler chops stream of characters sent by client into separate messages
    class handler {
    private:
        client* _client;
        std::deque<char> buffer;
        char msgType = '?';
        static std::unordered_map<char, std::function<void(handler*)>> messageProcessors;
        
        // @brief handles onRecevie client callback
        void handle(const std::vector<char>& stream);

        bool tryGetString(std::string& str, std::deque<char>::iterator& it, char delim = '\n');

    public:
        handler(client* client_);
        
        // handler should not by copiable nor movable
        // because it passes pointer to itself to client's onRecevie callback
        handler(const handler& other) = delete;
        handler(handler&& other) = delete;
        handler& operator=(const handler& other) = delete;
        handler& operator=(handler&& other) = delete;

        std::function<void(const message::base*)> onNewMessage = [](const message::base*){};

        static void init();

        ~handler();
    };
}