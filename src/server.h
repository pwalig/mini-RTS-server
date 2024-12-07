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

    int fd() const;
    int epollFd() const;

    // @brief allocates new client and inserts pointer to new client to set of clients
    void newClient();

    // @brief erases client from set of clients and deletes the client
    void removeClient(client* client_);

    // @param interrupt function to be called at constant interval
    // @param millis time interval in milliseconds, dictating how often to call interrupt function
    void loop(const std::function<void()>& interrupt = [](){}, const int& millis = -1);
    void sendToAll(const std::vector<char>& data);
    
    ~server();
};