#pragma once

#include <unordered_set>

class client;

class server {
private:
    int _fd;
    std::unordered_set<client*> clients;

public:
    server(const char *port);
    int fd() const;
    void newClient();
    ~server();
};