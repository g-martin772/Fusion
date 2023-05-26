#pragma once

#include "Core/Application.h"

extern FusionEngine::Application* CreateApplication();

int main(int argc, char** argv)
{
    FusionEngine::Application* app = CreateApplication();
    
    app->Run();
    
    app->Shutdown();
    
    return 0;
}