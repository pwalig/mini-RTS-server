#include <rts/game.hpp>
#include <net/error.hpp>
#include <msg/handler.hpp>
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
    if (argc != 2) error(1, 0, "Usage: %s <port>", argv[0]);

    signal(SIGINT, ctrl_c);

    message::handler::init();

    game_ = new rts::game(argv[1]);

    game_->run();

    cleanup();
    return 0;
}