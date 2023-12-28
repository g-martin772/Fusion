#include "fepch.h"
#include "Application.h"

#include "Result.h"
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
    }

    void Application::Run()
    {
        Window* secondWindow = Window::Create();
        secondWindow->Init();
        
        while(m_Running)
        {
            Platform::Update();
            m_PrimaryWindow->OnUpdate();
            secondWindow->OnUpdate();
        }

        secondWindow->ShutDown();
    }

    void Application::Shutdown()
    {
        if(!m_Running)
            return;
        
        FE_INFO("Fusion Engine Shutdown");
        m_Running = false;

        m_PrimaryWindow->ShutDown();
    }

    void Application::Close()
    {
        m_Running = false;
    }
}

