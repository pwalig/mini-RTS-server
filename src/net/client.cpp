#include <net/client.hpp>
#include <net/error.hpp>
#include <net/server.hpp>
#include <unistd.h>
#include <netdb.h>

const unsigned long client::bufsiz = 256;

client::client(server* server_) : owner(server_) {

    sockaddr_storage clientAddr;
    socklen_t clientAddrSize(sizeof(clientAddr));

    // accept new connection
    _fd = accept(server_->fd(), (sockaddr *)&clientAddr, &clientAddrSize);
    if (_fd == -1) {
        perror("accept failed");
        throw connectException("accept failed");
    }
    
    char host_[NI_MAXHOST], port_[NI_MAXSERV];
    getnameinfo((sockaddr *)&clientAddr, clientAddrSize, host_, NI_MAXHOST, port_, NI_MAXSERV, 0);
    _hostname = std::string(host_);
    _port = stoi(std::string(port_));

    // tell who has connected
    printf("new connection from: ");
    printName();
    printf("\n");

    // add to epoll
    epoll_event ee = inEvent();
    epoll_ctl(server_->epollFd(), EPOLL_CTL_ADD, _fd, &ee);
}

int client::fd() const {return _fd;}

std::string client::hostname() const {
    return _hostname;
}

unsigned int client::port() const {
    return _port;
}

void client::printName() const {
    printf("%s:%d (fd: %d)", _hostname.c_str(), _port, _fd);
}

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

    buffer.insert(buffer.end(), data.begin(), data.end());
}

void client::sendFromBuffer(){
    long n = send(_fd, buffer.data(), buffer.size(), MSG_DONTWAIT);
    if (n <= -1) return;
    if ((unsigned long)n == buffer.size()) { // all sent
        buffer.clear();
        epoll_event ee = inEvent();
        epoll_ctl(owner->epollFd(), EPOLL_CTL_MOD, _fd, &ee); // remove EPOLLOUT event
        return;
    }
    buffer.erase(buffer.begin(), buffer.begin() + n);
}

std::vector<char> client::receive() {
    std::vector<char> out;

    out.resize(bufsiz);
    int r = recv(_fd, out.data(), bufsiz, 0);
    if (r >= 0) out.resize(r);

    if (r == 0) {
        onDisconnect();
        owner->deleteClient(this);
        return out;
    }

    while (r > 0)
    {
        unsigned long s = out.size();
        out.resize(s + bufsiz);
        r = recv(_fd, out.data() + s, bufsiz, MSG_DONTWAIT);
        if (r >= 0) out.resize(s + r);
        else out.resize(s);
    }
    
    onReceive(out);
    return out;
}

client::~client() {
    epoll_ctl(owner->epollFd(), EPOLL_CTL_DEL, _fd, nullptr);
    shutdown(_fd, SHUT_RDWR);
    close(_fd);
    printf("closed client ");
    printName();
    printf("\n");
}

client::connectException::connectException(const char* _Message) : runtime_error(_Message) {}
client::connectException::connectException(const std::string& _Message) : runtime_error(_Message) {}
