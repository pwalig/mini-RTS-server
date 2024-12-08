#include "handler.h"

#include <string>

#include "name.h"

std::unordered_map<char, std::function<void(message::handler*)>> message::handler::messageProcessors;

message::handler::handler(client* client_) : _client(client_) {
    _client->onReceive = std::bind(&message::handler::handle, this, std::placeholders::_1);
}

void message::handler::handle(const std::vector<char>& stream) {
    buffer.insert(buffer.end(), stream.begin(), stream.end()); // append stream to buffer

    while (!buffer.empty())
    {
        if (msgType == '?') {
            msgType = buffer[0];
            buffer.pop_front();
        }

        if (messageProcessors.find(msgType) == messageProcessors.end()){
            message::base msg(type::invalid);
            onNewMessage(&msg);
            msgType = '?';
        }
    }
    

}

void message::handler::init(){
    messageProcessors['n'] = [](message::handler* mh){
        std::string name;
        unsigned long i = 0;
        while (i < mh->buffer.size() && mh->buffer[i] != '\0')
        {
            name.push_back(mh->buffer[i]);
            ++i;
        }

        if (i < mh->buffer.size()) {
            mh->buffer.erase(mh->buffer.begin(), mh->buffer.begin() + i);
            message::name msg(name);
            mh->onNewMessage(&msg);
            mh->msgType = '?';
        }
        
    };
}

message::handler::~handler() {
    _client->onReceive = [](const std::vector<char>&) {};
}