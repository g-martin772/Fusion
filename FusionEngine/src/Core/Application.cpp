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

        m_Time = MakeRef<Time>();

        m_Window = Window::Create();
        m_Window->Init();
        Input::Init();

        RenderCommand::Init();
        Renderer2D::Init();
    }

    void Application::Run()
    {
        while(m_Running)
        {
            m_Time->Update();
            Input::OnUpdate();

            RenderCommand::BeginFrame();
            
            for (Layer* layer : m_LayerStack)
                layer->OnUpdate(m_Time);

            RenderCommand::EndFrame();
            m_Window->OnUpdate();
        }
    }

    void Application::Shutdown()
    {
        FE_INFO("Fusion Engine Shutdown");

        for (Layer* layer : m_LayerStack)
        {
            layer->OnDetach();
            delete layer;
        }

        Renderer2D::ShutDown();
        RenderCommand::ShutDown();
        Input::Shutdown();
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

