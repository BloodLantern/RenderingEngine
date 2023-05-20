#include <iostream>

#include "core/debug/assertion.hpp"
#include "core/debug/logger.hpp"
#include "core/application.hpp"

int main()
{
    Application app;

    if (!app.Initialize({ 1280, 720 }, "ModernOpenGL"))
    {
        app.Shutdown();
        return -1;
    }
    app.MainLoop();
    app.Shutdown();

    return 0;
}
