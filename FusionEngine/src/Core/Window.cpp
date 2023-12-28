#include "fepch.h"
#include "Window.h"

#include "Input.h"
#include "Layer.h"
#include "Result.h"

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
    }

    void Window::OnUpdate()
    {
        
    }

    void Window::ShutDown() const
    {
        Platform::DestroyNativeWindow(m_PlatformHandle);
    }

    Window::Window()
    = default;
}
