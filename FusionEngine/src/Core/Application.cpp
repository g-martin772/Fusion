#include "fepch.h"
#include "Application.h"

#include "Renderer/RenderCommand.h"

namespace FusionEngine
{
    static Application* s_Application;
    
    Application* Application::Get()
    {
        return s_Application;
    }

    void Application::Init()
    {
        s_Application = this;
        
        Log::Init();
        FE_INFO("Startin Fusion Engine");

        m_Window = Window::Create();
        m_Window->Init();

        RenderCommand::Init();
    }

    void Application::Run()
    {
        while(m_Running)
        {
            m_Window->OnUpdate();
        }
    }

    void Application::Shutdown()
    {
        FE_INFO("Fusion Engine Shutdown");
        RenderCommand::ShutDown();
        m_Window->ShutDown();
    }
}

