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
        Log::Info("Starting Fusion Engine");

        auto result = Platform::Init();
        FE_ASSERT(result.is_ok(), "Platform Init Failed");
        Platform::State = result.unwrap();
        
        m_PrimaryWindow = Window::Create();
        m_PrimaryWindow->Init();
        m_CurrentWindow = m_PrimaryWindow.get();
        m_FocusedWindow = m_PrimaryWindow.get();

        RendererBackend::Init();
    }

    void Application::Run()
    {
        const Unique<Window> secondWindow = Window::Create();
        secondWindow->Init();
        
        while(m_Running)
        {
            m_PrimaryWindow->OnUpdate();
            secondWindow->OnUpdate();
            Platform::Update();
        }

        secondWindow->ShutDown();
    }

    void Application::Shutdown()
    {
        if(!m_Running)
            return;
        
        Log::Info("Fusion Engine Shutdown");
        m_Running = false;

        RendererBackend::Shutdown();
        
        m_PrimaryWindow->ShutDown();
    }

    void Application::Close()
    {
        m_Running = false;
    }
}

