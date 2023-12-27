#include "fepch.h"
#include "Window.h"

#include "Input.h"
#include "Layer.h"
#include "Platform/Windows/WindowsWindow.h"

namespace FusionEngine
{
    Window* Window::Create()
    {
        #ifdef FE_WINDOWS
            return new WindowsWindow();
        #elif FE_LINUX
            return nullptr;
        #else
            return nullptr;
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
}
