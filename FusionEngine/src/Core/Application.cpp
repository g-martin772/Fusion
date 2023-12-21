#include "fepch.h"
#include "Application.h"

#include "Input.h"
#include "Renderer/Pipeline.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer2D.h"

namespace FusionEngine
{
    static Application* s_Application;
    
    Application* Application::Get()
    {
        return s_Application;
    }

    Application::Application()
    {
        s_Application = this;
        
        Log::Init();
        FE_INFO("Startin Fusion Engine");
    }

    void Application::Run()
    {
        while(m_Running)
        {
        }
    }

    void Application::Shutdown()
    {
        FE_INFO("Fusion Engine Shutdown");
    }

    void Application::Close()
    {
        m_Running = false;
    }
}

