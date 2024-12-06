#pragma once
#include <vector>
#include <sys/epoll.h>

class server;

class client {
private:
    int _fd;
    server* owner;

    static const unsigned long bufsiz;
    std::vector<char> buffer;
public:
    client(server* server_);

    client(const client& other) = delete;
    client(client&& other) = delete;
    client& operator=(const client& other) = delete;
    client& operator=(client&& other) = delete;

    int fd() const;
    epoll_event inEvent();
    epoll_event inoutEvent();

    void sendToClient(const std::vector<char>& data);
    void sendFromBuffer();
    std::vector<char> receive();

    ~client();
};