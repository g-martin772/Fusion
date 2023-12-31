#include "fepch.h"
#include "Window.h"

#include "Core/Application.h"
#include "Core/Result.h"
#include "Core/Events/EventSystem.h"

namespace FusionEngine
{
    FE_EVENT_CALLBACK_FN(WindowCloseCallback) {
        if (instance != context.Sender)
            return false;
            
        Window* window = static_cast<Window*>(instance);
            
        if (context.Sender == Application::Get()->GetPrimaryWindow())
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

    Window::Window() = default;
}
