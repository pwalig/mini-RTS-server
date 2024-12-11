#include "rts/game.h"
#include "net/error.h"
#include "msg/handler.h"
#include <signal.h>

rts::game* game_;

void cleanup(){
    if (game_)
        delete game_;
        
}

void ctrl_c(int){
    printf("\n");
    cleanup();
    exit(0);
}

int main(int argc, char** argv){
    if (argc != 2 && argc != 3) error(1, 0, "Usage: %s <port> [config file]", argv[0]);

    signal(SIGINT, ctrl_c);

    message::handler::init();

    game_ = new rts::game(argv[1], argc == 3 ? argv[2] : nullptr);

    game_->run();

    cleanup();
    return 0;
}