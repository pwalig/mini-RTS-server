#pragma once

#include <unordered_set>
#include <vector>

class client;

class server {
private:
    int _fd;
    int _epollFd;
    std::unordered_set<client*> clients;

public:
    server(const char *port);
    int fd() const;
    int epollFd() const;
    void newClient();
    void removeClient(client* client_);
    void loop();
    void sendToAll(const std::vector<char>& data);
    ~server();
};