#include "fepch.h"
#include "SandboxLayer.h"
#include "Core/Entrypoint.h"

using namespace FusionEngine;

class FusionForgeApplication : public Application
{
    
};

Application* CreateApplication()
{
    auto* app = new FusionForgeApplication();
    app->PushLayer(new SandboxLayer);
    return app;
}