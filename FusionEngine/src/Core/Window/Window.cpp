#include "fepch.h"
#include "Window.h"

#include "Core/Application.h"
#include "Core/Result.h"
#include "Core/Events/Event.h"

namespace FusionEngine
{
    Window* Window::Create()
    {
        return new Window();
    }

    void Window::Init()
    {
        auto result = Platform::CreateNativeWindow();
        FE_ASSERT(result.is_ok(), "Window creation failed");
        m_PlatformHandle = result.unwrap();

        FE_INFO("Opend window {0}", m_PlatformHandle.Handle);
        
        Event::Register(SystemEvent::WindowClose, FE_EVENT_CALLBACK {
            Window* window = static_cast<Window*>(instance);
            
            if (window->m_PlatformHandle.Handle != context.Sender)
                return false;
            
            if (context.Sender == Application::Get()->GetPrimaryWindow()->m_PlatformHandle.Handle)
            {
                Application::Get()->Shutdown();
                return true;
            }

            window->ShutDown();
            
            return true;
        }, this);
    }

    void Window::OnUpdate()
    {
        
    }

    void Window::ShutDown()
    {
        if (!m_PlatformHandle.Handle)
            return;
        FE_INFO("Closing window {0}", m_PlatformHandle.Handle);
        Platform::DestroyNativeWindow(m_PlatformHandle);
        m_PlatformHandle.Handle = nullptr;
    }

    Window::Window()
    = default;
}
