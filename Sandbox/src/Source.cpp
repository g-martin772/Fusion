#include "fepch.h"
#include "Core/Entrypoint.h"

using namespace FusionEngine;

class SandboxLayer : public Layer
{
public:
    void OnAttach() override
    {
        Log::Info("Layer1 Attached");
    }
    
    void OnDetach() override
    {
        Log::Info("Layer1 Detached");
    }
    
    void OnUpdate() override
    {
        Log::Info("Layer1 Updated");
    }
};

Application* CreateApplication()
{
    auto* app = new Application();
    //Window::Current()->PushLayer(new SandboxLayer());
    return app;
}
