#include "Application.h"

int main(int argc, char **argv)
{
    Logger::setLevel(Logger::Debug);

    Application app(argc, argv);

    return app.start();
}
