#include "fepch.h"
#include "Window.h"

#include "Platform/Windows/WindowsWindow.h"

namespace FusionEngine
{
    Ref<Window> Window::Create()
    {
        #ifdef FE_WINDOWS
        return MakeRef<WindowsWindow>();
        #endif
    }
}
