#include "fepch.h"
#include "Application.h"

#include "Renderer/Pipeline.h"
#include "Renderer/RenderCommand.h"

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

        m_Window = Window::Create();
        m_Window->Init();

        RenderCommand::Init();
    }

    void Application::Run()
    {
        auto startTime = std::chrono::high_resolution_clock::now();
        int frameCount = 0;
        while(m_Running)
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            const auto deltaTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
            frameCount++;

            if (deltaTime >= 1)
            {
                const double fps = static_cast<double>(frameCount) / deltaTime;
                FE_TRACE("Currently running at {0}fps", fps);
                frameCount = 0;
                startTime = currentTime;
            }
            
            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();

            m_Window->OnUpdate();
        }
    }

    void Application::Shutdown()
    {
        FE_INFO("Fusion Engine Shutdown");
        RenderCommand::ShutDown();
        m_Window->ShutDown();
    }

    void Application::Close()
    {
        m_Running = false;
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }
}

