#include "fepch.h"
#include "Application.h"

#include "Result.h"
#include "Platform/Platform.h"
#include "Window/Input.h"

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
        m_CurrentWindow = m_PrimaryWindow; // TODO
        m_FocusedWindow = m_PrimaryWindow; // TODO
    }

    void Application::Run()
    {
        const Shared<Window> secondWindow = Window::Create();
        secondWindow->Init();
        
        while(m_Running)
        {
            Platform::Update();
            m_PrimaryWindow->OnUpdate();
            glm::uvec2 pos = Input::GetMouse();
            //FE_TRACE("x: {0}, y: {1}", pos.x, pos.y);
            FE_TRACE("a down: {0}", Input::IsKeyDown(KeyCode::A));
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

