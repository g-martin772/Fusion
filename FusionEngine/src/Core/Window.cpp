#include "fepch.h"
#include "Window.h"

#include "Layer.h"
#include "LayerStack.h"
#include "Platform/Windows/WindowsWindow.h"

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
        OnUpdatePlatform();
        m_Time.Update();
    }

    void Window::ShutDown()
    {
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
