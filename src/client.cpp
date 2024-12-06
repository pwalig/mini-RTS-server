#include "client.h"
#include <netdb.h>
#include "error.h"
#include "server.h"
#include <unistd.h>
#include <sys/epoll.h>

const unsigned long client::bufsiz = 256;

client::client(server* server_) : owner(server_){
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

    // add to epoll
    epoll_event ee;
    ee.events = EPOLLIN;
    ee.data.ptr = this;

    epoll_ctl(server_->epollFd(), EPOLL_CTL_ADD, _fd, &ee);
}

int client::fd() const {return _fd;}

void client::send(const std::vector<char>& data) {
    int n = write(_fd, data.data(), data.size());
}

std::vector<char> client::receive() {
    std::vector<char> out;
    
    out.resize(bufsiz);
    int r = recv(_fd, out.data(), bufsiz, 0);
    if (r >= 0) out.resize(r);

    if (r == 0) {
        owner->removeClient(this);
        return out;
    }

    while (r > 0)
    {
        unsigned long s = out.size();
        out.resize(s + bufsiz);
        r = recv(_fd, out.data() + s, bufsiz, MSG_DONTWAIT);
        if (r >= 0) out.resize(s + r);
    }

    return out;
}

client::~client() {
    epoll_ctl(owner->epollFd(), EPOLL_CTL_DEL, _fd, nullptr);
    shutdown(_fd, SHUT_RDWR);
    close(_fd);
    printf("closed client (fd: %d)\n", _fd);
}