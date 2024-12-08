#pragma once
#include <vector>
#include <functional>
#include <sys/epoll.h>

class server;

class client {
private:
    int _fd;
    server* owner; // server to which this client is connected to

    static const unsigned long bufsiz;
    std::vector<char> buffer;
public:
    client(server* server_);

    client(const client& other) = delete;
    client(client&& other) = delete;
    client& operator=(const client& other) = delete;
    client& operator=(client&& other) = delete;

    std::function<void(const std::vector<char>&)> onReceive = [](const std::vector<char>&){};
    std::function<void()> onDisconnect = [](){};

    int fd() const;
    epoll_event inEvent();
    epoll_event inoutEvent();

    // @brief copies data to buffer and sets servers' epoll to wait for EPOLLOUT event.
    // @param data data to be copied to buffer and later sent to client
    void sendToClient(const std::vector<char>& data);

    // @brief sends to client whatever is left in buffer. Should be a reaction to EPOLLOUT event.
    void sendFromBuffer();
    
    // @brief Reads data from client socket.
    // Can also detect if client has disconnected.
    // In such case informs the server (calls server::removeClient -> erases client from set of clients and deletes the client)
    // @return the read data or empty vector if client disconnected
    // @note using vector here instead of deque because deque has no member data()
    std::vector<char> receive();

    ~client();
};