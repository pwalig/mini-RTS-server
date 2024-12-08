#pragma once

#include <unordered_set>
#include <vector>
#include <functional>

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

    std::function<void(client*)> onNewClient = [](client*){};
    std::function<void(client*)> onRemoveClient = [](client*){};
    std::function<void()> loopLogic = [](){};

    int fd() const;
    int epollFd() const;

    // @brief allocates new client and inserts pointer to new client to set of clients
    client* newClient();

    // @brief erases client from set of clients and deletes the client
    void removeClient(client* client_);

    // @param millis time interval in milliseconds, dictating how often to interrupt polling with logic funcion
    void loop(const int& millis = -1);
    
    void sendToAll(const std::vector<char>& data);
    
    ~server();
};