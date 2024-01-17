#include "fepch.h"

#include "EditorLayer.h"
#include "Core/Entrypoint.h"
#include "IO/File.h"

FusionEngine::Application* CreateApplication(int argc, char** argv)
{
     if (!FusionEngine::File::Exists("fusion.conf"))
          FusionEngine::ApplicationConfig::GenerateDefaultConfig("fusion.conf");

     FusionEngine::ApplicationConfig conf;
     conf.LoadFromConfigFile("fusion.conf");
     conf.LoadFromArgs(argc, argv);
     
     auto* app = new FusionEngine::Application(conf);
     return app;
}
