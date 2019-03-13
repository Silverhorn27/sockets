#include "AppServer.h"

int main(int argc, char **argv)
{
    AppServer app(argc, argv);

    return app.start();
}
