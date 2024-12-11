#include <msg/handler.hpp>
#include <string>
#include <msg/name.hpp>
#include <msg/unitCommands.hpp>

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

bool message::handler::tryGetString(std::string& str, std::deque<char>::iterator& it, char delim) {
    for (; it != buffer.end() && *it != delim; ++it){
        str.push_back(*it);
    }

    if (it != buffer.end() && *it == delim) { // string found
        ++it;
        return true;
    }

    return false;
}

void message::handler::init(){
    messageProcessors['n'] = [](message::handler* mh){
        std::string name;
        auto it = mh->buffer.begin();
        if (mh->tryGetString(name, it)) {
            message::name msg(name);
            mh->onNewMessage(&msg);
            mh->buffer.erase(mh->buffer.begin(), it);
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

    messageProcessors['m'] = [](message::handler* mh){
        std::string sx, sy, dx, dy;
        if (mh->buffer[0] != ' ') return;
        mh->buffer.pop_front();
        auto it = mh->buffer.begin();
        bool success = mh->tryGetString(sx, it, ' ');
        if (success) success &= mh->tryGetString(sy, it, ' ');
        if (success) success &= mh->tryGetString(dx, it, ' ');
        if (success) success &= mh->tryGetString(dy, it, '\n');
        if (success) {
            message::move msg(atoi(dx.c_str()), atoi(dy.c_str()), atoi(sx.c_str()), atoi(sy.c_str()));
            mh->onNewMessage(&msg);
            mh->buffer.erase(mh->buffer.begin(), it);
            mh->msgType = '?';
        }
    };

    messageProcessors['a'] = [](message::handler* mh){
        std::string sx, sy, dx, dy;
        if (mh->buffer[0] != ' ') return;
        mh->buffer.pop_front();
        auto it = mh->buffer.begin();
        bool success = mh->tryGetString(sx, it, ' ');
        if (success) success &= mh->tryGetString(sy, it, ' ');
        if (success) success &= mh->tryGetString(dx, it, ' ');
        if (success) success &= mh->tryGetString(dy, it, '\n');
        if (success) {
            message::attack msg(atoi(dx.c_str()), atoi(dy.c_str()), atoi(sx.c_str()), atoi(sy.c_str()));
            mh->onNewMessage(&msg);
            mh->buffer.erase(mh->buffer.begin(), it);
            mh->msgType = '?';
        }
    };

    messageProcessors['d'] = [](message::handler* mh){
        std::string sx, sy;
        if (mh->buffer[0] != ' ') return;
        mh->buffer.pop_front();
        auto it = mh->buffer.begin();
        bool success = mh->tryGetString(sx, it, ' ');
        if (success) success &= mh->tryGetString(sy, it, '\n');
        if (success) {
            message::mine msg(atoi(sx.c_str()), atoi(sy.c_str()));
            mh->onNewMessage(&msg);
            mh->buffer.erase(mh->buffer.begin(), it);
            mh->msgType = '?';
        }
    };
}

message::handler::~handler() {
    _client->onReceive = [](const std::vector<char>&) {};
}