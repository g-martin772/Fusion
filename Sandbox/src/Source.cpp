#include "fepch.h"
#include "Core/Entrypoint.h"

using namespace FusionEngine;

class SandboxLayer : public Layer
{
public:
    void OnAttach() override
    {
        FE_INFO("Layer1 Attached");
    }
    
    void OnDetach() override
    {
        FE_INFO("Layer1 Detached");
    }
    
    void OnUpdate() override
    {
        FE_INFO("Layer1 Updated");
    }
};

Application* CreateApplication()
{
    auto* app = new Application();
    Window::Current()->PushLayer(new SandboxLayer());
    return app;
}
