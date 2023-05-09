#include "fepch.h"
#include "Application.h"

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
            FE_ASSERT(false, "Some error");
        }
    }

    void Application::Shutdown()
    {
        FE_INFO("Shutdown");
    }
}

