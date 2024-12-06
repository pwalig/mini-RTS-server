#pragma once
#include <vector>

class server;

class client {
private:
    int _fd;
    server* owner;
    std::vector<char> buffer;
public:
    client(server* server_);

    client(const client& other) = delete;
    client(client&& other) = delete;
    client& operator=(const client& other) = delete;
    client& operator=(client&& other) = delete;

    int fd() const;
    void send(const std::vector<char>& data);

    ~client();
};