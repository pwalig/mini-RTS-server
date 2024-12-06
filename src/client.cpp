#include "client.h"
#include <netdb.h>
#include "error.h"
#include "server.h"
#include <unistd.h>
#include <sys/socket.h>

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
    epoll_event ee = inEvent();
    epoll_ctl(server_->epollFd(), EPOLL_CTL_ADD, _fd, &ee);
}

int client::fd() const {return _fd;}

epoll_event client::inEvent() {
    epoll_event ee;
    ee.data.ptr = this;
    ee.events = EPOLLIN;
    return ee;
}
epoll_event client::inoutEvent() {
    epoll_event ee;
    ee.data.ptr = this;
    ee.events = EPOLLIN | EPOLLOUT;
    return ee;
}

void client::sendToClient(const std::vector<char>& data) {
    epoll_event ee = inoutEvent();
    epoll_ctl(owner->epollFd(), EPOLL_CTL_MOD, _fd, &ee);

    buffer = data;
}

void client::sendFromBuffer(){

    int n = send(_fd, buffer.data(), buffer.size(), MSG_DONTWAIT);
    if (n == -1) return;
    if (n == buffer.size()) { // all sent
        buffer.clear();
        epoll_event ee = inEvent();
        epoll_ctl(owner->epollFd(), EPOLL_CTL_MOD, _fd, &ee); // remove EPOLLOUT event
    }
    buffer.erase(buffer.begin(), buffer.begin() + n);
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