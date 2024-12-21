#include <net/server.hpp>

#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <cerrno>
#include <stdio.h>
#include <sys/epoll.h>
#include <chrono>

#include <net/error.hpp>
#include <net/client.hpp>

server::server(const char *port) {

    // resolve port to a 'sockaddr*' for a TCP server
    addrinfo *res, hints{};
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;
    int rv = getaddrinfo(nullptr, port, &hints, &res);
    if (rv) error(1, 0, "getaddrinfo: %s", gai_strerror(rv));

    // create socket
    _fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (_fd == -1) error(1, errno, "socket failed");

    // try to set REUSEADDR
    const int one = 1;
    setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

    // bind to address as resolved by getaddrinfo
    if (bind(_fd, res->ai_addr, res->ai_addrlen))
        error(1, errno, "bind failed");

    // enter listening mode
    if (listen(_fd, 1))
        error(1, errno, "listen failed");

    freeaddrinfo(res);

    // create epoll
    _epollFd = epoll_create1(0);

    epoll_event ee;
    ee.events = EPOLLIN;
    ee.data.ptr = nullptr;

    rv = epoll_ctl(_epollFd, EPOLL_CTL_ADD, _fd, &ee);
    if (rv) perror("epoll_ctl failed");
}

int server::fd() const {return _fd;}

int server::epollFd() const {return _epollFd;}

client* server::newClient() {
    client* client_ = new client(this);
    clients.insert(client_);
    onNewClient(client_);
    return client_;
}

void server::deleteClient(client* client_){
    onRemoveClient(client_);
    clients.erase(client_);
    delete client_;
}

void server::loop(const int& millis){
    epoll_event ee;
    int remaining = millis;
    while (true)
    {
        auto begin = std::chrono::steady_clock::now();
        int ready = epoll_wait(_epollFd, &ee, 1, remaining);
        if (ready == 1) {
            if (ee.data.ptr == nullptr){
                newClient();
            }
            else {
                client* client_ = (client*)(ee.data.ptr);
                if (ee.events & EPOLLIN) {
                    client_->receive();
                }
                if (ee.events & EPOLLOUT) {
                    client_->sendFromBuffer();
                }
            }
        }
        auto end = std::chrono::steady_clock::now();
        remaining -= std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

        if (remaining <= 0) {
            auto begin = std::chrono::steady_clock::now();
            loopLogic();
            remaining = millis;
            auto end = std::chrono::steady_clock::now();
            remaining -= std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        }
    }
    
}

void server::sendToAll(const std::vector<char>& data){
    for (client* cli : clients){
        cli->sendToClient(data);
    }
}

server::~server() {
    for (client* cli : clients){
        delete cli;
    }
    close(_fd);
}