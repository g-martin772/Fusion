#include "fepch.h"
#include "Application.h"

#include "Log.h"

namespace FusionEngine
{
    void Application::Init()
    {
        Log::Init();
        FE_INFO("Startup");
    }

    void Application::Run()
    {
        while(m_Running)
        {
            FE_INFO("Running");
        }
    }

    void Application::Shutdown()
    {
        FE_INFO("Shutdown");
    }
}

