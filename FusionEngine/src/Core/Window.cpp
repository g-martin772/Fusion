#include "fepch.h"
#include "Window.h"

#include "Input.h"
#include "Layer.h"
#include "LayerStack.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Renderer/RenderCommand.h"

namespace FusionEngine
{
    Shared<Window> Window::Create()
    {
        #ifdef FE_WINDOWS
        return MakeShared<WindowsWindow>();
        #endif
    }

    void Window::Init()
    {
        InitPlatform();
    }

    void Window::OnUpdate()
    {
        Input::OnUpdate();
        
        RenderCommand::BeginFrame();
            
        for (Layer* layer : m_LayerStack)
            layer->OnUpdate(m_Time);

        RenderCommand::EndFrame();
        
        OnUpdatePlatform();
        m_Time.Update();
    }

    void Window::ShutDown()
    {
        for (Layer* layer : m_LayerStack)
        {
            layer->OnDetach();
            delete layer;
        }
        
        ShutDownPlatform();
    }

    void Window::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Window::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }
}
