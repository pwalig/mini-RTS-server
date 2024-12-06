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
    
    server(const server& other) = delete;
    server(server&& other) = delete;
    server& operator=(const server& other) = delete;
    server& operator=(server&& other) = delete;

    int fd() const;
    int epollFd() const;
    void newClient();
    void removeClient(client* client_);
    void loop();
    void sendToAll(const std::vector<char>& data);
    ~server();
};