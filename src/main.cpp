#include "server.h"
#include "error.h"
#include <signal.h>

server* server_;

void cleanup(){
    if (server_)
        delete server_;
}

void ctrl_c(int){
    cleanup();
}

int main(int argc, char** argv){
    if (argc != 2) error(1, 0, "Usage: %s <port>", argv[0]);

    // signal(SIGINT, ctrl_c);

    server_ = new server(argv[1]);

    while (true) {
        server_->newClient();
    }

    cleanup();
    return 0;
}