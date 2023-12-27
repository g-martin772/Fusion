#include "fepch.h"
#include "Core/Entrypoint.h"

using namespace FusionEngine;

Application* CreateApplication()
{
    auto* app = new Application();
    app->Run();
    return app;
}