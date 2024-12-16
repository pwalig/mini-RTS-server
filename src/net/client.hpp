#pragma once
#include <sys/epoll.h>
#include <sys/socket.h>

#include <vector>
#include <functional>
#include <string>
#include <cstring>

class server;

class client {
private:
    int _fd;
    server* owner; // server to which this client is connected to
    sockaddr_storage clientAddr;
    socklen_t clientAddrSize;

    static const unsigned long bufsiz;
    std::vector<char> buffer;

    epoll_event inEvent(); // non const because epoll_event::data::ptr is not const
    epoll_event inoutEvent(); // non const because epoll_event::data::ptr is not const
    
    client(server* server_);

    client(const client& other) = delete;
    client(client&& other) = delete;
    client& operator=(const client& other) = delete;
    client& operator=(client&& other) = delete;

    // @brief sends to client whatever is left in buffer. Should be a reaction to EPOLLOUT event.
    void sendFromBuffer();
    
    // @brief Reads data from client socket.
    // Can also detect if client has disconnected.
    // In such case informs the server (calls server::deleteClient -> erases client from set of clients and deletes the client)
    // @return the read data or empty vector if client disconnected
    // @note using vector here instead of deque because deque has no member data()
    std::vector<char> receive();

    friend class server;

public:

    std::function<void(const std::vector<char>&)> onReceive = [](const std::vector<char>&){};
    std::function<void()> onDisconnect = [](){};

    int fd() const;
    std::string host() const;
    unsigned int port() const;
    void name(std::string* host, unsigned int* port) const;
    void printName() const;

    // @brief copies data to buffer and sets servers' epoll to wait for EPOLLOUT event.
    // @param data data to be copied to buffer and later sent to client
    void sendToClient(const std::vector<char>& data);

    ~client();
};