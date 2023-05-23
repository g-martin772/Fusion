#include "fepch.h"
#include "Input.h"

#include "Platform/Windows/WindowsInput.h"

namespace  FusionEngine
{
    Input* Input::s_Instance;
    
    void Input::Init()
    {
        s_Instance = new WindowsInput();
    }

    void Input::Shutdown()
    {
        delete s_Instance;
    }
}
