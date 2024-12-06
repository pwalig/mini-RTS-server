#include "server.h"

#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <cerrno>
#include <stdio.h>
#include <sys/epoll.h>

#include "error.h"
#include "client.h"

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
    ee.data.ptr = this;

    rv = epoll_ctl(_epollFd, EPOLL_CTL_ADD, _fd, &ee);
    if (rv) perror("epoll_ctl failed");
}

int server::fd() const {return _fd;}

void server::newClient() {
    clients.insert(new client(this));
}

void server::loop(){
    epoll_event ee;
    while (true)
    {
        int ready = epoll_wait(_epollFd, &ee, 1, -1);
        if (ready != 1) continue;
        if (ee.data.ptr == this){
            newClient();
        }
    }
    
}

server::~server() {
    for (client* cli : clients){
        delete cli;
    }
    close(_fd);
}