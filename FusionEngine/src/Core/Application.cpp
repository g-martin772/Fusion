#include "fepch.h"
#include "Application.h"

#include "Core/Result.h"
#include "Platform/Platform.h"

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
        FE_INFO("Starting Fusion Engine");

        auto result = Platform::Init();
        FE_ASSERT(result.is_ok(), "Platform Init Failed");
        Platform::State = result.unwrap();
        
        m_PrimaryWindow = Window::Create();
        m_PrimaryWindow->Init();
        m_CurrentWindow = m_PrimaryWindow;
        m_FocusedWindow = m_PrimaryWindow;
    }

    void Application::Run()
    {
        Window* secondWindow = Window::Create();
        secondWindow->Init();
        
        while(m_Running)
        {
            m_PrimaryWindow->OnUpdate();
            secondWindow->OnUpdate();
            Platform::Update();
        }

        secondWindow->ShutDown();
        delete secondWindow;
    }

    void Application::Shutdown()
    {
        if(!m_Running)
            return;
        
        FE_INFO("Fusion Engine Shutdown");
        m_Running = false;
        
        m_PrimaryWindow->ShutDown();
        delete m_PrimaryWindow;
    }

    void Application::Close()
    {
        m_Running = false;
    }
}

