#pragma once

#include <unordered_set>

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
    ~server();
};