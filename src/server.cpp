#include "server.h"
#include "error.h"
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <cerrno>
#include <stdio.h>

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
}

int server::fd() const {return _fd;}

void server::newClient() {
    sockaddr_storage clientAddr{0};
    socklen_t clientAddrSize = sizeof(clientAddr);

    // accept new connection
    auto clientFd = accept(_fd, (sockaddr *)&clientAddr, &clientAddrSize);
    if (clientFd == -1)
        perror("accept failed");

    // add client to all clients set
    clientFds.insert(clientFd);

    // tell who has connected
    char host[NI_MAXHOST], port[NI_MAXSERV];
    getnameinfo((sockaddr *)&clientAddr, clientAddrSize, host, NI_MAXHOST, port, NI_MAXSERV, 0);
    printf("new connection from: %s:%s (fd: %d)\n", host, port, clientFd);
}

server::~server() {
    for (int clientFd : clientFds) {
        shutdown(clientFd, SHUT_RDWR);
        // note that if the message cannot be sent out immediately,
        // it will be discarded upon 'close'
        close(clientFd); 
    }
    close(_fd);
}