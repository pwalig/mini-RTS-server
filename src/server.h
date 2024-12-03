#pragma once

#include <unordered_set>

class server {
private:
    int _fd;
    std::unordered_set<int> clientFds;

public:
    server(int argc, const char *const *argv);
    ~server();
};