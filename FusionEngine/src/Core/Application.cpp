#include "fepch.h"
#include "Application.h"

#include "ApplicationConfig.h"
#include "Core/Result.h"
#include "Platform/Platform.h"

namespace FusionEngine
{
    static Application* s_Application;
    
    Application* Application::Get()
    {
        return s_Application;
    }

    Application::Application(ApplicationConfig& config): m_Config(config)
    {
        s_Application = this;
        
        Log::Init();
        Log::Info("Starting Fusion Engine");

        if (config.UseRendering != config.CreateMainWindow)
        {
            Log::Error("Option UseRendering and Create window need to be the same!");
            config.CreateMainWindow = config.UseRendering;
        }

        auto result = Platform::Init();
        FE_ASSERT(result.is_ok(), "Platform Init Failed");
        Platform::State = result.unwrap();

        if (config.CreateMainWindow)
        {
            m_PrimaryWindow = Window::Create(config.ApplicationName, config.MainWindowSize, config.MainWindowPosition);
            m_CurrentWindow = m_PrimaryWindow.get();
            m_FocusedWindow = m_PrimaryWindow.get();
        }
       
        if (config.UseRendering)
        {
            RendererBackend::Init();
        }
    }

    void Application::Run()
    {
        const Unique<Window> secondWindow = Window::Create();
        
        while(m_Running)
        {
            m_CurrentWindow = m_PrimaryWindow.get();
            m_PrimaryWindow->OnUpdate();
            
            m_CurrentWindow = secondWindow.get();
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

        if (m_Config.UseRendering)
        {
            RendererBackend::Shutdown();
        }

        if (m_Config.CreateMainWindow)
        {
            m_PrimaryWindow->ShutDown();
        }
    }

    void Application::Close()
    {
        m_Running = false;
    }
}

