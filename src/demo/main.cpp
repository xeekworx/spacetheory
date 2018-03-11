#include "demo_application.h"
#include <memory>

int main(int argc, char *argv[])
{
    int exitcode = 0;
    {
        std::unique_ptr<demo_application> app = std::make_unique<demo_application>();
        exitcode = app->run(argc, argv);
    }
    return exitcode;
}