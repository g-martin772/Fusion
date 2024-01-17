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

Application* CreateApplication(int argc, char** argv)
{
    if (!File::Exists("fusion.conf"))
        ApplicationConfig::GenerateDefaultConfig("fusion.conf");

    ApplicationConfig conf;
    conf.LoadFromConfigFile("fusion.conf");
    conf.LoadFromArgs(argc, argv);
        
    auto* app = new Application(conf);
    //Window::Current()->PushLayer(new SandboxLayer());
    return app;
}
