#pragma once
#include <iostream>

#include "Base.h"
#include "Window.h"

namespace FusionEngine
{
    FE_API class Application
    {
    public:
        static Application* Get();
        
        void Init();
        void Run();
        void Shutdown();

        void SetRunning(const bool state) { m_Running = state; }
    private:
        bool m_Running = true;

        Ref<Window> m_Window;
    };
}
