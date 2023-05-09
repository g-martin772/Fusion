#pragma once

#include "Core/Application.h"

extern FusionEngine::Application* CreateApplication();

inline int main(int argc, char** argv)
{
    FusionEngine::Application* app = CreateApplication();

    app->Init();

    app->Run();

    app->Shutdown();
    
    return 0;
}