#include <msg/handler.hpp>
#include <string>
#include <stdexcept>
#include <msg/name.hpp>
#include <msg/unitCommands.hpp>
#include <msg/state.hpp>

std::unordered_map<char, std::function<void(message::handler*)>> message::handler::messageProcessors;

message::handler::handler(client* client_) : _client(client_) {
    _client->onReceive = std::bind(&message::handler::handle, this, std::placeholders::_1);
    _client->onDisconnect = [this](){
        message::state msg(state::action::disconnect);
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
            message::base msg;
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

bool message::handler::tryGetInt(int& i, std::deque<char>::iterator& it, char delim) {
    std::string str;
    if (!tryGetString(str, it, delim)) return false;
    
    try
    {
        i = std::stoi(str);
        return true;
    }
    catch (std::invalid_argument const& ex)
    {
        buffer.erase(buffer.begin(), it);
        msgType = '?';
        return false;
    }
    catch (std::out_of_range const& ex)
    {
        buffer.erase(buffer.begin(), it);
        msgType = '?';
        return false;
    }

    return true;
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
        message::state msg(state::action::joinRequest);
        mh->onNewMessage(&msg);
        mh->msgType = '?';
    };

    messageProcessors['q'] = [](message::handler* mh){
        message::state msg(state::action::quit);
        mh->onNewMessage(&msg);
        mh->msgType = '?';
    };

    messageProcessors['m'] = [](message::handler* mh){
        int a, b, c;
        auto it = mh->buffer.begin();

        if (mh->tryGetInt(a, it, ' ')){
            if (mh->tryGetInt(b, it, ' ')){
                if (mh->tryGetInt(c, it, '\n')) {
                        message::move msg(a, b, c);
                        mh->onNewMessage(&msg);
                        mh->buffer.erase(mh->buffer.begin(), it);
                        mh->msgType = '?';
                }
            }
        }
    };

    messageProcessors['a'] = [](message::handler* mh){
        int a, b;
        auto it = mh->buffer.begin();

        if (mh->tryGetInt(a, it, ' ')){
            if (mh->tryGetInt(b, it, '\n')){
                message::attack msg(a, b);
                mh->onNewMessage(&msg);
                mh->buffer.erase(mh->buffer.begin(), it);
                mh->msgType = '?';
            }
        }
    };

    messageProcessors['d'] = [](message::handler* mh){
        int a;
        auto it = mh->buffer.begin();

        if (mh->tryGetInt(a, it, '\n')){
            message::mine msg(a);
            mh->onNewMessage(&msg);
            mh->buffer.erase(mh->buffer.begin(), it);
            mh->msgType = '?';
        }
    };
}

message::handler::~handler() {
    _client->onReceive = [](const std::vector<char>&) {};
}