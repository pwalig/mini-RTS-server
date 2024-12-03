#include "server.h"
#include "error.h"

int main(int argc, char** argv){
    if (argc != 2) error(1, 0, "Usage: %s <port>", argv[0]);

    server srv(argv[1]);
    
    return 0;
}