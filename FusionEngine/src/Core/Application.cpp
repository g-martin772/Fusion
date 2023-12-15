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
        FE_INFO("Startin Fusion Engine");
        
        Log::Init();

        m_Time = MakeShared<Time>();

        m_MainWindow = Window::Create();
        m_MainWindow->Init();

        m_TestWindow = Window::Create();
        m_TestWindow->Init();
        
        Input::Init();

        RenderCommand::Init();
        Renderer2D::Init();
    }

    void Application::Run()
    {
        while(m_Running)
        {
            m_Time->Update();
            m_MainWindow->OnUpdate();
            m_TestWindow->OnUpdate();
        }
    }

    void Application::Shutdown()
    {
        FE_INFO("Fusion Engine Shutdown");

        Renderer2D::ShutDown();
        RenderCommand::ShutDown();
        Input::Shutdown();
        m_MainWindow->ShutDown();
        m_TestWindow->ShutDown();
    }

    void Application::Close()
    {
        m_Running = false;
    }
}

