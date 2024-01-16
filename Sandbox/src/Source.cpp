#include "fepch.h"
#include "Core/ApplicationConfig.h"
#include "Core/Entrypoint.h"
#include "IO/File.h"

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
    if (!File::Exists("fusion.conf"))
        ApplicationConfig::GenerateDefaultConfig("fusion.conf");

    ApplicationConfig conf;
    conf.LoadFromConfigFile("fusion.conf");
        
    auto* app = new Application();
    //Window::Current()->PushLayer(new SandboxLayer());
    return app;
}
