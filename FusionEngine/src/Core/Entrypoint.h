#pragma once

#include "Core/Application.h"

extern Application* CreateApplication();

inline int main(int argc, char** argv)
{
    Application* app = CreateApplication();

    app->Init();

    app->Run();

    app->Shutdown();
    
    return 0;
}