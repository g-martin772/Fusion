#include "fepch.h"
#include "Window.h"

#include "Core/Application.h"
#include "Core/Result.h"
#include "Core/Events/EventSystem.h"

namespace FusionEngine
{
    FE_EVENT_CALLBACK_FN(WindowCloseCallback) {
        Window* window = static_cast<Window*>(instance);

        if (window->GetPlatformHandle().Handle != context.Sender)
            return false;
            
        if (window == Application::Get()->GetPrimaryWindow())
        {
            Application::Get()->Shutdown();
            return true;
        }

        window->ShutDown();
            
        return true;
    }
    
    Window* Window::Create()
    {
        return new Window();
    }

    void Window::Init()
    {
        auto result = Platform::CreateNativeWindow(this);
        FE_ASSERT(result.is_ok(), "Window creation failed");
        m_PlatformHandle = result.unwrap();

        FE_INFO("Opend window {0}", m_PlatformHandle.Handle);
        
        EventSystem::Register(Event::WindowClose, WindowCloseCallback, this);
    }

    void Window::OnUpdate()
    {
        Platform::UpdateNativeWindow(m_PlatformHandle);
        m_LayerStack.Update();
    }

    void Window::ShutDown()
    {
        if (!m_PlatformHandle.Handle)
            return;
        FE_INFO("Closing window {0}", m_PlatformHandle.Handle);
        Platform::DestroyNativeWindow(m_PlatformHandle);
        EventSystem::Unregister(Event::WindowClose, WindowCloseCallback, this);
    }

    void Window::MakeCurrent()
    {
        Application::Get()->m_CurrentWindow = this;
    }

    Window* Window::Current()
    {
        return Application::Get()->GetCurrentWindow();
    }

    void Window::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
    }

    void Window::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
    }

    void Window::PopLayer(Layer* layer)
    {
        m_LayerStack.PopLayer(layer);
    }

    void Window::PopOverlay(Layer* overlay)
    {
        m_LayerStack.PopOverlay(overlay);
    }

    Window::Window() = default;
}
