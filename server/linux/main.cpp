#include "server.h"

int main(int argc, char** argv) {
    // if (argc != 3) {

    // }

    Net::Server Server(argv[1]);

    Server.start();
}