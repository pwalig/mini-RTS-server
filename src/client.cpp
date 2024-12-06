#include "client.h"
#include <netdb.h>
#include "error.h"
#include "server.h"
#include <unistd.h>

client::client(server* server_){
    sockaddr_storage clientAddr{0};
    socklen_t clientAddrSize = sizeof(clientAddr);

    // accept new connection
    _fd = accept(server_->fd(), (sockaddr *)&clientAddr, &clientAddrSize);
    if (_fd == -1)
        perror("accept failed");

    // tell who has connected
    char host[NI_MAXHOST], port[NI_MAXSERV];
    getnameinfo((sockaddr *)&clientAddr, clientAddrSize, host, NI_MAXHOST, port, NI_MAXSERV, 0);
    printf("new connection from: %s:%s (fd: %d)\n", host, port, _fd);
}

void client::send(const std::vector<char>& data) {
    int n = write(_fd, data.data(), data.size());
}

client::~client() {
    shutdown(_fd, SHUT_RDWR);
    close(_fd);
    printf("closed client (fd: %d)\n", _fd);
}