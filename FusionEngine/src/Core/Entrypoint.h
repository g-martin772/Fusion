#pragma once

#include "Core/Application.h"

extern FusionEngine::Application* CreateApplication(int argc, char** argv);

int main(int argc, char** argv)
{
    FusionEngine::Application* app = CreateApplication(argc, argv);
    
    app->Run();
    
    app->Shutdown();
    
    return 0;
}

