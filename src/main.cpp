#include "Logger/Logger.h"
#include "Mutlithreading/All.h"
#include "Application.h"

int main(int argc, char **argv)
{
    Application app(argc, argv);

    return app.start();
}
