#pragma once

#include <unordered_set>

class server {
private:
    int _fd;
    std::unordered_set<int> clientFds;

public:
    server(const char *port);
    ~server();
};