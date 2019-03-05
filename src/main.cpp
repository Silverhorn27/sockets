#include "Application.h"

int main(int argc, char **argv)
{
    Logger::setLevel(Logger::Trace);

    Application app(argc, argv);

    return app.start();
}
