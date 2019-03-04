#include <iostream>
#include "server.h"

int main(int argc, char **argv) {
    Server server("127.0.0.1", 12343);
    server.start();

    return 0;
}