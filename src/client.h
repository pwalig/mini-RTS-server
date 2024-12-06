#pragma once
#include <vector>

class server;

class client {
private:
    int _fd;
    std::vector<char> buffer;
public:
    client(server* server_);
    void send(const std::vector<char>& data);
    ~client();
};