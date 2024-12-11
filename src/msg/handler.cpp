#include <msg/handler.hpp>
#include <string>

#include <msg/name.hpp>

std::unordered_map<char, std::function<void(message::handler*)>> message::handler::messageProcessors;

message::handler::handler(client* client_) : _client(client_) {
    _client->onReceive = std::bind(&message::handler::handle, this, std::placeholders::_1);
    _client->onDisconnect = [this](){
        message::base msg(type::disconnect);
        this->onNewMessage(&msg);
        };
}

void message::handler::handle(const std::vector<char>& stream) {
    buffer.insert(buffer.end(), stream.begin(), stream.end()); // append stream to buffer

    while (!buffer.empty())
    {
        while(std::isspace(buffer[0])){
            buffer.pop_front();
            if (buffer.empty()) return;
        }
        
        if (msgType == '?') {
            msgType = buffer[0];
            buffer.pop_front();
        }

        if (messageProcessors.find(msgType) == messageProcessors.end()){
            message::base msg(type::invalid);
            onNewMessage(&msg);
            msgType = '?';
        }
        else messageProcessors[msgType](this);

        if (msgType != '?') return;
    }
}

void message::handler::init(){
    messageProcessors['n'] = [](message::handler* mh){
        std::string name;
        unsigned long i = 0;
        while (i < mh->buffer.size() && mh->buffer[i] != '\n')
        {
            name.push_back(mh->buffer[i]);
            ++i;
        }

        if (i < mh->buffer.size() && mh->buffer[i] == '\n') {
            mh->buffer.erase(mh->buffer.begin(), mh->buffer.begin() + i + 1);
            message::name msg(name);
            mh->onNewMessage(&msg);
            mh->msgType = '?';
        }
        
    };

    messageProcessors['j'] = [](message::handler* mh){
        message::base msg(type::joinRequest);
        mh->onNewMessage(&msg);
        mh->msgType = '?';
    };

    messageProcessors['q'] = [](message::handler* mh){
        message::base msg(type::quit);
        mh->onNewMessage(&msg);
        mh->msgType = '?';
    };
}

message::handler::~handler() {
    _client->onReceive = [](const std::vector<char>&) {};
}